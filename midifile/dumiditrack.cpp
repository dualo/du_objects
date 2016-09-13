#include "dumiditrack.h"

#include "dumidibasicevent.h"
#include "dumidichannelevent.h"
#include "dumidimetaevent.h"
#include "dumidisysexevent.h"

#include "../general/duarray.h"

#include <QDataStream>



DU_OBJECT_IMPL(DuMidiTrack)

DuMidiTrack::DuMidiTrack() :
    DuContainer()
{
    addChild(KEY_MIDITRACK_EVENTS, new DuArray<DuMidiBasicEvent>());
}


DuObjectPtr DuMidiTrack::clone() const
{
    return DuMidiTrackPtr(new DuMidiTrack(*this));
}


DuMidiTrackPtr DuMidiTrack::fromMidiBinary(QDataStream &stream)
{
    QByteArray trackId;

    trackId.resize(MIDI_TRACK_ID_SIZE);
    stream.readRawData(trackId.data(), trackId.size());

    if (trackId != MIDI_TRACK_ID_VALUE)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
               << "DuMidiTrack::fromMidiBinary():\n"
               << "the processed chunk is not a track";

        return DuMidiTrackPtr();
    }

    DuMidiTrackPtr track(new DuMidiTrack);

    bool trackEnded = false;
    quint8 status = 0x00;
    quint32 offset = 0;
    quint32 delta = 0;

    quint32 byteCount;
    stream >> byteCount;

    quint32 totalSize = 0;

    while ((!trackEnded) && totalSize < byteCount)
    {
        DuMidiBasicEventPtr event(new DuMidiBasicEvent());

        event->setTime(stream, offset);
        delta = static_cast<quint32>(event->getTime()) - offset;

        quint8 tmp;
        stream >> tmp;

        if (tmp > 0x7F)
        {
            if (tmp < 0xF0)
            {
                status = tmp;
                event = DuMidiChannelEvent::fromMidiBinary(stream, status);
            }

            else
            {
                status = 0x00;

                if (tmp == 0xFF)
                {
                    event = DuMidiMetaEvent::fromMidiBinary(stream, &trackEnded);
                }
                else if (tmp == 0xF0 || tmp == 0xF7)
                {
                    event = DuMidiSysExEvent::fromMidiBinary(stream, tmp);
                }
                else
                {
                    qCCritical(LOG_CAT_DU_OBJECT)
                            << "DuMidiTrack::fromMidiBinary():\n"
                            << "invalid status byte encountered" << tmp;

                    return DuMidiTrackPtr();
                }
            }
        }
        else
        {
            event = DuMidiChannelEvent::fromMidiBinary(stream, status, tmp);
        }

        if (event == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT)
                    << "DuMidiTrack::fromMidiBinary():\n"
                    << "problem encountered during event generation";

            return DuMidiTrackPtr();
        }

        event->setTime(delta, offset);
        offset += delta;

        track->appendEvent(event);

        if (trackEnded)
            track->m_duration = offset;

        totalSize += static_cast<quint32>(event->size());
    }

    if (totalSize > byteCount)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuMidiTrack::fromMidiBinary():\n"
                << "total track size too big" << byteCount << totalSize;

        return DuMidiTrackPtr();
    }

    return track;
}


QByteArray DuMidiTrack::toDuMusicBinary() const
{
    Q_UNIMPLEMENTED();
    return QByteArray();
}

QByteArray DuMidiTrack::toMidiBinary() const
{
    QByteArray retArray = getEvents()->toMidiBinary();

    quint32 size = static_cast<quint32>(retArray.size());

    retArray.prepend(static_cast<char>(size & 0xFF));
    retArray.prepend(static_cast<char>((size >> 8) & 0xFF));
    retArray.prepend(static_cast<char>((size >> 16) & 0xFF));
    retArray.prepend(static_cast<char>((size >> 24) & 0xFF));
    retArray.prepend(MIDI_TRACK_ID_VALUE);

    return retArray;
}

QJsonValue DuMidiTrack::toJson() const
{
    Q_UNIMPLEMENTED();
    return QJsonValue();
}


int DuMidiTrack::size() const
{
    const DuArrayConstPtr<DuMidiBasicEvent> &events = getChildAs< DuArray<DuMidiBasicEvent> >(KEY_MIDITRACK_EVENTS);

    if (events == NULL)
        return -1;

    return MIDI_TRACK_ID_SIZE + MIDI_TRACK_SIZE_SIZE + events->size();
}


DuArrayPtr<DuMidiBasicEvent> DuMidiTrack::getEvents()
{
    return getChildAs< DuArray<DuMidiBasicEvent> >(KEY_MIDITRACK_EVENTS);
}

DuArrayConstPtr<DuMidiBasicEvent> DuMidiTrack::getEvents() const
{
    return getChildAs< DuArray<DuMidiBasicEvent> >(KEY_MIDITRACK_EVENTS);
}

void DuMidiTrack::setEvents(const DuArrayPtr<DuMidiBasicEvent> &array)
{
    addChild(KEY_MIDITRACK_EVENTS, array);
}

bool DuMidiTrack::appendEvent(const DuMidiBasicEventPtr &event)
{
    DuArrayPtr<DuMidiBasicEvent> tmp = getChildAs< DuArray<DuMidiBasicEvent> >(KEY_MIDITRACK_EVENTS);

    if (tmp == NULL)
        return false;

    return tmp->append(event);
}
