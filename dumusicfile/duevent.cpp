#include "duevent.h"

#include <QJsonObject>
#include <QDebug>

DU_OBJECT_IMPL(DuEvent)

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

DuObjectPtr DuEvent::clone() const
{
    return DuEventPtr(new DuEvent(*this));
}


DuEventPtr DuEvent::fromDuMusicBinary(const music_sample &du_sample)
{
    DuEventPtr event(new DuEvent);
    bool verif = true;

    verif = verif && event->setTime(du_sample.time);
    verif = verif && event->setControl(du_sample.control);
    verif = verif && event->setKeyboard(du_sample.canal);
    verif = verif && event->setNote(du_sample.note & 0x7F);
    verif = verif && event->setValue(du_sample.value);

    if (!verif)
    {
        qCritical() << "DuEvent::fromDuMusicBinary():\n"
                    << "failed to generate DuEvent\n"
                    << "a child was not set properly";

        return DuEventPtr();
    }

    return event;
}


DuEventPtr DuEvent::fromJson(const QJsonObject &jsonEvent)
{
    QJsonValue jsonTime = jsonEvent[KEY_EVENT_TIME];
    QJsonValue jsonCtrl = jsonEvent[KEY_EVENT_CONTROL];
    QJsonValue jsonKbrd = jsonEvent[KEY_EVENT_KEYBOARD];
    QJsonValue jsonNote = jsonEvent[KEY_EVENT_NOTE];
    QJsonValue jsonVal  = jsonEvent[KEY_EVENT_VALUE];

    if (        !jsonTime.isDouble()    ||  !jsonCtrl.isDouble()
            ||  !jsonKbrd.isDouble()    ||  !jsonNote.isDouble()
            ||  !jsonVal.isDouble())
    {
        qCritical() << "DuEvent::fromJson():\n"
                    << "failed to generate DuEvent\n"
                    << "a json key did not contain the proper type";

        return DuEventPtr();
    }


    DuEventPtr event(new DuEvent);
    bool verif = true;

    verif = verif && event->setTime(jsonTime.toInt());
    verif = verif && event->setControl(jsonCtrl.toInt());
    verif = verif && event->setKeyboard(jsonKbrd.toInt());
    verif = verif && event->setNote(jsonNote.toInt());
    verif = verif && event->setValue(jsonVal.toInt());

    if (!verif)
    {
        qCritical() << "DuEvent::fromJson():\n"
                    << "failed to generate DuEvent\n"
                    << "a child was not set properly";

        return DuEventPtr();
    }

    return event;
}


QByteArray DuEvent::toDuMusicBinary() const
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
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EVENT_TIME);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEvent::setTime(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_EVENT_TIME);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuEvent::getControl() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EVENT_CONTROL);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEvent::setControl(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_EVENT_CONTROL);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuEvent::getKeyboard() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EVENT_KEYBOARD);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEvent::setKeyboard(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_EVENT_KEYBOARD);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuEvent::getNote() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EVENT_NOTE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEvent::setNote(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_EVENT_NOTE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuEvent::getValue() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EVENT_VALUE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEvent::setValue(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_EVENT_VALUE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}
