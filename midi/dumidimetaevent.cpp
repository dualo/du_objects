#include "dumidimetaevent.h"

DuMidiMetaEvent::DuMidiMetaEvent() :
    DuAbstractMidiEvent(0, 0xFF)
{
    type = new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0x7F, 0x00);
    data = new DuMidiData();
}

DuMidiMetaEvent::~DuMidiMetaEvent()
{
    delete type;
    delete data;
}


QByteArray DuMidiMetaEvent::toByteArray(bool runningStatusActive)
{
    DuMidiVariableLength *conv = DuMidiVariableLength::getInstance();

    QByteArray array = conv->formattedTimeArray(getTime());
    array.append(getStatus());

    array.append(getType());
    array += conv->formattedLengthArray(getData().size());
    array += getData();

    return array;
}

void DuMidiMetaEvent::setDataBytes(QDataStream &stream)
{
    data->setData(stream);
}

void DuMidiMetaEvent::setDataBytes(const QByteArray &array)
{
   data->setData(array);
}


quint32 DuMidiMetaEvent::size()
{
    DuMidiVariableLength *conv = DuMidiVariableLength::getInstance();
    quint32 length = getData().size();

    return (conv->formattedSize(getTime()) + 2
            + conv->formattedSize(getData().size()) + length);
}


quint8 DuMidiMetaEvent::getType() const
{
    return type->getNumeric();
}

void DuMidiMetaEvent::setType(quint8 value)
{
    type->setNumeric(value);
}


quint32 DuMidiMetaEvent::getLength() const
{
    return getData().size();
}

void DuMidiMetaEvent::setLength(quint32 value)
{
    data->setMaxSize(value);
}


const QByteArray DuMidiMetaEvent::getData() const
{
    return data->getConstData();
}