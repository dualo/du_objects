#include "duarray.h"

DuArray::DuArray(int maxSize) :
    DuObject(),
    maxSize(maxSize)
{
}

DuArray::~DuArray()
{
    while (!array.isEmpty())
        delete array.takeFirst();
}


QJsonValue DuArray::toJson() const
{
    QJsonArray jsonArray;
    int count = array.count();

    for (int i = 0; i < count; i++)
    {
        jsonArray.append(array[i]->toJson());
    }

    return QJsonValue(jsonArray);
}


int DuArray::size() const
{
    int size = 0;
    int count = array.count();

    for (int i = 0; i < count; i++)
    {
        int tmpSize = array[i]->size();
        if (tmpSize == -1)
            return -1;

        size += tmpSize;
    }

    return size;
}


void DuArray::append(DuObject *element)
{
    array.append(element);
}


void DuArray::insert(int index, DuObject *element)
{
    if (index >= array.count())
        array.append(element);

    array.insert(index, element);
}


void DuArray::removeAt(int index)
{
    if (index >= array.count())
        return;

    array.removeAt(index);
}

void DuArray::replace(int index, DuObject *element)
{
    if (index >= array.count())
        array.append(element);

    array.replace(index, element);
}


int DuArray::count() const
{
    return array.count();
}


DuObject *DuArray::at(int index)
{
    if (index >= array.count())
        return NULL;

    return array.at(index);
}

DuObject *DuArray::operator[](int index)
{
    if (index >= array.count())
        return NULL;

    return array[index];
}
