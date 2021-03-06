#include "duvalue.h"

#include <QJsonValue>


DU_OBJECT_IMPL(DuValue)


DuValue::DuValue(int maxSize) :
    DuObject(),
    m_maxSize(maxSize)
{
}

DuValue::DuValue(const QVariant &value, int maxSize) :
    DuObject(),
    m_value(value),
    m_maxSize(maxSize)
{
}

DuObjectPtr DuValue::clone() const
{
    return DuValuePtr(new DuValue(*this));
}

QByteArray DuValue::toDuMusicBinary() const
{
    Q_UNIMPLEMENTED();
    return m_value.toByteArray();
}

QJsonValue DuValue::toJson() const
{
    Q_UNIMPLEMENTED();
    return m_value.toJsonValue();
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
    return m_maxSize;
}


const QVariant &DuValue::getValue() const
{
    return m_value;
}

bool DuValue::setValue(const QVariant &value)
{
    bool success = false;
    this->m_value = checkValue(value, success);
    return success;
}

QVariant DuValue::checkValue(const QVariant &value, bool& success)
{
    success = true;
    return value;
}

int DuValue::getMaxSize() const
{
    return m_maxSize;
}

void DuValue::setMaxSize(int value)
{
    m_maxSize = value;
}

QDebug DuValue::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuValue(";
    dbg.nospace() << m_value;
    dbg.nospace() << ")";

    return dbg.space();
}


QVariant &DuValue::getValue()
{
    return m_value;
}

bool DuValue::equals(const DuObject &other) const
{
    if (!DuObject::equals(other))
        return false;

    const DuValue& castedOther = static_cast<const DuValue&>(other);
    return this->m_value == castedOther.m_value && this->m_maxSize == castedOther.m_maxSize;
}
