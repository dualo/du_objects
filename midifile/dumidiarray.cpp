#include "dumidiarray.h"

#include <QDebug>
#include <QListIterator>


DU_OBJECT_IMPL(DuMidiArray)

DuMidiArray::DuMidiArray(int maxSize) :
    DuArray(maxSize)
{
}

DuMidiArray::~DuMidiArray()
{
}

DuObjectPtr DuMidiArray::clone() const
{
    return DuMidiArrayPtr(new DuMidiArray(*this));
}


QByteArray DuMidiArray::toDuMusicBinary() const
{
    Q_UNIMPLEMENTED();
    return QByteArray();
}


QByteArray DuMidiArray::toMidiBinary() const
{
    QByteArray retArray;
    retArray.clear();

    if (isEmpty())
        return QByteArray("");

    QListIterator<DuObjectPtr> i(getArray());
    while (i.hasNext())
    {
        retArray.append(i.next()->toMidiBinary());
    }

    return retArray;
}


QJsonValue DuMidiArray::toJson() const
{
    Q_UNIMPLEMENTED();
    return QJsonValue();
}


int DuMidiArray::size() const
{
    int size = 0;

    QListIterator<DuObjectPtr> i(getArray());
    while (i.hasNext())
    {
        int tmpSize = i.next()->size();
        if (tmpSize == -1)
            return -1;

        size += tmpSize;
    }

    return size;
}


QDebug DuMidiArray::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuMidiArray(";

    QListIterator<DuObjectPtr> i(getArray());
    while (i.hasNext()) {
        dbg.nospace() << i.next();

        if (i.hasNext())
            dbg.nospace() << ", ";
    }

    dbg.nospace() << ")";

    return dbg.space();
}
