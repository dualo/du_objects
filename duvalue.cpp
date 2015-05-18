#include "duvalue.h"

#include <QDebug>

DuValue::DuValue() :
    DuObject(),
    maxSize(0)
{
}

DuValue::DuValue(int maxSize) :
    DuObject(),
    maxSize(maxSize)
{
}

DuValue::~DuValue()
{
}


int DuValue::size() const
{
    return maxSize;
}


QVariant DuValue::getValue() const
{
    return value;
}

void DuValue::setValue(const QVariant &value)
{
    this->value = value;
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
