#include "dunumeric.h"

#include <QDebug>


DU_OBJECT_IMPL(DuNumeric)

DuNumeric::DuNumeric(int value) :
    DuValue(4),
    defaultValue(0),
    maxValue(0x7FFFFFFF),
    minValue(0x00000000)
{
    bool res = setNumeric(value);
    Q_ASSERT(res);
}

DuNumeric::DuNumeric(int defaultValue, int byteSize, int maxValue, int minValue) :
    DuValue(byteSize),
    defaultValue(defaultValue),
    maxValue(maxValue),
    minValue(minValue)
{
    bool res = setNumeric(defaultValue)
            && minValue <= defaultValue
            && maxValue >= defaultValue;

    Q_ASSERT(res);
}

DuNumeric::DuNumeric(int value, int byteSize,
                     int maxValue, int minValue, int defaultValue) :
    DuValue(byteSize),
    defaultValue(defaultValue),
    maxValue(maxValue),
    minValue(minValue)
{
    bool res = setNumeric(value)
            && minValue <= defaultValue
            && maxValue >= defaultValue;

    Q_ASSERT(res);
}

DuNumeric::~DuNumeric()
{
}


DuObjectPtr DuNumeric::clone() const
{
    return DuNumericPtr(new DuNumeric(*this));
}


QByteArray DuNumeric::toDuMusicBinary() const
{
    int size = getMaxSize();

    if (size == -1)
        return QByteArray();

    if (size == 0)
        return QByteArray("");

    QByteArray retArray;
    retArray.clear();

    int num = getNumeric();

    for (int i = 0; i < size; i++)
    {
        retArray.append((quint8)((num >> (i * 8)) & 0xFF));
    }

    return retArray;
}

QByteArray DuNumeric::toMidiBinary() const
{
    int size = getMaxSize();

    if (size == -1)
        return QByteArray();

    if (size == 0)
        return QByteArray("");

    QByteArray retArray;
    retArray.clear();

    int num = getNumeric();

    for (int i = 1; i < size + 1; i++)
    {
        retArray.append((quint8)((num >> ((size - i) * 8)) & 0xFF));
    }

    return retArray;
}

QJsonValue DuNumeric::toJson() const
{
    return QJsonValue(getNumeric());
}


QHttpPart DuNumeric::toHttpPart(const QString &name) const
{
    QHttpPart part;
    part.setHeader(QNetworkRequest::ContentDispositionHeader,
                   "form-data; name=\"" + name + "\"");
    part.setBody(QByteArray::number(getNumeric()));

    return part;
}


QDebug DuNumeric::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuNumeric(" << getNumeric() << ")";

    return dbg.space();
}


int DuNumeric::getNumeric() const
{
    return getValue().toInt();
}

bool DuNumeric::setNumeric(int value)
{
    if (value > maxValue || value < minValue)
    {
        setValue(defaultValue);
        return false;
    }

    setValue(value);
    return true;
}


int DuNumeric::getDefault() const
{
    return defaultValue;
}

bool DuNumeric::setDefault(int value)
{
    if (value > maxValue || value < minValue)
        return false;

    defaultValue = value;
    return true;
}


int DuNumeric::getMax() const
{
    return maxValue;
}

bool DuNumeric::setMax(int value)
{
    if (value < getNumeric() || value < defaultValue)
        return false;

    maxValue = value;
    return true;
}

int DuNumeric::getMin() const
{
    return minValue;
}

bool DuNumeric::setMin(int value)
{
    if (value > getNumeric() || value > defaultValue)
        return false;

    minValue = value;
    return true;
}
