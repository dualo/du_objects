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
    data().resize(size);
}


QByteArray &DuMidiData::append(char c)
{
    return data().append(c);
}


QByteArray DuMidiData::data()
{
    return getValue().toByteArray();
}
