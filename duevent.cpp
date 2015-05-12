#include "duevent.h"

DuEvent::DuEvent(int time, int control, int keyboard, int note, int value) :
    DuContainer()
{
    addChild(KEY_EVENT_TIME, new DuNumeric(time));

    addChild(KEY_EVENT_CONTROL,
             new DuNumeric(control, NUMERIC_DEFAULT_SIZE,
                           6, 0, 0));

    addChild(KEY_EVENT_KEYBOARD,
             new DuNumeric(keyboard, NUMERIC_DEFAULT_SIZE,
                           0xFF, 0x00, 0x00));

    addChild(KEY_EVENT_NOTE,
             new DuNumeric(note, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00, 0x40));

    addChild(KEY_EVENT_VALUE,
             new DuNumeric(value, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00, 0x40));
}

DuEvent::DuEvent()
{
    addChild(KEY_EVENT_TIME, new DuNumeric(0));

    addChild(KEY_EVENT_CONTROL,
             new DuNumeric(0, NUMERIC_DEFAULT_SIZE,
                           6, 0));

    addChild(KEY_EVENT_KEYBOARD,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0xFF, 0x00));

    addChild(KEY_EVENT_NOTE,
             new DuNumeric(0x40, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KEY_EVENT_VALUE,
             new DuNumeric(0x40, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));
}

DuEvent::~DuEvent()
{
}


DuEvent *DuEvent::fromDuMusicFile(const music_sample &du_sample)
{
    DuEvent *event = new DuEvent(du_sample.time, du_sample.control, du_sample.canal,
                                 (du_sample.note & 0x7F), du_sample.value);

    return event;
}


DuEvent *DuEvent::fromJson(const QJsonObject &jsonEvent)
{
    DuEvent *event = new DuEvent;
    const QStringList &keyList = event->keys();

    bool test = true;
    for (int i = 0; i < keyList.count(); i++)
    {
        test = test && jsonEvent.contains(keyList[i]);
    }

    if (!test)
        return NULL;

    event->setTime(jsonEvent[KEY_EVENT_TIME].toInt());
    event->setControl(jsonEvent[KEY_EVENT_CONTROL].toInt());
    event->setKeyboard(jsonEvent[KEY_EVENT_KEYBOARD].toInt());
    event->setNote(jsonEvent[KEY_EVENT_NOTE].toInt());
    event->setValue(jsonEvent[KEY_EVENT_VALUE].toInt());

    return event;
}


QByteArray DuEvent::toDuMusicFile()
{
    music_sample du_sample;
    du_sample.time = getTime();
    du_sample.control = getControl();
    du_sample.canal = getKeyboard();
    du_sample.note = ((du_sample.canal << 4) & 0x80) | getNote();
    du_sample.value = getValue();

    return QByteArray((char *)&(du_sample), MUSIC_SAMPLE_SIZE);
}


int DuEvent::getTime() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EVENT_TIME));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuEvent::setTime(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EVENT_TIME));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


int DuEvent::getControl() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EVENT_CONTROL));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuEvent::setControl(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EVENT_CONTROL));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


int DuEvent::getKeyboard() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EVENT_KEYBOARD));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuEvent::setKeyboard(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EVENT_KEYBOARD));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


int DuEvent::getNote() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EVENT_NOTE));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuEvent::setNote(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EVENT_NOTE));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


int DuEvent::getValue() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EVENT_VALUE));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuEvent::setValue(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EVENT_VALUE));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}
