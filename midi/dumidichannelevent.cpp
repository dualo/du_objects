#include "dumidichannelevent.h"

DuMidiChannelEvent::DuMidiChannelEvent() :
    DuAbstractMidiEvent(),
    key(0xFF),
    value(0xFF)
{
}

DuMidiChannelEvent::~DuMidiChannelEvent()
{
}


QByteArray DuMidiChannelEvent::toByteArray(bool runningStatusActive)
{
    QByteArray array =
            DuMidiVariableLength::getInstance()->formattedTimeArray(time);
    if (!runningStatusActive) array.append(status);

    switch (getType())
    {
    case NoteOff:
    case NoteOn:
    case KeyAftertouch:
    case ControlChange:
    case PitchWheelChange:
    {
        array.append(key);
        array.append(value);
        break;
    }
    case ProgramChange:
    case ChannelPressure:
    {
        array.append(value);
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
        stream >> key;
        stream >> value;
        break;
    }
    case ProgramChange:
    case ChannelPressure:
    {
        stream >> value;
        break;
    }
    default:
    {
    }
    }
}

void DuMidiChannelEvent::setDataBytes(QByteArray &array)
{
    switch (getType())
    {
    case NoteOff:
    case NoteOn:
    case KeyAftertouch:
    case ControlChange:
    case PitchWheelChange:
    {
        key = array[0];
        value = array[1];
        break;
    }
    case ProgramChange:
    case ChannelPressure:
    {
        value = array[0];
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
        size = DuMidiVariableLength::getInstance()->formattedSize(time) + 3;
        break;
    }
    case ProgramChange:
    case ChannelPressure:
    {
        size = DuMidiVariableLength::getInstance()->formattedSize(time) + 2;
        break;
    }
    default:
    {
    }
    }

    return size;
}


quint8 DuMidiChannelEvent::getType()
{
    return (status >> 4) & 0x0F;
}

quint8 DuMidiChannelEvent::getChannel()
{
    return status & 0x0F;
}

quint8 DuMidiChannelEvent::getKey()
{
    return key;
}

quint8 DuMidiChannelEvent::getValue()
{
    return value;
}


void DuMidiChannelEvent::setType(quint8 value)
{
    status = (status & 0x0F) | ((value << 4) & 0xF0);
}

void DuMidiChannelEvent::setChannel(quint8 value)
{
    status = (status & 0xF0) | (value & 0x0F);
}

void DuMidiChannelEvent::setKey(quint8 value)
{
    key = value;
}

void DuMidiChannelEvent::setValue(quint8 value)
{
    this->value = value;
}
