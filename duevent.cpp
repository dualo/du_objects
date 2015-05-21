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
    DuEvent *event = new DuEvent;
    bool verif = true;

    verif = verif && event->setTime(du_sample.time);
    verif = verif && event->setControl(du_sample.control);
    verif = verif && event->setKeyboard(du_sample.canal);
    verif = verif && event->setNote(du_sample.note & 0x7F);
    verif = verif && event->setValue(du_sample.value);

    if (!verif)
    {
        delete event;
        return NULL;
    }

    return event;
}


DuEvent *DuEvent::fromJson(const QJsonObject &jsonEvent)
{
    QJsonValue jsonTime = jsonEvent[KEY_EVENT_TIME];
    QJsonValue jsonCtrl = jsonEvent[KEY_EVENT_CONTROL];
    QJsonValue jsonKbrd = jsonEvent[KEY_EVENT_KEYBOARD];
    QJsonValue jsonNote = jsonEvent[KEY_EVENT_NOTE];
    QJsonValue jsonVal  = jsonEvent[KEY_EVENT_VALUE];

    if (        !jsonTime.isDouble()    ||  !jsonCtrl.isDouble()
            ||  !jsonKbrd.isDouble()    ||  !jsonNote.isDouble()
            ||  !jsonVal.isDouble())

        return NULL;


    DuEvent *event = new DuEvent;
    bool verif = true;

    verif = verif && event->setTime(jsonTime.toInt());
    verif = verif && event->setControl(jsonCtrl.toInt());
    verif = verif && event->setKeyboard(jsonKbrd.toInt());
    verif = verif && event->setNote(jsonNote.toInt());
    verif = verif && event->setValue(jsonVal.toInt());

    if (!verif)
    {
        delete event;
        return NULL;
    }

    return event;
}


QByteArray DuEvent::toDuMusicFile() const
{
    music_sample du_sample;

    int tmpNum = 0;


    tmpNum = getTime();
    if (tmpNum == -1)
        return QByteArray();
    du_sample.time = tmpNum;

    tmpNum = getControl();
    if (tmpNum == -1)
        return QByteArray();
    du_sample.control = tmpNum;

    tmpNum = getKeyboard();
    if (tmpNum == -1)
        return QByteArray();
    du_sample.canal = tmpNum;

    tmpNum = getNote();
    if (tmpNum == -1)
        return QByteArray();
    du_sample.note = ((du_sample.canal << 4) & 0x80) | tmpNum;

    tmpNum = getValue();
    if (tmpNum == -1)
        return QByteArray();
    du_sample.value = tmpNum;


    return QByteArray((char *)&(du_sample), MUSIC_SAMPLE_SIZE);
}


int DuEvent::size() const
{
    return MUSIC_SAMPLE_SIZE;
}


int DuEvent::getTime() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EVENT_TIME));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEvent::setTime(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EVENT_TIME));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuEvent::getControl() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EVENT_CONTROL));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEvent::setControl(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EVENT_CONTROL));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuEvent::getKeyboard() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EVENT_KEYBOARD));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEvent::setKeyboard(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EVENT_KEYBOARD));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuEvent::getNote() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EVENT_NOTE));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEvent::setNote(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EVENT_NOTE));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuEvent::getValue() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EVENT_VALUE));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEvent::setValue(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EVENT_VALUE));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}
