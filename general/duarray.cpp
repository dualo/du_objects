#include "duarray.h"

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
    array.reserve(other.array.size());
    foreach (const DuObjectPtr& obj, other.array)
    {
        array.append(obj->clone());
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
    {
        return QByteArray();
    }

    QListIterator<DuObjectPtr> i(array);
    while (i.hasNext())
    {
        QByteArray tmpArray = i.next()->toDuMusicBinary();

        if (tmpArray.isNull())
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "DuArray::toDuMusicBinary():\n"
                        << "element byte array was null";

            return QByteArray();
        }

        retArray.append(tmpArray);
    }

    return retArray;
}


QByteArray DuArray::toMidiBinary() const
{
    QByteArray retArray;
    retArray.clear();

    if (array.isEmpty())
    {
        return QByteArray("");
    }

    QListIterator<DuObjectPtr> i(array);
    while (i.hasNext())
    {
        QByteArray tmpArray = i.next()->toMidiBinary();

        if (tmpArray.isNull())
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "DuArray::toMidiBinary():\n"
                        << "element byte array was null";

            return QByteArray();
        }

        retArray.append(tmpArray);
    }

    return retArray;
}


QJsonValue DuArray::toJson() const
{
    QJsonArray jsonArray;

    if (array.isEmpty())
    {
        return QJsonValue(QJsonArray());
    }

    QListIterator<DuObjectPtr> i(array);
    while (i.hasNext())
    {
        QJsonValue tmpValue = i.next()->toJson();

        if (tmpValue.isUndefined())
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "DuArray::toJson():\n"
                        << "element json value was undefined";

            return QJsonValue(QJsonValue::Undefined);
        }

        jsonArray.append(tmpValue);
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
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "DuArray::size():\n"
                        << "element size was -1";
            return -1;
        }

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
    if (value != -1 && value < array.count())
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuArray::setMaxSize():\n"
                   << value << "is above current"
                   << "element count and was not set";

        return;
    }

    maxSize = value;
}


bool DuArray::append(const DuObjectPtr &element)
{
    if (maxSize != -1 && array.count() == maxSize)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuArray::append():\n"
                   << "the element was not appended\n"
                   << "the array already reached its"
                   << "maximum size" << maxSize;

        return false;
    }

    array.append(element);
    return true;
}

bool DuArray::append(DuObject *element)
{
    return append(DuObjectPtr(element));
}

void DuArray::insert(int index, const DuObjectPtr &element)
{
    if (index > array.count())
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuArray::insert():\n"
                   << index << "is above element count\n"
                   << "the element was appended to the array";

        array.append(element);
        return;
    }

    array.insert(index, element);
}


void DuArray::removeAt(int index)
{
    if (index >= array.count())
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuArray::removeAt():\n"
                   << "index" << index << "is invalid";
        return;
    }

    array.removeAt(index);
}

void DuArray::replace(int index, const DuObjectPtr &element)
{
    if (index >= array.count())
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuArray::replace():\n"
                   << index << "is above element count\n"
                   << "the element was appended to the array";

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
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuArray::at():\n"
                   << index << "is above element count\n"
                   << "last element returned";

        return array.last();
    }

    return array.at(index);
}

DuObjectConstPtr DuArray::at(int index) const
{
    if (index >= array.count())
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuArray::at():\n"
                   << index << "is above element count\n"
                   << "last element returned";

        return array.last();
    }

    return array.at(index);
}

DuObjectPtr DuArray::operator[](int index)
{
    if (index >= array.count())
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuArray::operator[]:\n"
                   << index << "is above element count\n"
                   << "default constructed value returned";

        return DuObjectPtr();
    }

    return array[index];
}


const QList<DuObjectPtr> &DuArray::getArray() const
{
    return array;
}
