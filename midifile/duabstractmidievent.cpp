#include "duabstractmidievent.h"

#include <QDebug>

DU_OBJECT_IMPL(DuAbstractMidiEvent)


DuAbstractMidiEvent::DuAbstractMidiEvent(quint32 time, quint8 status) :
    DuMidiContainer()
{
    addChild(KEY_MIDIEVENT_TIME, new DuMidiVariableLength(time));

    addChild(KEY_MIDIEVENT_STATUS,
             new DuMidiNumeric(status, MIDINUMERIC_DEFAULT_SIZE, 0xFF, 0x80));
}

DuAbstractMidiEvent::~DuAbstractMidiEvent()
{
}


quint32 DuAbstractMidiEvent::getTime() const
{
    const DuMidiVariableLengthConstPtr& tmp =
            getChildAs<DuMidiVariableLength>(KEY_MIDIEVENT_TIME);

    if (tmp == NULL)
        return -1;

    return tmp->getAbsolute();
}

void DuAbstractMidiEvent::setTime(quint32 delta, quint32 offset)
{
    DuMidiVariableLengthPtr& tmp =
            getChildAs<DuMidiVariableLength>(KEY_MIDIEVENT_TIME);

    if (tmp == NULL)
        return;

    tmp->setAbsolute(delta, offset);
}


quint8 DuAbstractMidiEvent::getStatus() const
{
    const DuMidiNumericConstPtr& tmp =
            getChildAs<DuMidiNumeric>(KEY_MIDIEVENT_STATUS);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

void DuAbstractMidiEvent::setStatus(quint8 value)
{
   DuMidiNumericPtr& tmp =
           getChildAs<DuMidiNumeric>(KEY_MIDIEVENT_STATUS);

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}
