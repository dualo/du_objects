#include "dubinarydata.h"

#include <QJsonValue>



DU_OBJECT_IMPL(DuBinaryData)

DuBinaryData::DuBinaryData(int maxSize) :
    DuValue(maxSize)
{
    resize(0);
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
    return getBinaryData();
}

QByteArray DuBinaryData::toMidiBinary() const
{
    return getBinaryData();
}

QJsonValue DuBinaryData::toJson() const
{
    return QJsonValue(getBinaryData().data());
}


int DuBinaryData::size() const
{
    return getBinaryData().size();
}


const QByteArray DuBinaryData::getBinaryData() const
{
    return getValue().toByteArray();
}


bool DuBinaryData::setBinaryData(const QByteArray &value)
{
    int size = getMaxSize();
    if (size != -1 && value.size() > size)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuBinaryData::setData():\n"
                   << "the byte array was longer than the"
                   << "maximum size" << size
                   << "and was truncated before being set";

        setValue(value.left(size));
        return false;
    }

    setValue(value);
    return true;
}

const QByteArray DuBinaryData::getData() const
{
    return getBinaryData();
}

bool DuBinaryData::setData(const QByteArray &value)
{
    return setBinaryData(value);
}

void DuBinaryData::setData(QDataStream &stream)
{
    QByteArray array;
    int tmp = size();
    array.resize(tmp);

    stream.readRawData(array.data(), tmp);
    setBinaryData(array);
}

void DuBinaryData::resize(int size)
{
    QByteArray tmp = data();
    tmp.resize(size);
    setBinaryData(tmp);
}


void DuBinaryData::append(char c)
{
    QByteArray tmp = data().append(c);
    setBinaryData(tmp);
}


QByteArray DuBinaryData::data()
{
    return getValue().toByteArray();
}
