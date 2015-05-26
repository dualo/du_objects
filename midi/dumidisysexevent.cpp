#include "dumidisysexevent.h"

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
    QByteArray array = time->toMidiFile();
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

quint32 DuMidiSysExEvent::size() const
{
    quint32 length = getData().size();

    //TODO: refactoring
    //return (time->size() + 1 + conv->formattedSize(length) + length);

    return (time->size() + 1 + length);
}


const QByteArray DuMidiSysExEvent::getData() const
{
    return data->getData();
}

void DuMidiSysExEvent::setData(const QByteArray &value)
{
    data->setData(value);
}


void DuMidiSysExEvent::setLength(quint32 value)
{
    data->resize(value);
}
