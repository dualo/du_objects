#include "dumidifile.h"

#include <QDebug>

DuMidiFile::DuMidiFile() :
    format(0x0000),
    division(0x0000),
    tracks(QList<DuMidiTrack*>())
{
    tracks.clear();
}

DuMidiFile::DuMidiFile(QIODevice *input) :
    format(0x0000),
    division(0x0000),
    tracks(QList<DuMidiTrack*>())
{
    tracks.clear();
    parseFile(input);
}

DuMidiFile::~DuMidiFile()
{
    qDeleteAll(tracks);
}


quint16 DuMidiFile::getFormat()
{
    return format;
}

quint16 DuMidiFile::getDivision()
{
    return division;
}

QList<DuMidiTrack *>& DuMidiFile::getTracks()
{
    return tracks;
}


void DuMidiFile::setFormat(quint16 value)
{
    format = value;
}

void DuMidiFile::setDivision(quint16 value)
{
    division = value;
}

void DuMidiFile::appendTrack(DuMidiTrack *track)
{
    tracks.append(track);
}


QByteArray DuMidiFile::toByteArray()
{
    QByteArray array;
    array.clear();

    count = tracks.count();

    array.append(MIDI_HEADER_ID_VALUE);
    array.append((char)((MIDI_HEADER_CHUNK_SIZE >> 24) & 0xFF));
    array.append((char)((MIDI_HEADER_CHUNK_SIZE >> 16) & 0xFF));
    array.append((char)((MIDI_HEADER_CHUNK_SIZE >> 8) & 0xFF));
    array.append((char)(MIDI_HEADER_CHUNK_SIZE & 0xFF));

    array.append((char)((format >> 8) & 0xFF));
    array.append((char)(format & 0xFF));
    array.append((char)((count >> 8) & 0xFF));
    array.append((char)(count & 0xFF));
    array.append((char)((division >> 8) & 0xFF));
    array.append((char)(division & 0xFF));

    for (int i = 0; i < count; i++)
    {
        array += tracks[i]->toByteArray();
    }

    return array;
}


QList<DuMidiMetaEvent *> DuMidiFile::findMetaEvents(quint16 trackIndex, quint8 type)
{
    QList<DuMidiMetaEvent *> metaEvents;
    metaEvents.clear();

    DuMidiTrack *track = tracks[trackIndex];
    QList<DuAbstractMidiEvent *> &events = track->getEvents();

    for(int i = 0; i < events.count(); i++)
    {
        if (events[i]->getStatus() == 0xFF)
        {
            DuMidiMetaEvent *tmpEvent = dynamic_cast<DuMidiMetaEvent *>(events[i]);
            if (tmpEvent->getType() == type)
                metaEvents.append(tmpEvent);
        }
    }

    return metaEvents;
}

/*
void MidiFile::sortEvents(
        QMap<quint16, QMap<quint8, QList<MidiChannelEvent *>>> *channelMap,
        QMap<quint16, QMap<quint8, QList<MidiMetaEvent *>>> *metaMap)
{
    for(int i = 0; i < tracks.count(); i++)
    {
        QMap<quint8, QList<MidiChannelEvent *>> trackChannelMap;
        QMap<quint8, QList<MidiMetaEvent *>> trackMetaMap;
        trackChannelMap.clear();
        trackMetaMap.clear();

        tracks[i]->sortEvents(&trackChannelMap, &trackMetaMap);
        channelMap->insert(i, trackChannelMap);
        metaMap->insert(i, trackMetaMap);
    }
}
*/

void DuMidiFile::parseFile(QIODevice *input)
{
    QDataStream stream(input);
    QByteArray headerId;
    quint32 headerSize;

    headerId.resize(MIDI_HEADER_ID_SIZE);
    stream.readRawData(headerId.data(), headerId.size());
    stream >> headerSize;

    if (headerId == MIDI_HEADER_ID_VALUE && headerSize == MIDI_HEADER_CHUNK_SIZE)
    {
        stream >> format;
        if (format == 0x00 || format == 0x01)
        {
            stream >> count;
            stream >> division;

            for (int i = 0; i < count; i++)
            {
                DuMidiTrack* track = parseTrack(stream);
                tracks.append(track);
            }
        }
        else
        {
            qCritical() << "The software cannot read MIDI format " << format;
        }
    }
    else
    {
        qCritical() << "The selected file is not a Standard MIDI File";
    }
}


DuMidiTrack* DuMidiFile::parseTrack(QDataStream &stream)
{
    DuMidiVariableLength::getInstance()->resetTime();

    DuMidiTrack* track = new DuMidiTrack();
    QByteArray trackId;

    trackId.resize(MIDI_TRACK_ID_SIZE);
    stream.readRawData(trackId.data(), trackId.size());

    if (trackId == MIDI_TRACK_ID_VALUE)
    {
        bool trackEnded = false;
        quint8 runningStatus = 0x00;

        stream.skipRawData(MIDI_TRACK_SIZE_SIZE);

        while (!trackEnded)
        {
            DuAbstractMidiEvent* event =
                    parseEvent(stream, &runningStatus, &trackEnded);
            if (event != NULL) track->appendEvent(event);
        }
    }
    else
    {
        qCritical() << "The processed chunk is not a track";
    }

    return track;
}


DuAbstractMidiEvent* DuMidiFile::parseEvent(QDataStream &stream,
                                            quint8 *runningStatus,
                                            bool *trackEnded)
{
    DuAbstractMidiEvent* event = NULL;
    quint32 tmpTime;
    quint8 tmp;

    tmpTime = DuMidiVariableLength::getInstance()->getTime(stream);
    stream >> tmp;

    if (tmp > 0x7F)
    {
        if (tmp < 0xF0)
        {
            (*runningStatus) = tmp;
            event = parseChannelEvent(stream, runningStatus);
        }

        else
        {
            (*runningStatus) = 0x00;

            if (tmp == 0xFF)
                event = parseMetaEvent(stream, trackEnded);

            else
                event = parseSysExEvent(stream, tmp);
        }
    }
    else
        event = parseChannelEvent(stream, runningStatus, &tmp);

    if (event != NULL) event->setTime(tmpTime);

    return event;
}


DuMidiChannelEvent* DuMidiFile::parseChannelEvent(QDataStream &stream,
                                                  quint8 *runningStatus,
                                                  quint8 *byte)
{
    DuMidiChannelEvent* channelEvent = new DuMidiChannelEvent;

    quint8 tmpCommand = (*runningStatus) / 16;
    //quint8 tmpChannel = (*runningStatus) % 16;

    QByteArray tmpArray;
    tmpArray.clear();

    switch(tmpCommand)
    {
    case DuMidiChannelEvent::NoteOff:
    case DuMidiChannelEvent::NoteOn:
    case DuMidiChannelEvent::KeyAftertouch:
    case DuMidiChannelEvent::ControlChange:
    case DuMidiChannelEvent::PitchWheelChange:
    {
        channelEvent->setKey(*byte);

        quint8 tmp;
        stream >> tmp;
        channelEvent->setValue(tmp);

        break;
    }
    case DuMidiChannelEvent::ProgramChange:
    case DuMidiChannelEvent::ChannelPressure:
    {
        channelEvent->setValue(*byte);
        break;
    }
    default:
        qCritical() << "Invalid channel command";
    }

    channelEvent->setStatus(*runningStatus);

    return channelEvent;
}

DuMidiChannelEvent* DuMidiFile::parseChannelEvent(QDataStream &stream,
                                                  quint8 *runningStatus)
{
    quint8 tmp;
    stream >> tmp;

    DuMidiChannelEvent* channelEvent = parseChannelEvent(stream, runningStatus, &tmp);

    return channelEvent;
}


DuMidiMetaEvent* DuMidiFile::parseMetaEvent(QDataStream &stream, bool *trackEnded)
{
    DuMidiMetaEvent* metaEvent = new DuMidiMetaEvent();

    quint8 tmpType;
    quint32 tmpLength;

    stream >> tmpType;
    tmpLength = DuMidiVariableLength::getInstance()->getLength(stream);

    metaEvent->setType(tmpType);
    metaEvent->setLength(tmpLength);
    metaEvent->setDataBytes(stream);

    (*trackEnded) = (tmpType == 0x2F);

    return metaEvent;
}


DuMidiSysExEvent* DuMidiFile::parseSysExEvent(QDataStream &stream, quint8 status)
{
    DuMidiSysExEvent* sysExEvent = new DuMidiSysExEvent();

    sysExEvent->setStatus(status);
    sysExEvent->setLength(DuMidiVariableLength::getInstance()->getLength(stream));
    sysExEvent->setDataBytes(stream);

    return sysExEvent;
}
