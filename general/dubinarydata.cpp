#include "dubinarydata.h"

#include <QJsonValue>



DU_OBJECT_IMPL(DuBinaryData)

DuBinaryData::DuBinaryData(int maxSize) :
    DuValue(maxSize)
{
    resize(0);
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

QDebug DuBinaryData::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuBinaryData(" << getBinaryData().toHex() << ")";

    return dbg.space();
}

int DuBinaryData::size() const
{
    return getBinaryData().size();
}

QVariant DuBinaryData::checkValue(const QVariant &value, bool &success)
{
    if (!value.canConvert<QByteArray>())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "value is not of type QByteArray:" << value;
        success = false;
        return QVariant();
    }

    const QByteArray& convertedValue = value.toByteArray();

    int size = getMaxSize();
    if (size != -1 && convertedValue.size() > size)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                << "the byte array was longer than the"
                << "maximum size" << size
                << "and was truncated before being set";

        success = false;
        return convertedValue.left(size);
    }

    success = true;
    return value;
}

const QByteArray DuBinaryData::getBinaryData() const
{
    return getValue().toByteArray();
}


bool DuBinaryData::setBinaryData(const QByteArray &value)
{
    return setValue(value);
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
