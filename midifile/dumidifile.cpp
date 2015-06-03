#include "dumidifile.h"

#include <QDebug>


DU_OBJECT_IMPL(DuMidiFile)

DuMidiFile::DuMidiFile() :
    DuContainer()
{
    addChild(KEY_MIDIFILE_FORMAT,   new DuNumeric(1, 2, 2, 0));

    addChild(KEY_MIDIFILE_DIVISION, new DuNumeric(DUMUSIC_DIVISION, 2, 0xFFFF, 0x0000));

    addChild(KEY_MIDIFILE_TRACKS,   new DuArray());
}

DuMidiFile::~DuMidiFile()
{
}


DuObjectPtr DuMidiFile::clone() const
{
    return DuMidiFilePtr(new DuMidiFile(*this));
}


DuMidiFilePtr DuMidiFile::fromMidiBinary(QIODevice *input)
{
    QDataStream stream(input);
    QByteArray headerId;
    quint32 headerSize;

    headerId.resize(MIDI_HEADER_ID_SIZE);
    stream.readRawData(headerId.data(), headerId.size());
    stream >> headerSize;

    if (headerId != MIDI_HEADER_ID_VALUE || headerSize != MIDI_HEADER_CHUNK_SIZE)
    {
        qCritical() << "The selected file is not a Standard MIDI File";

        return DuMidiFilePtr();
    }
    else
    {
        //TODO: refactoring
        quint16 format;
        stream >> format;
        if (format == 0x00 || format == 0x01)
        {
            DuMidiFilePtr midiFile(new DuMidiFile());

            quint16 count;
            quint16 division;
            //TODO: refactoring
            stream >> count;
            stream >> division;

            for (int i = 0; i < count; i++)
            {
                DuMidiTrack* track = midiFile->parseTrack(stream);
                //TODO: refactoring
                //tracks.append(track);
            }

            return midiFile;
        }
        else
        {
            qCritical() << "The software cannot read MIDI format" << format;

            return DuMidiFilePtr();
        }
    }
}


QByteArray DuMidiFile::toDuMusicBinary() const
{
    Q_UNIMPLEMENTED();
    return QByteArray();
}

QByteArray DuMidiFile::toMidiBinary() const
{
    QByteArray array;
    array.clear();

    int format = getFormat();
    int count = getTracks()->count();
    int division = getDivision();

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
/*
    for (int i = 0; i < count; i++)
    {
        array += tracks[i]->toMidiBinary();
    }
*/
    return array;
}


QJsonValue DuMidiFile::toJson() const
{
    Q_UNIMPLEMENTED();
    return QJsonValue();
}


int DuMidiFile::size() const
{
    //TODO: implementation
    return -1;
}


int DuMidiFile::getFormat() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_MIDIFILE_FORMAT);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuMidiFile::setFormat(quint16 value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_MIDIFILE_FORMAT);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuMidiFile::getDivision() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_MIDIFILE_DIVISION);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuMidiFile::setDivision(quint16 value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_MIDIFILE_DIVISION);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


DuArrayConstPtr DuMidiFile::getTracks() const
{
    return getChildAs<DuArray>(KEY_MIDIFILE_TRACKS);
}

void DuMidiFile::setTracks(const DuArrayPtr &array)
{
    addChild(KEY_MIDIFILE_TRACKS, array);
}

bool DuMidiFile::appendTrack(const DuMidiTrackPtr &track)
{
    DuArrayPtr tmp = getChildAs<DuArray>(KEY_MIDIFILE_TRACKS);

    if (tmp == NULL)
        return false;

    tmp->append(track);
    return true;
}


DuMidiTrack* DuMidiFile::parseTrack(QDataStream &stream)
{
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
            DuMidiBasicEvent* event =
                    parseEvent(stream, &runningStatus, &trackEnded);
            //TODO: refactoring
            //if (event != NULL) track->appendEvent(event);
        }
    }
    else
    {
        qCritical() << "The processed chunk is not a track";
    }

    return track;
}


DuMidiBasicEvent *DuMidiFile::parseEvent(QDataStream &stream,
                                            quint8 *runningStatus,
                                            bool *trackEnded)
{
    DuMidiBasicEvent* event = NULL;
    quint32 tmpTime;
    quint8 tmp;

    //TODO: refactoring
    //tmpTime = DuMidiVariableLength::getInstance()->getTime(stream);
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

    //TODO: refactoring
    //if (event != NULL) event->setTime(tmpTime);

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

    stream >> tmpType;

    metaEvent->setType(tmpType);
    //TODO: refactoring
    //metaEvent->setLength(DuMidiVariableLength::getInstance()->getLength(stream));
    metaEvent->setData(stream);

    (*trackEnded) = (tmpType == 0x2F);

    return metaEvent;
}


DuMidiSysExEvent* DuMidiFile::parseSysExEvent(QDataStream &stream, quint8 status)
{
    DuMidiSysExEvent* sysExEvent = new DuMidiSysExEvent();

    sysExEvent->setStatus(status);
    //TODO: refactoring
    //sysExEvent->setLength(DuMidiVariableLength::getInstance()->getLength(stream));
    sysExEvent->setData(stream);

    return sysExEvent;
}
