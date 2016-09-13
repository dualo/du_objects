#ifndef DUEVENT_H
#define DUEVENT_H

#include "../general/ducontainer.h"


class MidiConversionHelper;
DU_OBJECT(DuMidiChannelEvent);
DU_OBJECT(DuSound);

DU_OBJECT(DuEvent);

class DuEvent : public DuContainer
{
public:
    DuEvent();
    virtual ~DuEvent() = default;

    virtual DuObjectPtr clone() const;

    static DuEventPtr fromDuMusicBinary(const music_sample &du_sample);
    static DuEventPtr fromMidi(const DuMidiChannelEventPtr &channelEvent,
                               const DuSoundConstPtr &sound,
                               const MidiConversionHelper &helper);

    QByteArray toDuMusicBinary() const;
    DuMidiChannelEventPtr toDuMidiChannelEvent(quint32 prevTime, quint8 prevType,
                                               int presetOctave, int transpose,
                                               bool isPercu, int instrKeyMap) const;

    int size() const;

    DU_KEY_ACCESSORS(Time,      int)
    DU_KEY_ACCESSORS(Control,   int)
    DU_KEY_ACCESSORS(Canal,     int)
    DU_KEY_ACCESSORS(Keyboard,  int)
    DU_KEY_ACCESSORS(Note,      int)
    DU_KEY_ACCESSORS(Value,     int)
};

#endif // DUEVENT_H
