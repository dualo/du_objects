#ifndef DUMIDITRACK_H
#define DUMIDITRACK_H

#include "../general/ducontainer.h"


#define MIDI_TRACK_ID_VALUE     "MTrk"
#define MIDI_TRACK_ID_SIZE      4
#define MIDI_TRACK_SIZE_SIZE    4

#define KEY_MIDITRACK_EVENTS    QStringLiteral("Events")


DU_OBJECT(DuArray)
DU_OBJECT(DuMidiBasicEvent)

DU_OBJECT(DuMidiTrack)

class DuMidiTrack : public DuContainer
{
public:
    explicit DuMidiTrack();
    ~DuMidiTrack();

    DuObjectPtr clone() const;

    static DuMidiTrackPtr fromMidiBinary(QDataStream &stream);

    QByteArray toDuMusicBinary() const;
    QByteArray toMidiBinary() const;
    QJsonValue toJson() const;

    int size() const;

    DuArrayPtr getEvents();
    DuArrayConstPtr getEvents() const;
    void setEvents(const DuArrayPtr &array);

    bool appendEvent(const DuMidiBasicEventPtr &event);

private:
    quint32 m_duration;
};

#endif // DUMIDITRACK_H
