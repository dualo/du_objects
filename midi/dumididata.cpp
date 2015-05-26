#include "dumididata.h"

DuMidiData::DuMidiData(int maxSize) :
    DuValue(maxSize)
{
}

DuMidiData::~DuMidiData()
{
}


QByteArray DuMidiData::toDuMusicFile() const
{
    return QByteArray();
}

QJsonValue DuMidiData::toJson() const
{
    return QJsonValue();
}


int DuMidiData::size() const
{
    return getConstData().size();
}


const QByteArray DuMidiData::getConstData() const
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


QByteArray &DuMidiData::append(char c)
{
    return getData().append(c);
}


QByteArray DuMidiData::getData()
{
    return getValue().toByteArray();
}
