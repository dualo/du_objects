#include "dumidimetaevent.h"

#include <QDebug>

DU_OBJECT_IMPL(DuMidiMetaEvent)


DuMidiMetaEvent::DuMidiMetaEvent() :
    DuAbstractMidiEvent(0, 0xFF)
{
    type = new DuMidiNumeric(0x00, MIDINUMERIC_DEFAULT_SIZE, 0x7F, 0x00);
    data = new DuMidiData();
}

DuMidiMetaEvent::~DuMidiMetaEvent()
{
    delete type;
    delete data;
}


QByteArray DuMidiMetaEvent::toByteArray(bool runningStatusActive)
{
    //TODO: refactoring
    QByteArray array ;//= time->toMidiBinary();
    array.append(getStatus());

    array.append(getType());

    array += length->toMidiBinary();
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


DuObjectPtr DuMidiMetaEvent::clone() const
{
    return DuMidiMetaEventPtr(new DuMidiMetaEvent(*this));
}


const QByteArray DuMidiMetaEvent::toMidiBinary() const
{
    //TODO: implement toMidiBinary()
    return QByteArray();
}


int DuMidiMetaEvent::size() const
{
    //TODO: refactoring
    return (1 + 2 + length->size() + getLength());
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
    return length->getAbsolute();
}

void DuMidiMetaEvent::setLength(quint32 value)
{
    length->setAbsolute(value);
    data->resize(length->getAbsolute());
}


const QByteArray DuMidiMetaEvent::getData() const
{
    return data->getData();
}

void DuMidiMetaEvent::setData(const QByteArray &value)
{
    data->setData(value);
}
