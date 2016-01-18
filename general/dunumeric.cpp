#include "dunumeric.h"

#include <QJsonValue>
#include <climits>


DU_OBJECT_IMPL(DuNumeric)

DuNumeric::DuNumeric(int value) :
    DuValue(4),
    defaultValue(0),
    maxValue(INT_MAX),
    minValue(INT_MIN)
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
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuNumeric::toDuMusicBinary():\n"
                   << "invalid numeric size";

        return QByteArray();
    }

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
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuNumeric::toMidiBinary():\n"
                   << "invalid numeric size";

        return QByteArray();
    }

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

QVariant DuNumeric::checkValue(const QVariant &value, bool &success)
{
    if (!value.canConvert<int>())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "value is not of type int:" << value;
        success = false;
        return QVariant();
    }

    int convertedValue = value.toInt();

    if (convertedValue < minValue)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                << convertedValue << "below" << minValue
                << "and could not be set\n"
                << minValue << "was set instead";

        success = false;
        return minValue;
    }
    else if (convertedValue > maxValue)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                << convertedValue << "above" << maxValue
                << "and could not be set\n"
                << maxValue << "was set instead";

        success = false;
        return maxValue;
    }

    success = true;
    return value;
}

int DuNumeric::getNumeric() const
{
    return getValue().toInt();
}

bool DuNumeric::setNumeric(int value)
{
    return setValue(value);
}


int DuNumeric::getDefault() const
{
    return defaultValue;
}

bool DuNumeric::setDefault(int value)
{
    if (value > maxValue || value < minValue)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                << value << "not comprised between"
                << minValue << "and" << maxValue;

        return false;
    }

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
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuNumeric::setMax():\n"
                   << value << "is below" << getNumeric()
                   << "or" << defaultValue
                   << "and was not set";

        return false;
    }

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
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuNumeric::setMin():\n"
                   << value << "is above" << getNumeric()
                   << "or" << defaultValue
                   << "and was not set";

        return false;
    }

    minValue = value;
    return true;
}
