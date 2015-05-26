#include "dumidichannelevent.h"

DuMidiChannelEvent::DuMidiChannelEvent() :
    DuAbstractMidiEvent()
{
    key = new DuNumeric(0, NUMERIC_DEFAULT_SIZE, 0x00, 0x7F);
    value = new DuNumeric(0, NUMERIC_DEFAULT_SIZE, 0x00, 0x7F);
}

DuMidiChannelEvent::~DuMidiChannelEvent()
{
    delete key;
    delete value;
}


QByteArray DuMidiChannelEvent::toByteArray(bool runningStatusActive)
{
    QByteArray array =
            DuMidiVariableLength::getInstance()->formattedTimeArray(getTime());
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


quint32 DuMidiChannelEvent::size()
{
    quint32 size;

    switch (getType())
    {
    case NoteOff:
    case NoteOn:
    case KeyAftertouch:
    case ControlChange:
    case PitchWheelChange:
    {
        size = DuMidiVariableLength::getInstance()->formattedSize(getTime()) + 3;
        break;
    }
    case ProgramChange:
    case ChannelPressure:
    {
        size = DuMidiVariableLength::getInstance()->formattedSize(getTime()) + 2;
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

quint8 DuMidiChannelEvent::getChannel() const
{
    return getStatus() & 0x0F;
}

quint8 DuMidiChannelEvent::getKey() const
{
    return key->getNumeric();
}

quint8 DuMidiChannelEvent::getValue() const
{
    return value->getNumeric();
}


void DuMidiChannelEvent::setType(quint8 value)
{
    setStatus((getStatus() & 0x0F) | ((value << 4) & 0xF0));
}

void DuMidiChannelEvent::setChannel(quint8 value)
{
    setStatus((getStatus() & 0xF0) | (value & 0x0F));
}

void DuMidiChannelEvent::setKey(quint8 value)
{
    key->setNumeric(value);
}

void DuMidiChannelEvent::setValue(quint8 value)
{
    this->value->setValue(value);
}
