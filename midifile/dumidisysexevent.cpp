#include "dumidisysexevent.h"

#include <QDebug>


DU_OBJECT_IMPL(DuMidiSysExEvent)

DuMidiSysExEvent::DuMidiSysExEvent() :
    DuMidiAbstractEvent()
{
    addChild(KEY_MIDISYSEXEVENT_LENGTH, new DuMidiVariableLength(0));

    addChild(KEY_MIDISYSEXEVENT_DATA,   new DuBinaryData());
}

DuMidiSysExEvent::~DuMidiSysExEvent()
{
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
    setData(stream);
}

void DuMidiSysExEvent::setDataBytes(const QByteArray &array)
{
    setData(array);
}


DuObjectPtr DuMidiSysExEvent::clone() const
{
    return DuMidiSysExEventPtr(new DuMidiSysExEvent(*this));
}


QByteArray DuMidiSysExEvent::toMidiBinary() const
{
    //TODO: implement toMidiBinary()
    return QByteArray();
}


int DuMidiSysExEvent::getLength() const
{
    const DuMidiVariableLengthConstPtr &tmp =
            getChildAs<DuMidiVariableLength>(KEY_MIDISYSEXEVENT_LENGTH);

    if (tmp == NULL)
        return -1;

    return tmp->getAbsolute();
}

void DuMidiSysExEvent::setLength(quint32 value)
{
    DuMidiVariableLengthPtr &length =
            getChildAs<DuMidiVariableLength>(KEY_MIDISYSEXEVENT_LENGTH);
    if (length == NULL)
        return;

    DuBinaryDataPtr &data =
            getChildAs<DuBinaryData>(KEY_MIDISYSEXEVENT_DATA);
    if (data == NULL)
        return;

    length->setAbsolute(value);
    data->resize(value);
}


const QByteArray DuMidiSysExEvent::getData() const
{
    const DuBinaryDataConstPtr &tmp =
            getChildAs<DuBinaryData>(KEY_MIDISYSEXEVENT_DATA);

    if (tmp == NULL)
        return QByteArray();

    return tmp->getData();
}

void DuMidiSysExEvent::setData(const QByteArray &value)
{
    DuBinaryDataPtr &tmp =
            getChildAs<DuBinaryData>(KEY_MIDISYSEXEVENT_DATA);

    if (tmp == NULL)
        return;

    tmp->setData(value);
}

void DuMidiSysExEvent::setData(QDataStream &stream)
{
    DuBinaryDataPtr &tmp =
            getChildAs<DuBinaryData>(KEY_MIDISYSEXEVENT_DATA);

    if (tmp == NULL)
        return;

    tmp->setData(stream);
}
