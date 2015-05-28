#include "dumidiabstractevent.h"

#include <QDebug>


DU_OBJECT_IMPL(DuMidiAbstractEvent)

DuMidiAbstractEvent::DuMidiAbstractEvent(quint32 time, quint8 status) :
    DuContainer()
{
    addChild(KEY_MIDIEVENT_TIME, new DuMidiVariableLength(time));

    addChild(KEY_MIDIEVENT_STATUS,
             new DuNumeric(status, NUMERIC_DEFAULT_SIZE, 0xFF, 0x80));
}

DuMidiAbstractEvent::~DuMidiAbstractEvent()
{
}


QByteArray DuMidiAbstractEvent::toDuMusicBinary() const
{
    Q_UNIMPLEMENTED();
    return QByteArray();
}

QJsonValue DuMidiAbstractEvent::toJson() const
{
    Q_UNIMPLEMENTED();
    return QJsonValue();
}


quint32 DuMidiAbstractEvent::getTime() const
{
    const DuMidiVariableLengthConstPtr &tmp =
            getChildAs<DuMidiVariableLength>(KEY_MIDIEVENT_TIME);

    if (tmp == NULL)
        return -1;

    return tmp->getAbsolute();
}

void DuMidiAbstractEvent::setTime(quint32 delta, quint32 offset)
{
    DuMidiVariableLengthPtr &tmp =
            getChildAs<DuMidiVariableLength>(KEY_MIDIEVENT_TIME);

    if (tmp == NULL)
        return;

    tmp->setAbsolute(delta, offset);
}


quint8 DuMidiAbstractEvent::getStatus() const
{
    const DuNumericConstPtr &tmp =
            getChildAs<DuNumeric>(KEY_MIDIEVENT_STATUS);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

void DuMidiAbstractEvent::setStatus(quint8 value)
{
   DuNumericPtr &tmp =
           getChildAs<DuNumeric>(KEY_MIDIEVENT_STATUS);

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}
