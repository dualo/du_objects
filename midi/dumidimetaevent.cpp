#include "dumidimetaevent.h"

DuMidiMetaEvent::DuMidiMetaEvent() :
    DuAbstractMidiEvent(),
    type(0x00),
    data(QByteArray())
{
    status = 0xFF;
    data.clear();
}

DuMidiMetaEvent::~DuMidiMetaEvent()
{
}


QByteArray DuMidiMetaEvent::toByteArray(bool runningStatusActive)
{
    DuMidiVariableLength *conv = DuMidiVariableLength::getInstance();

    QByteArray array = conv->formattedTimeArray(time);
    array.append(status);

    array.append(type);
    array += conv->formattedLengthArray(data.size());
    array += data;

    return array;
}

void DuMidiMetaEvent::setDataBytes(QDataStream &stream)
{
    stream.readRawData(data.data(), data.size());
}

void DuMidiMetaEvent::setDataBytes(QByteArray &array)
{
   data = array;
}


quint32 DuMidiMetaEvent::size()
{
    DuMidiVariableLength *conv = DuMidiVariableLength::getInstance();
    quint32 length = data.size();

    return (conv->formattedSize(time) + 2 + conv->formattedSize(data.size()) + length);
}


quint8 DuMidiMetaEvent::getType()
{
    return type;
}

quint32 DuMidiMetaEvent::getLength()
{
    return data.size();
}

QByteArray& DuMidiMetaEvent::getData()
{
    return data;
}


void DuMidiMetaEvent::setType(quint8 value)
{
    type = value;
}

void DuMidiMetaEvent::setLength(quint32 value)
{
    data.resize(value);
}
