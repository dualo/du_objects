#include "dubinarydata.h"

#include <QDebug>


DU_OBJECT_IMPL(DuBinaryData)

DuBinaryData::DuBinaryData(int maxSize) :
    DuValue(maxSize)
{
}

DuBinaryData::~DuBinaryData()
{
}


DuObjectPtr DuBinaryData::clone() const
{
    return DuBinaryDataPtr(new DuBinaryData(*this));
}


QByteArray DuBinaryData::toDuMusicBinary() const
{
    Q_UNIMPLEMENTED();
    return QByteArray();
}

QByteArray DuBinaryData::toMidiBinary() const
{
    return getData();
}

QJsonValue DuBinaryData::toJson() const
{
    Q_UNIMPLEMENTED();
    return QJsonValue();
}


int DuBinaryData::size() const
{
    return getData().size();
}


const QByteArray DuBinaryData::getData() const
{
    return getValue().toByteArray();
}


bool DuBinaryData::setData(const QByteArray &value)
{
    if (getMaxSize() != -1 && value.size() > getMaxSize())
    {
        setValue(value.left(getMaxSize()));
        return false;
    }

    setValue(value);
    return true;
}

void DuBinaryData::setData(QDataStream &stream)
{
    QByteArray array;
    int tmp = size();
    array.resize(tmp);

    stream.readRawData(array.data(), tmp);
    setData(array);
}

void DuBinaryData::resize(int size)
{
    QByteArray tmp = data();
    tmp.resize(size);
    setData(tmp);
}


void DuBinaryData::append(char c)
{
    QByteArray tmp = data().append(c);
    setData(tmp);
}


QByteArray DuBinaryData::data()
{
    return getValue().toByteArray();
}
