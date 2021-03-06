#include "dumidisysexevent.h"

#include "dumidistatus.h"
#include "dumidivariablelength.h"

#include "../general/dubinarydata.h"



DU_OBJECT_IMPL(DuMidiSysExEvent)

DuMidiSysExEvent::DuMidiSysExEvent(quint32 time, quint8 status) :
    DuMidiBasicEvent(time, status)
{
    addChild(KEY_MIDISYSEXEVENT_LENGTH, new DuMidiVariableLength(0));

    addChild(KEY_MIDISYSEXEVENT_DATA,   new DuBinaryData());
}


DuObjectPtr DuMidiSysExEvent::clone() const
{
    return DuMidiSysExEventPtr(new DuMidiSysExEvent(*this));
}


DuMidiSysExEventPtr DuMidiSysExEvent::fromMidiBinary(QDataStream &stream, quint8 status)
{
    DuMidiSysExEventPtr sysExEvent(new DuMidiSysExEvent);

    sysExEvent->setStatus(status);
    sysExEvent->setLength(stream);
    sysExEvent->setData(stream);

    return sysExEvent;
}


QByteArray DuMidiSysExEvent::toMidiBinary() const
{
    QByteArray retArray;
    retArray.clear();

    const DuMidiVariableLengthConstPtr &time =
            getChildAs<DuMidiVariableLength>(KEY_MIDIEVENT_TIME);

    if (time == Q_NULLPTR)
        return QByteArray();

    retArray += time->toMidiBinary();


    const DuMidiStatusConstPtr &status =
            getChildAs<DuMidiStatus>(KEY_MIDIEVENT_STATUS);

    if (status == Q_NULLPTR)
        return QByteArray();

    retArray += status->toMidiBinary();


    const DuMidiVariableLengthConstPtr &length =
            getChildAs<DuMidiVariableLength>(KEY_MIDISYSEXEVENT_LENGTH);

    if (length == Q_NULLPTR)
        return QByteArray();

    retArray += length->toMidiBinary();


    const DuBinaryDataConstPtr &data =
            getChildAs<DuBinaryData>(KEY_MIDISYSEXEVENT_DATA);

    if (data == Q_NULLPTR)
        return QByteArray();

    retArray += data->toMidiBinary();


    return retArray;
}


int DuMidiSysExEvent::getLength() const
{
    const DuMidiVariableLengthConstPtr &tmp =
            getChildAs<DuMidiVariableLength>(KEY_MIDISYSEXEVENT_LENGTH);

    if (tmp == Q_NULLPTR)
        return -1;

    return static_cast<int>(tmp->getAbsolute());
}

void DuMidiSysExEvent::setLength(quint32 value)
{
    const DuMidiVariableLengthPtr &length =
            getChildAs<DuMidiVariableLength>(KEY_MIDISYSEXEVENT_LENGTH);
    if (length == Q_NULLPTR)
        return;

    const DuBinaryDataPtr &data =
            getChildAs<DuBinaryData>(KEY_MIDISYSEXEVENT_DATA);
    if (data == Q_NULLPTR)
        return;

    length->setAbsolute(value);
    data->resize(static_cast<int>(value));
}

void DuMidiSysExEvent::setLength(QDataStream &stream)
{
    const DuMidiVariableLengthPtr &length =
            getChildAs<DuMidiVariableLength>(KEY_MIDISYSEXEVENT_LENGTH);
    if (length == Q_NULLPTR)
        return;

    const DuBinaryDataPtr &data =
            getChildAs<DuBinaryData>(KEY_MIDISYSEXEVENT_DATA);
    if (data == Q_NULLPTR)
        return;

    length->setAbsolute(stream);
    data->resize(static_cast<int>(length->getAbsolute()));
}


const QByteArray DuMidiSysExEvent::getData() const
{
    const DuBinaryDataConstPtr &tmp =
            getChildAs<DuBinaryData>(KEY_MIDISYSEXEVENT_DATA);

    if (tmp == Q_NULLPTR)
        return QByteArray();

    return tmp->getBinaryData();
}

void DuMidiSysExEvent::setData(const QByteArray &value)
{
    const DuMidiVariableLengthPtr &length =
            getChildAs<DuMidiVariableLength>(KEY_MIDISYSEXEVENT_LENGTH);
    if (length == Q_NULLPTR)
        return;

    const DuBinaryDataPtr &data =
            getChildAs<DuBinaryData>(KEY_MIDISYSEXEVENT_DATA);
    if (data == Q_NULLPTR)
        return;

    length->setAbsolute(static_cast<quint32>(value.size()));
    data->setBinaryData(value);
}

void DuMidiSysExEvent::setData(QDataStream &stream)
{
    const DuBinaryDataPtr &tmp =
            getChildAs<DuBinaryData>(KEY_MIDISYSEXEVENT_DATA);

    if (tmp == Q_NULLPTR)
        return;

    tmp->setData(stream);
}
