#include "duvalue.h"

#include <QJsonValue>


DU_OBJECT_IMPL(DuValue)


DuValue::DuValue(int maxSize) :
    DuObject(),
    maxSize(maxSize)
{
}

DuValue::DuValue(const QVariant &value, int maxSize) :
    DuObject(),
    value(value),
    maxSize(maxSize)
{
}

DuValue::~DuValue()
{
}

bool DuValue::parseJson(const QJsonValue &jsonValue)
{
    if (jsonValue.isNull())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "jsonValue is null:" << jsonValue;
        return false;
    }

    if (jsonValue.isUndefined())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "jsonValue is undefined:" << jsonValue;
        return false;
    }

    return setValue(jsonValue.toVariant());
}

int DuValue::size() const
{
    return maxSize;
}


const QVariant &DuValue::getValue() const
{
    return value;
}

bool DuValue::setValue(const QVariant &value)
{
    bool success = false;
    this->value = checkValue(value, success);
    return success;
}

QVariant DuValue::checkValue(const QVariant &value, bool& success)
{
    success = true;
    return value;
}

int DuValue::getMaxSize() const
{
    return maxSize;
}

void DuValue::setMaxSize(int value)
{
    maxSize = value;
}

QDebug DuValue::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuValue(";
    dbg.nospace() << value;
    dbg.nospace() << ")";

    return dbg.space();
}


QVariant &DuValue::getValue()
{
    return value;
}
