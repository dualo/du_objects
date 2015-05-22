#include "duarray.h"

#include <QDebug>

DuArray::DuArray(int maxSize) :
    DuObject(),
    maxSize(maxSize)
{
}

DuArray::~DuArray()
{
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

QDebug DuArray::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuArray(";

    QListIterator< QSharedPointer<DuObject> > i(array);
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

QByteArray DuArray::toDuMusicFile() const
{
    QByteArray retArray;
    retArray.clear();

    if (array.isEmpty())
        return QByteArray("");

    int count = array.count();
    for (int i = 0; i < count; i++)
    {
        retArray.append(array[i]->toDuMusicFile());
    }

    return retArray;
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
    array.append(QSharedPointer<DuObject>(element));
}


void DuArray::insert(int index, DuObject *element)
{
    if (index >= array.count())
    {
        array.append(QSharedPointer<DuObject>(element));
        return;
    }

    array.insert(index, QSharedPointer<DuObject>(element));
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
    {
        array.append(QSharedPointer<DuObject>(element));
        return;
    }

    array.replace(index, QSharedPointer<DuObject>(element));
}


int DuArray::count() const
{
    return array.count();
}


QSharedPointer<DuObject> DuArray::at(int index)
{
    if (index >= array.count())
        return QSharedPointer<DuObject>();

    return array.at(index);
}

QSharedPointer<DuObject> DuArray::operator[](int index)
{
    if (index >= array.count())
        return QSharedPointer<DuObject>();

    return array[index];
}
