#include "duarray.h"

#include <QDebug>
#include <QListIterator>
#include <QJsonArray>


DU_OBJECT_IMPL(DuArray)

DuArray::DuArray(int maxSize) :
    DuObject(),
    maxSize(maxSize)
{
}

DuArray::~DuArray()
{
}

DuArray::DuArray(const DuArray &other) :
    DuObject(other),
    maxSize(other.maxSize)
{
    QListIterator<DuObjectPtr> i(other.array);
    while (i.hasNext()) {
        array.append(i.next()->clone());
    }
}

DuObjectPtr DuArray::clone() const
{
    return DuArrayPtr(new DuArray(*this));
}


QByteArray DuArray::toDuMusicBinary() const
{
    QByteArray retArray;
    retArray.clear();

    if (array.isEmpty())
        return QByteArray("");

    QListIterator<DuObjectPtr> i(array);
    while (i.hasNext())
    {
        retArray.append(i.next()->toDuMusicBinary());
    }

    return retArray;
}


QByteArray DuArray::toMidiBinary() const
{
    QByteArray retArray;
    retArray.clear();

    if (array.isEmpty())
        return QByteArray("");

    QListIterator<DuObjectPtr> i(array);
    while (i.hasNext())
    {
        retArray.append(i.next()->toMidiBinary());
    }

    return retArray;
}


QJsonValue DuArray::toJson() const
{
    QJsonArray jsonArray;

    if (array.isEmpty())
        return QJsonValue(QJsonArray());

    QListIterator<DuObjectPtr> i(array);
    while (i.hasNext())
    {
        jsonArray.append(i.next()->toJson());
    }

    return QJsonValue(jsonArray);
}


int DuArray::size() const
{
    int size = 0;

    QListIterator<DuObjectPtr> i(array);
    while (i.hasNext())
    {
        int tmpSize = i.next()->size();
        if (tmpSize == -1)
            return -1;

        size += tmpSize;
    }

    return size;
}


QDebug DuArray::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuArray(";

    QListIterator<DuObjectPtr> i(array);
    while (i.hasNext()) {
        dbg.nospace() << i.next();

        if (i.hasNext())
            dbg.nospace() << ", ";
    }

    dbg.nospace() << ")";

    return dbg.space();
}


int DuArray::getMaxSize() const
{
    return maxSize;
}

void DuArray::setMaxSize(int value)
{
    maxSize = value;
}


void DuArray::append(const DuObjectPtr &element)
{
    array.append(element);
}

void DuArray::insert(int index, const DuObjectPtr &element)
{
    if (index >= array.count())
    {
        array.append(element);
        return;
    }

    array.insert(index, element);
}


void DuArray::removeAt(int index)
{
    if (index >= array.count())
        return;

    array.removeAt(index);
}

void DuArray::replace(int index, const DuObjectPtr &element)
{
    if (index >= array.count())
    {
        array.append(element);
        return;
    }

    array.replace(index, element);
}


int DuArray::count() const
{
    return array.count();
}

bool DuArray::isEmpty() const
{
    return array.isEmpty();
}


DuObjectPtr DuArray::at(int index)
{
    if (index >= array.count())
        return DuObjectPtr();

    return array.at(index);
}

DuObjectConstPtr DuArray::at(int index) const
{
    if (index >= array.count())
        return DuObjectConstPtr();

    return array.at(index);
}

DuObjectPtr DuArray::operator[](int index)
{
    if (index >= array.count())
        return DuObjectPtr();

    return array[index];
}


const QList<DuObjectPtr> &DuArray::getArray() const
{
    return array;
}
