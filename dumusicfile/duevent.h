#ifndef DUEVENT_H
#define DUEVENT_H

#include "../general/ducontainer.h"
#include "../general/dunumeric.h"

#include "../miditodumusic/midiconversionhelper.h"


#define KEY_EVENT_TIME          "Time"
#define KEY_EVENT_CONTROL       "Control"
#define KEY_EVENT_CANAL         "Canal"
#define KEY_EVENT_KEYBOARD      "Keyboard"
#define KEY_EVENT_NOTE          "Note"
#define KEY_EVENT_VALUE         "Value"


DU_OBJECT(DuEvent)

class DuEvent : public DuContainer
{
public:
    explicit DuEvent(int time, int control, int canal, int note, int value);
    explicit DuEvent();
    ~DuEvent();

    virtual DuObjectPtr clone() const;

    static DuEventPtr fromDuMusicBinary(const music_sample &du_sample);
    static DuEventPtr fromJson(const QJsonObject &jsonEvent);
    static DuEventPtr fromMidi(const DuMidiChannelEventPtr &channelEvent,
                               const MidiConversionHelper &helper, int loopIndex);

    QByteArray toDuMusicBinary() const;
    DuMidiChannelEventPtr toDuMidiChannelEvent(quint32 prevTime, quint8 prevType,
                                               bool isPercu, int instrKeyMap) const;

    int size() const;

    int getTime() const;
    bool setTime(int value);

    int getControl() const;
    bool setControl(int value);

    int getCanal() const;
    bool setCanal(int value);

    int getKeyboard() const;
    bool setKeyboard(int value);

    int getNote() const;
    bool setNote(int value);

    int getValue() const;
    bool setValue(int value);
};

#endif // DUEVENT_H
