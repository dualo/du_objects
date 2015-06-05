#include "dumidimetaevent.h"

#include <QDebug>


DU_OBJECT_IMPL(DuMidiMetaEvent)

DuMidiMetaEvent::DuMidiMetaEvent(quint32 time) :
    DuMidiBasicEvent(time, 0xFF)
{
    addChild(KEY_MIDIMETAEVENT_TYPE,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0x7F, 0x00));

    addChild(KEY_MIDIMETAEVENT_LENGTH,  new DuMidiVariableLength(0));

    addChild(KEY_MIDIMETAEVENT_DATA,    new DuBinaryData());
}

DuMidiMetaEvent::~DuMidiMetaEvent()
{
}


DuObjectPtr DuMidiMetaEvent::clone() const
{
    return DuMidiMetaEventPtr(new DuMidiMetaEvent(*this));
}

DuMidiMetaEventPtr DuMidiMetaEvent::fromMidiBinary(QDataStream &stream,
                                                   bool *trackEnded)
{
    DuMidiMetaEventPtr metaEvent(new DuMidiMetaEvent);

    quint8 type;

    stream >> type;

    metaEvent->setType(type);
    metaEvent->setLength(stream);
    metaEvent->setData(stream);

    *trackEnded = (type == EndOfTrack);

    return metaEvent;
}


QByteArray DuMidiMetaEvent::toMidiBinary() const
{
    QByteArray retArray;
    retArray.clear();

    const DuMidiVariableLengthConstPtr &time =
            getChildAs<DuMidiVariableLength>(KEY_MIDIEVENT_TIME);

    if (time == NULL)
        return QByteArray();

    retArray += time->toMidiBinary();


    const DuMidiStatusConstPtr &status =
            getChildAs<DuMidiStatus>(KEY_MIDIEVENT_STATUS);

    if (status == NULL)
        return QByteArray();

    retArray += status->toMidiBinary();


    const DuNumericConstPtr &type =
            getChildAs<DuNumeric>(KEY_MIDIMETAEVENT_TYPE);

    if (type == NULL)
        return QByteArray();

    retArray += type->toMidiBinary();


    const DuMidiVariableLengthConstPtr &length =
            getChildAs<DuMidiVariableLength>(KEY_MIDIMETAEVENT_LENGTH);

    if (length == NULL)
        return QByteArray();

    retArray += length->toMidiBinary();


    const DuBinaryDataConstPtr &data =
            getChildAs<DuBinaryData>(KEY_MIDIMETAEVENT_DATA);

    if (data == NULL)
        return QByteArray();

    retArray += data->toMidiBinary();


    return retArray;
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
    const DuNumericPtr &tmp =
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
    const DuMidiVariableLengthPtr &length =
            getChildAs<DuMidiVariableLength>(KEY_MIDIMETAEVENT_LENGTH);
    if (length == NULL)
        return;

    const DuBinaryDataPtr &data =
            getChildAs<DuBinaryData>(KEY_MIDIMETAEVENT_DATA);
    if (data == NULL)
        return;

    length->setAbsolute(value);
    data->resize(value);
}

void DuMidiMetaEvent::setLength(QDataStream &stream)
{
    const DuMidiVariableLengthPtr &length =
            getChildAs<DuMidiVariableLength>(KEY_MIDIMETAEVENT_LENGTH);
    if (length == NULL)
        return;

    const DuBinaryDataPtr &data =
            getChildAs<DuBinaryData>(KEY_MIDIMETAEVENT_DATA);
    if (data == NULL)
        return;

    length->setAbsolute(stream);
    data->resize(length->getAbsolute());
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
    const DuMidiVariableLengthPtr &length =
            getChildAs<DuMidiVariableLength>(KEY_MIDIMETAEVENT_LENGTH);
    if (length == NULL)
        return;

    const DuBinaryDataPtr &data =
            getChildAs<DuBinaryData>(KEY_MIDIMETAEVENT_DATA);
    if (data == NULL)
        return;

    length->setAbsolute(value.size());
    data->setData(value);
}

void DuMidiMetaEvent::setData(QDataStream &stream)
{
    const DuBinaryDataPtr &tmp =
            getChildAs<DuBinaryData>(KEY_MIDIMETAEVENT_DATA);

    if (tmp == NULL)
        return;

    tmp->setData(stream);
}
