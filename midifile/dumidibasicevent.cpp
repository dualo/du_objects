#include "dumidibasicevent.h"

#include "dumidistatus.h"
#include "dumidivariablelength.h"

#include <QJsonValue>



DU_OBJECT_IMPL(DuMidiBasicEvent)

DuMidiBasicEvent::DuMidiBasicEvent(quint32 time, quint8 status) :
    DuContainer()
{
    addChild(KEY_MIDIEVENT_TIME, new DuMidiVariableLength(static_cast<int>(time)));

    addChild(KEY_MIDIEVENT_STATUS, new DuMidiStatus(status));
}


DuObjectPtr DuMidiBasicEvent::clone() const
{
    return DuMidiBasicEventPtr(new DuMidiBasicEvent(*this));
}


QByteArray DuMidiBasicEvent::toDuMusicBinary() const
{
    Q_UNIMPLEMENTED();
    return QByteArray();
}

QByteArray DuMidiBasicEvent::toMidiBinary() const
{
    Q_UNIMPLEMENTED();
    return QByteArray();
}

QJsonValue DuMidiBasicEvent::toJson() const
{
    Q_UNIMPLEMENTED();
    return QJsonValue();
}


int DuMidiBasicEvent::getTime() const
{
    const DuMidiVariableLengthConstPtr &tmp =
            getChildAs<DuMidiVariableLength>(KEY_MIDIEVENT_TIME);

    if (tmp == NULL)
        return -1;

    return static_cast<int>(tmp->getAbsolute());
}

void DuMidiBasicEvent::setTime(quint32 delta, quint32 offset)
{
    const DuMidiVariableLengthPtr &tmp =
            getChildAs<DuMidiVariableLength>(KEY_MIDIEVENT_TIME);

    if (tmp == NULL)
        return;

    tmp->setAbsolute(delta, offset);
}

void DuMidiBasicEvent::setTime(QDataStream &stream, quint32 offset)
{
    const DuMidiVariableLengthPtr &tmp =
            getChildAs<DuMidiVariableLength>(KEY_MIDIEVENT_TIME);

    if (tmp == NULL)
        return;

    tmp->setAbsolute(stream, offset);
}


int DuMidiBasicEvent::getStatus() const
{
    const DuMidiStatusConstPtr &tmp =
            getChildAs<DuMidiStatus>(KEY_MIDIEVENT_STATUS);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

void DuMidiBasicEvent::setStatus(quint8 value)
{
   const DuMidiStatusPtr &tmp =
           getChildAs<DuMidiStatus>(KEY_MIDIEVENT_STATUS);

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


bool DuMidiBasicEvent::runningStatus() const
{
    const DuMidiStatusConstPtr &tmp =
            getChildAs<DuMidiStatus>(KEY_MIDIEVENT_STATUS);

    if (tmp == NULL)
        return false;

    return tmp->getRunningStatus();
}

void DuMidiBasicEvent::setRunningStatus(bool value)
{
    const DuMidiStatusPtr &tmp =
            getChildAs<DuMidiStatus>(KEY_MIDIEVENT_STATUS);

     if (tmp == NULL)
         return;

     tmp->setRunningStatus(value);
}
