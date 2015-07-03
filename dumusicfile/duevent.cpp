#include "duevent.h"

#include <QJsonObject>


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

DuEvent::DuEvent() :
    DuContainer()
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
    verif = verif && event->setValue(du_sample.value & 0x7F);

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuEvent::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
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
        qCCritical(LOG_CAT_DU_OBJECT) << "DuEvent::fromJson():\n"
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
        qCWarning(LOG_CAT_DU_OBJECT) << "DuEvent::fromJson():\n"
                   << "an attribute was not properly set";
    }

    return event;
}

DuEventPtr DuEvent::fromMidi(const DuMidiChannelEventPtr &channelEvent,
                             const MidiConversionHelper &helper,
                             int loopIndex)
{
    if (!helper.isValid())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuEvent::fromMidi():\n"
                                      << "failed to generate DuEvent\n"
                                      << "invalid conversion helper";

        return DuEventPtr();
    }

    //This case should not occur since it is already tested in DuLoop::fromMidi()
    if (channelEvent->getType() == DuMidiChannelEvent::ProgramChange)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuEvent::fromMidi():\n"
                                      << "failed to generate DuEvent\n"
                                      << "invalid channel event type";

        return DuEventPtr();
    }

    DuEventPtr event(new DuEvent);
    bool verif = true;

    verif = verif && event->setTime(channelEvent->getTime());
    verif = verif && event->setControl(channelEvent->getType() - 0x08);
    verif = verif && event->setValue(channelEvent->getValue());

    int key = channelEvent->getKey();

    if (!helper.isPercu(loopIndex))
    {
        verif = verif && event->setKeyboard(helper.fetchKeyboard(key, loopIndex));
        verif = verif && event->setNote(key);
    }
    else
    {
        verif = verif && event->setKeyboard(0);
        verif = verif && event->setNote(helper.fetchPercuKey(key, loopIndex));
    }

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuEvent::fromMidi():\n"
                                     << "an attribute was not properly set";
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
    du_sample.time = (quint32)tmpNum;

    tmpNum = getControl();
    if (tmpNum == -1)
        return QByteArray();
    du_sample.control = (quint8)tmpNum;

    tmpNum = getKeyboard();
    if (tmpNum == -1)
        return QByteArray();
    du_sample.canal = (quint8)tmpNum;

    tmpNum = getNote();
    if (tmpNum == -1)
        return QByteArray();
    du_sample.note = ((du_sample.canal << 4) & 0x80) | (quint8)tmpNum;

    tmpNum = getValue();
    if (tmpNum == -1)
        return QByteArray();
    du_sample.value = (quint8)tmpNum;


    return QByteArray((char *)&(du_sample), MUSIC_SAMPLE_SIZE);
}

DuMidiChannelEventPtr DuEvent::toDuMidiChannelEvent(quint32 prevTime,
                                                    quint8 prevType) const
{
    DuMidiChannelEventPtr channelEvent(new DuMidiChannelEvent);
    int tmp = 0;


    tmp = getTime();

    if (tmp == -1)
        return DuMidiChannelEventPtr();

    channelEvent->setTime((quint32)tmp - prevTime, prevTime);


    tmp = getControl();

    if (tmp == -1)
        return DuMidiChannelEventPtr();

    channelEvent->setRunningStatus(((quint8)tmp + 0x08) == prevType);
    channelEvent->setType((quint8)tmp + 0x08);


    tmp = getNote();

    if (tmp == -1)
        return DuMidiChannelEventPtr();

    channelEvent->setKey((quint8)tmp);


    tmp = getValue();

    if (tmp == -1)
        return DuMidiChannelEventPtr();

    channelEvent->setValue((quint8)tmp);


    return channelEvent;
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
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EVENT_TIME);

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
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EVENT_CONTROL);

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
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EVENT_KEYBOARD);

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
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EVENT_NOTE);

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
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EVENT_VALUE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}
