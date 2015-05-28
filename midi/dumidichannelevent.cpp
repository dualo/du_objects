#include "dumidichannelevent.h"

#include <QDebug>

DU_OBJECT_IMPL(DuMidiChannelEvent)


DuMidiChannelEvent::DuMidiChannelEvent(quint32 time, quint8 status) :
    DuAbstractMidiEvent(time, status)
{
    addChild(KEY_MIDICHANNELEVENT_KEY,
             new DuMidiNumeric(0, MIDINUMERIC_DEFAULT_SIZE, 0x7F, 0x00));

    addChild(KEY_MIDICHANNELEVENT_VALUE,
             new DuMidiNumeric(0, MIDINUMERIC_DEFAULT_SIZE, 0x7F, 0x00));
}

DuMidiChannelEvent::~DuMidiChannelEvent()
{
}


QByteArray DuMidiChannelEvent::toByteArray(bool runningStatusActive)
{
    //TODO: subclasses for DuMidiChannelEvent to differenciate virtual method

    //TODO: refactoring
    QByteArray array;// = time->toMidiBinary();
    if (!runningStatusActive) array.append(getStatus());

    switch (getType())
    {
    case NoteOff:
    case NoteOn:
    case KeyAftertouch:
    case ControlChange:
    case PitchWheelChange:
    {
        array.append(getKey());
        array.append(getValue());
        break;
    }
    case ProgramChange:
    case ChannelPressure:
    {
        array.append(getValue());
        break;
    }
    default:
    {
    }
    }

    return array;
}


void DuMidiChannelEvent::setDataBytes(QDataStream &stream)
{
    //TODO: subclasses for DuMidiChannelEvent to differenciate virtual method

    switch (getType())
    {
    case NoteOff:
    case NoteOn:
    case KeyAftertouch:
    case ControlChange:
    case PitchWheelChange:
    {
        quint8 tmp;

        stream >> tmp;
        setKey(tmp);

        stream >> tmp;
        setValue(tmp);

        break;
    }
    case ProgramChange:
    case ChannelPressure:
    {
        quint8 tmp;

        stream >> tmp;
        setValue(tmp);

        break;
    }
    default:
    {
    }
    }
}

void DuMidiChannelEvent::setDataBytes(const QByteArray &array)
{
    //TODO: subclasses for DuMidiChannelEvent to differenciate virtual method

    switch (getType())
    {
    case NoteOff:
    case NoteOn:
    case KeyAftertouch:
    case ControlChange:
    case PitchWheelChange:
    {
        setKey(array[0]);
        setValue(array[1]);
        break;
    }
    case ProgramChange:
    case ChannelPressure:
    {
        setValue(array[0]);
        break;
    }
    default:
    {
    }
    }
}


DuObjectPtr DuMidiChannelEvent::clone() const
{
    return DuMidiChannelEventPtr(new DuMidiChannelEvent(*this));
}


const QByteArray DuMidiChannelEvent::toMidiBinary() const
{
    //TODO: subclasses for DuMidiChannelEvent to differenciate virtual method

    QByteArray retArray;
    retArray.clear();

    const DuMidiVariableLengthConstPtr &time =
            getChildAs<DuMidiVariableLength>(KEY_MIDIEVENT_TIME);

    if (time == NULL)
        return QByteArray();

    retArray += time->toMidiBinary();


    const DuMidiNumericConstPtr &status =
            getChildAs<DuMidiNumeric>(KEY_MIDIEVENT_STATUS);

    if (status == NULL)
        return QByteArray();

    retArray += status->toMidiBinary();


    const DuMidiNumericConstPtr &key =
            getChildAs<DuMidiNumeric>(KEY_MIDICHANNELEVENT_KEY);

    if (key == NULL)
        return QByteArray();

    retArray += key->toMidiBinary();


    const DuMidiNumericConstPtr &value =
            getChildAs<DuMidiNumeric>(KEY_MIDICHANNELEVENT_VALUE);

    if (value == NULL)
        return QByteArray();

    retArray += value->toMidiBinary();

    return retArray;
}


int DuMidiChannelEvent::size() const
{
    //TODO: subclasses for DuMidiChannelEvent to differenciate virtual method

    quint32 size;

    switch (getType())
    {
    case NoteOff:
    case NoteOn:
    case KeyAftertouch:
    case ControlChange:
    case PitchWheelChange:
    {
        //TODO: refactoring
        //size = time->size() + 3;
        break;
    }
    case ProgramChange:
    case ChannelPressure:
    {
        //TODO: refactoring
        //size = time->size() + 2;
        break;
    }
    default:
    {
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
    const DuMidiNumericConstPtr &tmp =
            getChildAs<DuMidiNumeric>(KEY_MIDICHANNELEVENT_KEY);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

void DuMidiChannelEvent::setKey(quint8 value)
{
    DuMidiNumericPtr &tmp =
            getChildAs<DuMidiNumeric>(KEY_MIDICHANNELEVENT_KEY);

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


quint8 DuMidiChannelEvent::getValue() const
{
    const DuMidiNumericConstPtr &tmp =
            getChildAs<DuMidiNumeric>(KEY_MIDICHANNELEVENT_VALUE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

void DuMidiChannelEvent::setValue(quint8 value)
{
    DuMidiNumericPtr &tmp =
            getChildAs<DuMidiNumeric>(KEY_MIDICHANNELEVENT_VALUE);

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}
