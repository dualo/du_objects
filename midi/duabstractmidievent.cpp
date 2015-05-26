#include "duabstractmidievent.h"

DuAbstractMidiEvent::DuAbstractMidiEvent(quint32 time, quint8 status)
{
    this->time = new DuNumeric(time);
    this->status = new DuNumeric(status, NUMERIC_DEFAULT_SIZE, 0xFF, 0x80);
}

DuAbstractMidiEvent::~DuAbstractMidiEvent()
{
    delete time;
    delete status;
}


quint32 DuAbstractMidiEvent::getTime() const
{
    return time->getNumeric();
}

quint8 DuAbstractMidiEvent::getStatus() const
{
    return status->getNumeric();
}

void DuAbstractMidiEvent::setTime(quint32 value)
{
    time->setNumeric(value);
}

void DuAbstractMidiEvent::setStatus(quint8 value)
{
    status->setNumeric(value);
}
