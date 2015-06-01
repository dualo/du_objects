#include "dumidichannelevent.h"

#include <QDebug>


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

    switch (getType())
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
