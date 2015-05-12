#ifndef DUEVENT_H
#define DUEVENT_H

#include "ducontainer.h"
#include "dunumeric.h"


#define KEY_EVENT_TIME          "Time"
#define KEY_EVENT_CONTROL       "Control"
#define KEY_EVENT_KEYBOARD      "Keyboard"
#define KEY_EVENT_NOTE          "Note"
#define KEY_EVENT_VALUE         "Value"


class DuEvent : public DuContainer
{
public:
    explicit DuEvent(int time, int control, int keyboard, int note, int value);
    explicit DuEvent();
    ~DuEvent();

    static DuEvent *fromDuMusicFile(const music_sample &du_sample);
    static DuEvent *fromJson(const QJsonObject &jsonEvent);

    QByteArray toDuMusicFile();

    int getTime() const;
    void setTime(int value);

    int getControl() const;
    void setControl(int value);

    int getKeyboard() const;
    void setKeyboard(int value);

    int getNote() const;
    void setNote(int value);

    int getValue() const;
    void setValue(int value);
};

#endif // DUEVENT_H
