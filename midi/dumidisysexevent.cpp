#include "dumidisysexevent.h"

#include <QDebug>

DU_OBJECT_IMPL(DuMidiSysExEvent)


DuMidiSysExEvent::DuMidiSysExEvent() :
    DuAbstractMidiEvent()
{
    data = new DuMidiData();
}

DuMidiSysExEvent::~DuMidiSysExEvent()
{
    delete data;
}


QByteArray DuMidiSysExEvent::toByteArray(bool runningStatusActive)
{
    //TODO: refactoring
    QByteArray array;// = time->toMidiBinary();
    array.append(getStatus());

    //TODO: refactoring
    //array += conv->formattedLengthArray(getData().size());
    array += getData();

    return array;
}

void DuMidiSysExEvent::setDataBytes(QDataStream &stream)
{
    data->setData(stream);
}

void DuMidiSysExEvent::setDataBytes(const QByteArray &array)
{
    data->setData(array);
}


DuObjectPtr DuMidiSysExEvent::clone() const
{
    return DuMidiSysExEventPtr(new DuMidiSysExEvent(*this));
}


const QByteArray DuMidiSysExEvent::toMidiBinary() const
{
    //TODO: implement toMidiBinary()
    return QByteArray();
}


int DuMidiSysExEvent::size() const
{
    //TODO: refactoring
    //return (time->size() + 1 + length->size() + length->getAbsolute());
    return 0;
}


int DuMidiSysExEvent::getLength() const
{
    return length->getAbsolute();
}

void DuMidiSysExEvent::setLength(quint32 value)
{
    length->setAbsolute(value);
    data->resize(value);
}


const QByteArray DuMidiSysExEvent::getData() const
{
    return data->getData();
}

void DuMidiSysExEvent::setData(const QByteArray &value)
{
    data->setData(value);
}
