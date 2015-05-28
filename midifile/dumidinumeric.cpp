#include "dumidinumeric.h"

#include <QDebug>

DU_OBJECT_IMPL(DuMidiNumeric)


DuMidiNumeric::DuMidiNumeric(int value, int byteSize, int maxValue, int minValue) :
    DuMidiValue(byteSize),
    maxValue(maxValue),
    minValue(minValue)
{
    setNumeric(value);
}

DuMidiNumeric::~DuMidiNumeric()
{
}


DuObjectPtr DuMidiNumeric::clone() const
{
    return DuMidiNumericPtr(new DuMidiNumeric(*this));
}


const QByteArray DuMidiNumeric::toMidiBinary() const
{
    int size = getMaxSize();

    if (size == -1)
        return QByteArray();

    if (size == 0)
        return QByteArray("");

    QByteArray retArray;
    retArray.clear();
    int tmpValue = getNumeric();

    for (int i = 1; i < size + 1; i++)
    {
        retArray.append((quint8)((tmpValue >> ((size - i) * 8)) & 0xFF));
    }

    return retArray;
}


QDebug DuMidiNumeric::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuMidiNumeric(" << getNumeric() << ")";

    return dbg.space();
}


int DuMidiNumeric::getNumeric() const
{
    return getValue().toInt();
}

bool DuMidiNumeric::setNumeric(int value)
{
    if (value > maxValue || value < minValue)
        return false;

    setValue(value);
    return true;
}


int DuMidiNumeric::getMax() const
{
    return maxValue;
}

bool DuMidiNumeric::setMax(int value)
{
    if (value < getNumeric())
        return false;

    maxValue = value;
    return true;
}


int DuMidiNumeric::getMin() const
{
    return minValue;
}

bool DuMidiNumeric::setMin(int value)
{
    if (value > getNumeric())
        return false;

    minValue = value;
    return true;
}
