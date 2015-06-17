#include "dumidichannelevent.h"



DU_OBJECT_IMPL(DuMidiChannelEvent)

DuMidiChannelEvent::DuMidiChannelEvent(quint32 time, quint8 status) :
    DuMidiBasicEvent(time, status)
{
    addChild(KEY_MIDICHANNELEVENT_KEY,
             new DuNumeric(0, NUMERIC_DEFAULT_SIZE, 0x7F, 0x00));

    addChild(KEY_MIDICHANNELEVENT_VALUE,
             new DuNumeric(0, NUMERIC_DEFAULT_SIZE, 0x7F, 0x00));
}

DuMidiChannelEvent::~DuMidiChannelEvent()
{
}


DuObjectPtr DuMidiChannelEvent::clone() const
{
    return DuMidiChannelEventPtr(new DuMidiChannelEvent(*this));
}


DuMidiChannelEventPtr DuMidiChannelEvent::fromMidiBinary(QDataStream &stream,
                                                         quint8 prevStatus,
                                                         quint8 byte)
{
    DuMidiChannelEventPtr channelEvent(new DuMidiChannelEvent);

    quint8 type = prevStatus / 16;
    //quint8 channel = prevStatus % 16;

    switch(type)
    {
    case DuMidiChannelEvent::NoteOff:
    case DuMidiChannelEvent::NoteOn:
    case DuMidiChannelEvent::KeyAftertouch:
    case DuMidiChannelEvent::ControlChange:
    case DuMidiChannelEvent::PitchWheelChange:
    {
        channelEvent->setKey(byte);

        quint8 tmp;
        stream >> tmp;
        channelEvent->setValue(tmp);

        break;
    }
    case DuMidiChannelEvent::ProgramChange:
    case DuMidiChannelEvent::ChannelPressure:
    {
        channelEvent->setValue(byte);
        break;
    }
    default:
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Invalid channel status";

        return DuMidiChannelEventPtr();
    }
    }

    channelEvent->setStatus(prevStatus);
    channelEvent->setRunningStatus(true);

    return channelEvent;
}

DuMidiChannelEventPtr DuMidiChannelEvent::fromMidiBinary(QDataStream &stream,
                                                         quint8 prevStatus)
{
    quint8 tmp;
    stream >> tmp;

    DuMidiChannelEventPtr channelEvent = fromMidiBinary(stream, prevStatus, tmp);
    channelEvent->setRunningStatus(false);

    return channelEvent;
}


QByteArray DuMidiChannelEvent::toMidiBinary() const
{
    QByteArray retArray;
    retArray.clear();

    const DuMidiVariableLengthConstPtr &time =
            getChildAs<DuMidiVariableLength>(KEY_MIDIEVENT_TIME);

    if (time == NULL)
        return QByteArray();

    retArray += time->toMidiBinary();


    const DuMidiStatusConstPtr &status =
            getChildAs<DuMidiStatus>(KEY_MIDIEVENT_STATUS);

    if (status == NULL)
        return QByteArray();

    retArray += status->toMidiBinary();


    quint8 type = getType();
    switch(type)
    {
    case DuMidiChannelEvent::NoteOff:
    case DuMidiChannelEvent::NoteOn:
    case DuMidiChannelEvent::KeyAftertouch:
    case DuMidiChannelEvent::ControlChange:
    case DuMidiChannelEvent::PitchWheelChange:
    {
        const DuNumericConstPtr &key =
                getChildAs<DuNumeric>(KEY_MIDICHANNELEVENT_KEY);

        if (key == NULL)
            return QByteArray();

        retArray += key->toMidiBinary();

        const DuNumericConstPtr &value =
                getChildAs<DuNumeric>(KEY_MIDICHANNELEVENT_VALUE);

        if (value == NULL)
            return QByteArray();

        retArray += value->toMidiBinary();

        break;
    }
    case DuMidiChannelEvent::ProgramChange:
    case DuMidiChannelEvent::ChannelPressure:
    {
        const DuNumericConstPtr &value =
                getChildAs<DuNumeric>(KEY_MIDICHANNELEVENT_VALUE);

        if (value == NULL)
            return QByteArray();

        retArray += value->toMidiBinary();
        break;
    }
    default:
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Invalid channel status";

        return QByteArray();
    }
    }

    return retArray;
}


int DuMidiChannelEvent::size() const
{
    int size = 0;

    const DuMidiVariableLengthConstPtr &time =
            getChildAs<DuMidiVariableLength>(KEY_MIDIEVENT_TIME);

    if (time == NULL)
        return -1;

    size += time->getAbsolute() + 1;


    quint8 type = getType();
    switch (type)
    {
    case NoteOff:
    case NoteOn:
    case KeyAftertouch:
    case ControlChange:
    case PitchWheelChange:
    {
        size += 2;
        break;
    }
    case ProgramChange:
    case ChannelPressure:
    {
        size += 1;
        break;
    }
    default:
    {
        return -1;
    }
    }

    return size;
}


quint8 DuMidiChannelEvent::getType() const
{
    return (getStatus() >> 4) & 0x0F;
}

void DuMidiChannelEvent::setType(quint8 value)
{
    setStatus((getStatus() & 0x0F) | ((value << 4) & 0xF0));
}


quint8 DuMidiChannelEvent::getChannel() const
{
    return getStatus() & 0x0F;
}

void DuMidiChannelEvent::setChannel(quint8 value)
{
    setStatus((getStatus() & 0xF0) | (value & 0x0F));
}


quint8 DuMidiChannelEvent::getKey() const
{
    const DuNumericConstPtr &tmp =
            getChildAs<DuNumeric>(KEY_MIDICHANNELEVENT_KEY);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

void DuMidiChannelEvent::setKey(quint8 value)
{
    const DuNumericPtr &tmp =
            getChildAs<DuNumeric>(KEY_MIDICHANNELEVENT_KEY);

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


quint8 DuMidiChannelEvent::getValue() const
{
    const DuNumericConstPtr &tmp =
            getChildAs<DuNumeric>(KEY_MIDICHANNELEVENT_VALUE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

void DuMidiChannelEvent::setValue(quint8 value)
{
    const DuNumericPtr &tmp =
            getChildAs<DuNumeric>(KEY_MIDICHANNELEVENT_VALUE);

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}
