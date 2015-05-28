#include "dumidimetaevent.h"

#include <QDebug>


DU_OBJECT_IMPL(DuMidiMetaEvent)

DuMidiMetaEvent::DuMidiMetaEvent() :
    DuMidiAbstractEvent(0, 0xFF)
{
    addChild(KEY_MIDIMETAEVENT_TYPE,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));

    addChild(KEY_MIDIMETAEVENT_LENGTH,  new DuMidiVariableLength(0));

    addChild(KEY_MIDIMETAEVENT_DATA,    new DuBinaryData());
}

DuMidiMetaEvent::~DuMidiMetaEvent()
{
}


QByteArray DuMidiMetaEvent::toByteArray(bool runningStatusActive)
{
    //TODO: refactoring
    QByteArray array ;//= time->toMidiBinary();
    array.append(getStatus());

    array.append(getType());

    //array += length->toMidiBinary();
    array += getData();

    return array;
}

void DuMidiMetaEvent::setDataBytes(QDataStream &stream)
{
    setData(stream);
}

void DuMidiMetaEvent::setDataBytes(const QByteArray &array)
{
    setData(array);
}


DuObjectPtr DuMidiMetaEvent::clone() const
{
    return DuMidiMetaEventPtr(new DuMidiMetaEvent(*this));
}


QByteArray DuMidiMetaEvent::toMidiBinary() const
{
    //TODO: implement toMidiBinary()
    return QByteArray();
}


quint8 DuMidiMetaEvent::getType() const
{
    const DuNumericConstPtr &tmp =
            getChildAs<DuNumeric>(KEY_MIDIMETAEVENT_TYPE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

void DuMidiMetaEvent::setType(quint8 value)
{
    DuNumericPtr &tmp =
            getChildAs<DuNumeric>(KEY_MIDIMETAEVENT_TYPE);

     if (tmp == NULL)
         return;

     tmp->setNumeric(value);
}


quint32 DuMidiMetaEvent::getLength() const
{
    const DuMidiVariableLengthConstPtr &tmp =
            getChildAs<DuMidiVariableLength>(KEY_MIDIMETAEVENT_LENGTH);

    if (tmp == NULL)
        return -1;

    return tmp->getAbsolute();
}

void DuMidiMetaEvent::setLength(quint32 value)
{
    DuMidiVariableLengthPtr &length =
            getChildAs<DuMidiVariableLength>(KEY_MIDIMETAEVENT_LENGTH);
    if (length == NULL)
        return;

    DuBinaryDataPtr &data =
            getChildAs<DuBinaryData>(KEY_MIDIMETAEVENT_DATA);
    if (data == NULL)
        return;

    length->setAbsolute(value);
    data->resize(value);
}


const QByteArray DuMidiMetaEvent::getData() const
{
    const DuBinaryDataConstPtr &tmp =
            getChildAs<DuBinaryData>(KEY_MIDIMETAEVENT_DATA);

    if (tmp == NULL)
        return QByteArray();

    return tmp->getData();
}

void DuMidiMetaEvent::setData(const QByteArray &value)
{
    DuBinaryDataPtr &tmp =
            getChildAs<DuBinaryData>(KEY_MIDIMETAEVENT_DATA);

    if (tmp == NULL)
        return;

    tmp->setData(value);
}

void DuMidiMetaEvent::setData(QDataStream &stream)
{
    DuBinaryDataPtr &tmp =
            getChildAs<DuBinaryData>(KEY_MIDIMETAEVENT_DATA);

    if (tmp == NULL)
        return;

    tmp->setData(stream);
}
