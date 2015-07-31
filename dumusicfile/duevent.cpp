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

    verif = event->setTime(du_sample.time) ? verif : false;
    verif = event->setControl(du_sample.control) ? verif : false;
    verif = event->setKeyboard(du_sample.canal) ? verif : false;
    verif = event->setNote(du_sample.note & 0x7F) ? verif : false;
    verif = event->setValue(du_sample.value & 0x7F) ? verif : false;

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

    verif = event->setTime(jsonTime.toInt()) ? verif : false;
    verif = event->setControl(jsonCtrl.toInt()) ? verif : false;
    verif = event->setKeyboard(jsonKbrd.toInt()) ? verif : false;
    verif = event->setNote(jsonNote.toInt()) ? verif : false;
    verif = event->setValue(jsonVal.toInt()) ? verif : false;

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
    //This case should not occur since it is already tested in DuMusic::fromMidi();
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

    verif = event->setTime(channelEvent->getTime()) ? verif : false;
    verif = event->setControl(channelEvent->getType() - 0x08) ? verif : false;
    verif = event->setValue(channelEvent->getValue()) ? verif : false;

    int key = channelEvent->getKey();

    if (!helper.isPercu(loopIndex))
    {
        int side = helper.fetchKeyboard(key, loopIndex);
        verif = event->setKeyboard(0) ? verif : false;

        if (side != -1)
            verif = event->setNote(key + side) ? verif : false;
        else
            verif = event->setNote(key) ? verif : false;
    }
    else
    {
        int tmpKey = helper.fetchPercuKey(key, loopIndex);
        if (tmpKey == -1)
        {
            qCWarning(LOG_CAT_DU_OBJECT) << "DuEvent::fromMidi():\n"
                                         << "key not found in this mapping";

            return DuEventPtr();
        }

        verif = event->setKeyboard(0) ? verif : false;
        verif = event->setNote(tmpKey) ? verif : false;
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
                                                    quint8 prevType,
                                                    bool isPercu,
                                                    int instrKeyMap) const
{
    //This case should not occur since it is already tested in DuLoop::toDuMidiTrack()
    if (isPercu && instrKeyMap == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuEvent::toDuMidiChannelEvent():\n"
                << "invalid mapping for a percussive type instrument";

        return DuMidiChannelEventPtr();
    }

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


    quint8 midiType = (quint8)tmp + 0x08;
    int keyboard = getKeyboard();

    tmp = getNote();

    if (tmp == -1)
        return DuMidiChannelEventPtr();

    if (isPercu && midiType < DuMidiChannelEvent::KeyAftertouch)
    {
        if (keyboard == -1)
            return DuMidiChannelEventPtr();

        int midiKey =
                MidiConversionHelper::percuKey(tmp, (quint8)keyboard, instrKeyMap);

        if (midiKey == -1 || (quint8)midiKey > 0x7F)
            return DuMidiChannelEventPtr();

        channelEvent->setKey(midiKey);
    }
    else
    {
        channelEvent->setKey((quint8)tmp);
    }


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
