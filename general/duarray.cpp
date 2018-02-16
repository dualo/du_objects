#include "duarray.h"

DU_OBJECT_IMPL(DuArrayNoTemplate)

DuArrayNoTemplate::DuArrayNoTemplate() : DuObject()
{
}

DU_OBJECT_IMPL(DuNumericArray)

DuNumericArray::DuNumericArray(int maxSize) : DuArray<DuNumeric>(maxSize)
{
}

DuObjectPtr DuNumericArray::clone() const
{
    return DuNumericArrayPtr(new DuNumericArray(*this));
}

QList<int> DuNumericArray::getNumericArray() const
{
    QList<int> array;
    for (const DuNumericConstPtr& numeric : getArray())
    {
        array << numeric->getNumeric();
    }
    return array;
}

bool DuNumericArray::setNumericArray(const QList<int> &value)
{
    QList<DuNumericPtr>& array = getArray();
    array.clear();
    const int maxSize = getMaxSize();
    for (const int element : value)
    {
        if (maxSize != -1 && array.count() == maxSize)
        {
            qCWarning(LOG_CAT_DU_OBJECT)
                       << "some elements were not appended\n"
                       << "the array already reached its"
                       << "maximum size" << maxSize;

            return false;
        }

        array << DuNumericPtr(new DuNumeric(element));
    }
    return true;
}

bool DuNumericArray::append(int element)
{
    return DuArray::append(new DuNumeric(element));
}

void DuNumericArray::insert(int index, int element)
{
    DuArray::insert(index, DuNumericPtr(new DuNumeric(element)));
}

void DuNumericArray::replace(int index, int element)
{
    DuArray::replace(index, DuNumericPtr(new DuNumeric(element)));
}

int DuNumericArray::at(int index) const
{
    return DuArray::at(index)->getNumeric();
}

int DuNumericArray::operator[](int index) const
{
    return DuArray::operator[](index)->getNumeric();
}
