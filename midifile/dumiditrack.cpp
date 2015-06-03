#include "dumiditrack.h"

#include <QDebug>


DU_OBJECT_IMPL(DuMidiTrack)

DuMidiTrack::DuMidiTrack() :
    DuContainer()
{
    addChild(KEY_MIDITRACK_EVENTS, new DuArray());
}

DuMidiTrack::~DuMidiTrack()
{
}


DuObjectPtr DuMidiTrack::clone() const
{
    return DuMidiTrackPtr(new DuMidiTrack(*this));
}


QByteArray DuMidiTrack::toDuMusicBinary() const
{
    Q_UNIMPLEMENTED();
    return QByteArray();
}

QByteArray DuMidiTrack::toMidiBinary() const
{
    QByteArray retArray = getEvents()->toMidiBinary();

    quint32 size = retArray.size();

    retArray.prepend((char)(size & 0xFF));
    retArray.prepend((char)((size >> 8) & 0xFF));
    retArray.prepend((char)((size >> 16) & 0xFF));
    retArray.prepend((char)((size >> 24) & 0xFF));
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
    const DuArrayConstPtr &events = getChildAs<DuArray>(KEY_MIDITRACK_EVENTS);
    if (events == NULL)
    {
        return -1;
    }

    return MIDI_TRACK_ID_SIZE + MIDI_TRACK_SIZE_SIZE + events->size();
}


DuArrayConstPtr DuMidiTrack::getEvents() const
{
    return getChildAs<DuArray>(KEY_MIDITRACK_EVENTS);
}

void DuMidiTrack::setEvents(const DuArrayPtr &array)
{
    addChild(KEY_MIDITRACK_EVENTS, array);
}

bool DuMidiTrack::appendEvent(const DuMidiBasicEventPtr &event)
{
    DuArrayPtr tmp = getChildAs<DuArray>(KEY_MIDITRACK_EVENTS);
    if (tmp == NULL)
    {
        return false;
    }

    tmp->append(event);
    return true;
}
