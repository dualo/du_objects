#include "dumidisysexevent.h"

DuMidiSysExEvent::DuMidiSysExEvent() :
    DuAbstractMidiEvent(),
    data(QByteArray())
{
    data.clear();
}

DuMidiSysExEvent::~DuMidiSysExEvent()
{
}


QByteArray DuMidiSysExEvent::toByteArray(bool runningStatusActive)
{
    DuMidiVariableLength *conv = DuMidiVariableLength::getInstance();

    QByteArray array = conv->formattedTimeArray(time);
    array.append(status);

    array += conv->formattedLengthArray(data.size());
    array += data;

    return array;
}

void DuMidiSysExEvent::setDataBytes(QDataStream &stream)
{
    stream.readRawData(data.data(), data.size());
}

void DuMidiSysExEvent::setDataBytes(QByteArray &array)
{
    data = array;
}

quint32 DuMidiSysExEvent::size()
{
    DuMidiVariableLength *conv = DuMidiVariableLength::getInstance();
    quint32 length = data.size();

    return (conv->formattedSize(time) + 1 + conv->formattedSize(length) + length);
}


void DuMidiSysExEvent::setLength(quint32 value)
{
    data.resize(value);
}
