#include "duabstractmidievent.h"

DuAbstractMidiEvent::DuAbstractMidiEvent() :
    time(0x00000000),
    status(0x00)
{
}

DuAbstractMidiEvent::~DuAbstractMidiEvent()
{
}


quint32 DuAbstractMidiEvent::getTime()
{
    return time;
}

quint8 DuAbstractMidiEvent::getStatus()
{
    return status;
}

void DuAbstractMidiEvent::setTime(quint32 value)
{
    time = value;
}

void DuAbstractMidiEvent::setStatus(quint8 value)
{
    status = value;
}
