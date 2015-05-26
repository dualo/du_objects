#include "duabstractmidievent.h"

DuAbstractMidiEvent::DuAbstractMidiEvent(quint32 time, quint8 status)
{
    this->time = new DuMidiVariableLength(time);
    this->status = new DuNumeric(status, NUMERIC_DEFAULT_SIZE, 0xFF, 0x80);
}

DuAbstractMidiEvent::~DuAbstractMidiEvent()
{
    delete time;
    delete status;
}


quint32 DuAbstractMidiEvent::getTime() const
{
    return time->getAbsolute();
}

quint8 DuAbstractMidiEvent::getStatus() const
{
    return status->getNumeric();
}

void DuAbstractMidiEvent::setTime(quint32 offset, quint32 delta)
{
    time->setOffset(offset);
    time->setDelta(delta);
}

void DuAbstractMidiEvent::setStatus(quint8 value)
{
    status->setNumeric(value);
}
