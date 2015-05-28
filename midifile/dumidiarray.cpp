#include "dumidiarray.h"

#include <QListIterator>
#include <QDebug>

DU_OBJECT_IMPL(DuMidiArray)


DuMidiArray::DuMidiArray(int maxSize) :
    DuMidiObject(),
    maxSize(maxSize)
{
}

DuMidiArray::~DuMidiArray()
{
}

DuMidiArray::DuMidiArray(const DuMidiArray &other) :
    DuMidiObject(other),
    maxSize(other.maxSize)
{
    QListIterator<DuMidiObjectPtr> i(other.array);
    while (i.hasNext()) {
        array.append(i.next()->clone().dynamicCast<DuMidiObject>());
    }
}

DuObjectPtr DuMidiArray::clone() const
{
    return DuMidiArrayPtr(new DuMidiArray(*this));
}


const QByteArray DuMidiArray::toMidiBinary() const
{
    QByteArray retArray;
    retArray.clear();

    if (array.isEmpty())
        return QByteArray("");

    int count = array.count();
    for (int i = 0; i < count; i++)
    {
        retArray.append(array[i]->toMidiBinary());
    }

    return retArray;
}


int DuMidiArray::size() const
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


QDebug DuMidiArray::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuMidiArray(";

    QListIterator<DuMidiObjectPtr> i(array);
    while (i.hasNext()) {
        dbg.nospace() << i.next();

        if (i.hasNext())
            dbg.nospace() << ", ";
    }

    dbg.nospace() << ")";

    return dbg.space();
}
