#include "dumidivalue.h"

#include <QDebug>

DU_OBJECT_IMPL(DuMidiValue)


DuMidiValue::DuMidiValue(int maxSize) :
    DuMidiObject(),
    maxSize(maxSize)
{
}

DuMidiValue::~DuMidiValue()
{
}

QDebug DuMidiValue::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuMidiValue(";
    dbg.nospace() << value;
    dbg.nospace() << ")";

    return dbg.space();
}

int DuMidiValue::size() const
{
    return maxSize;
}


const QVariant &DuMidiValue::getValue() const
{
    return value;
}

void DuMidiValue::setValue(const QVariant &value)
{
    this->value = value;
}


int DuMidiValue::getMaxSize() const
{
    return maxSize;
}

void DuMidiValue::setMaxSize(int value)
{
    maxSize = value;
}


QVariant &DuMidiValue::getValue()
{
    return value;
}
