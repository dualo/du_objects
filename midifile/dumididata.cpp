#include "dumididata.h"

#include <QDebug>

DU_OBJECT_IMPL(DuMidiData)


DuMidiData::DuMidiData(int maxSize) :
    DuMidiValue(maxSize)
{
}

DuMidiData::~DuMidiData()
{
}


DuObjectPtr DuMidiData::clone() const
{
    return DuMidiDataPtr(new DuMidiData(*this));
}


const QByteArray DuMidiData::toMidiBinary() const
{
    return getData();
}


int DuMidiData::size() const
{
    return getData().size();
}


const QByteArray DuMidiData::getData() const
{
    return getValue().toByteArray();
}


bool DuMidiData::setData(const QByteArray &value)
{
    if (getMaxSize() != -1 && value.size() > getMaxSize())
    {
        setValue(value.left(getMaxSize()));
        return false;
    }

    setValue(value);
    return true;
}

void DuMidiData::setData(QDataStream &stream)
{
    QByteArray array;
    int tmp = size();
    array.resize(tmp);

    stream.readRawData(array.data(), tmp);
    setData(array);
}

void DuMidiData::resize(int size)
{
    QByteArray tmp = data();
    tmp.resize(size);
    setData(tmp);
}


void DuMidiData::append(char c)
{
    QByteArray tmp = data().append(c);
    setData(tmp);
}


QByteArray DuMidiData::data()
{
    return getValue().toByteArray();
}
