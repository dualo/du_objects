#ifndef DUMIDITRACK_H
#define DUMIDITRACK_H

#include "../general/ducontainer.h"


#define MIDI_TRACK_ID_VALUE     "MTrk"
#define MIDI_TRACK_ID_SIZE      4
#define MIDI_TRACK_SIZE_SIZE    4

#define KEY_MIDITRACK_EVENTS    QStringLiteral("Events")


#ifdef Q_OS_WIN
#include "../general/duarray.h"
#else
DU_OBJECT_TEMPLATE(DuArray);
#endif
DU_OBJECT(DuMidiBasicEvent);

DU_OBJECT(DuMidiTrack);

class DuMidiTrack : public DuContainer
{
public:
    DuMidiTrack();
    virtual ~DuMidiTrack() = default;

    DuObjectPtr clone() const;

    static DuMidiTrackPtr fromMidiBinary(QDataStream &stream);

    QByteArray toDuMusicBinary() const;
    QByteArray toMidiBinary() const;
    QJsonValue toJson() const;

    int size() const;

    DuArrayPtr<DuMidiBasicEvent> getEvents();
    DuArrayConstPtr<DuMidiBasicEvent> getEvents() const;
    void setEvents(const DuArrayPtr<DuMidiBasicEvent> &array);

    bool appendEvent(const DuMidiBasicEventPtr &event);

private:
    quint32 m_duration;
};

#endif // DUMIDITRACK_H
