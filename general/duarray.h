#ifndef DUARRAY_H
#define DUARRAY_H

#include "duobject.h"
#include <QJsonArray>
#include <QList>


DU_OBJECT_TEMPLATE(DuArray);

template <class T>
class DuArray : public DuObject
{
#if QT_VERSION >= 0x050500
    static_assert(std::is_base_of<DuObject, T>::value, "DuArray requires DuObject derived type");
#endif

public:
    explicit DuArray(int m_maxSize = -1);
    virtual ~DuArray() = default;

protected:
    explicit DuArray(const DuArray<T> &other);

public:
    virtual DuObjectPtr clone() const override;

    virtual QByteArray toDuMusicBinary() const override;
    virtual QByteArray toMidiBinary() const override;
    virtual QJsonValue toJson() const override;

    virtual bool parseJson(const QJsonValue &jsonValue) override;

    virtual int size() const override;

    virtual QDebug debugPrint(QDebug dbg) const override;

    virtual bool equals(const DuObject &other) const Q_DECL_OVERRIDE;

    int getMaxSize() const;
    void setMaxSize(int value);

    bool append(const QSharedPointer<T> &element);
    bool append(T* element);
    void insert(int index, const QSharedPointer<T> &element);

    void removeAt(int index);
    void replace(int index, const QSharedPointer<T> &element);

    int count() const;
    bool isEmpty() const;

    QSharedPointer<T> at(int index);
    QSharedPointer<const T> at(int index) const;

    QSharedPointer<T> operator[](int index);

    // STL container functions
    inline typename QList< QSharedPointer<T> >::iterator begin() { return m_array.begin(); }
    inline typename QList< QSharedPointer<T> >::const_iterator begin() const { return m_array.begin(); }
    inline typename QList< QSharedPointer<T> >::const_iterator cbegin() const { return m_array.cbegin(); }
    inline typename QList< QSharedPointer<T> >::iterator end() { return m_array.end(); }
    inline typename QList< QSharedPointer<T> >::const_iterator end() const { return m_array.end(); }
    inline typename QList< QSharedPointer<T> >::const_iterator cend() const { return m_array.cend(); }

protected:
    const QList< QSharedPointer<T> > &getArray() const;

private:
    QList< QSharedPointer<T> > m_array;
    int m_maxSize;
};

DU_OBJEC_TEMPLATE_IMPL(DuArray)

template <class T>
DuArray<T>::DuArray(int maxSize) :
    DuObject(),
    m_maxSize(maxSize)
{
}

template <class T>
DuArray<T>::DuArray(const DuArray<T> &other) :
    DuObject(other),
    m_maxSize(other.m_maxSize)
{
    m_array.reserve(other.m_array.size());
    foreach (const QSharedPointer<T>& obj, other.m_array)
    {
        m_array.append(obj->clone().template dynamicCast<T>());
    }
}

template <class T>
DuObjectPtr DuArray<T>::clone() const
{
    return DuArrayPtr<T>(new DuArray<T>(*this));
}

template <class T>
QByteArray DuArray<T>::toDuMusicBinary() const
{
    QByteArray retArray;
    retArray.clear();

    if (m_array.isEmpty())
    {
        return "";
    }

    foreach (const QSharedPointer<T>& obj, m_array)
    {
        QByteArray tmpArray = obj->toDuMusicBinary();

        if (tmpArray.isNull())
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "element byte array was null";
            return QByteArray();
        }

        retArray.append(tmpArray);
    }

    return retArray;
}

template <class T>
QByteArray DuArray<T>::toMidiBinary() const
{
    QByteArray retArray;
    retArray.clear();

    if (m_array.isEmpty())
    {
        return QByteArray("");
    }

    foreach (const QSharedPointer<T>& obj, m_array)
    {
        QByteArray tmpArray = obj->toMidiBinary();

        if (tmpArray.isNull())
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "element byte array was null";
            return QByteArray();
        }

        retArray.append(tmpArray);
    }

    return retArray;
}

template <class T>
QJsonValue DuArray<T>::toJson() const
{
    QJsonArray jsonArray;

    if (m_array.isEmpty())
    {
        return QJsonValue(QJsonArray());
    }

    foreach (const QSharedPointer<T>& obj, m_array)
    {
        QJsonValue tmpValue = obj->toJson();

        if (tmpValue.isUndefined())
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "element json value was undefined";
            return QJsonValue(QJsonValue::Undefined);
        }

        jsonArray.append(tmpValue);
    }

    return QJsonValue(jsonArray);
}

template <class T>
bool DuArray<T>::parseJson(const QJsonValue &jsonValue)
{
    if (jsonValue.isNull())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "jsonValue is null:" << jsonValue;
        return false;
    }

    if (jsonValue.isUndefined())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "jsonValue is undefined:" << jsonValue;
        return false;
    }

    if (!jsonValue.isArray())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "jsonValue is not an array:" << jsonValue;
        return false;
    }

    const QJsonArray& jsonArray = jsonValue.toArray();

    int originalSize = m_array.size();
    int size = jsonArray.size();

    for (int i = 0; i < size; ++i)
    {
        QSharedPointer<T> obj;
        if (i < originalSize)
        {
            obj = m_array.at(i);
        }
        else
        {
            obj = QSharedPointer<T>(new T);
            if (!append(obj))
            {
                qCCritical(LOG_CAT_DU_OBJECT) << "Failed to append object at index" << i;
                return false;
            }
        }

        const QJsonValue& value = jsonArray.at(i);
        if (!obj->parseJson(value))
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to parse json value" << value
                                          << "at index" << i << "in json array";
            return false;
        }
    }

    return true;
}

template <class T>
int DuArray<T>::size() const
{
    int size = 0;

    foreach (const QSharedPointer<T>& obj, m_array)
    {
        int tmpSize = obj->size();

        if (tmpSize == -1)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "element size was -1";
            return -1;
        }

        size += tmpSize;
    }

    return size;
}

template <class T>
QDebug DuArray<T>::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuArray(";

    QListIterator< QSharedPointer<T> > i(m_array);
    while (i.hasNext())
    {
        dbg.nospace() << i.next();

        if (i.hasNext())
            dbg.nospace() << ", ";
    }

    dbg.nospace() << ")";

    return dbg.space();
}

template<class T>
bool DuArray<T>::equals(const DuObject &other) const
{
    if (!DuObject::equals(other))
        return false;

    const DuArray<T>& castedOther = static_cast<const DuArray<T>&>(other);

    if (m_maxSize != castedOther.m_maxSize)
        return false;

    int count = m_array.size();
    if (count != castedOther.m_array.size())
        return false;

    for (int i = 0; i < count; ++i)
    {
        const QSharedPointer<const T>& element = m_array.at(i);
        const QSharedPointer<const T>& otherElement = castedOther.m_array.at(i);
        if (element.isNull() && otherElement.isNull())
            continue;

        if (element.isNull() || otherElement.isNull())
            return false;

        if (*element != *otherElement)
            return false;
    }

    return true;
}

template <class T>
int DuArray<T>::getMaxSize() const
{
    return m_maxSize;
}

template <class T>
void DuArray<T>::setMaxSize(int value)
{
    if (value != -1 && value < m_array.count())
    {
        qCWarning(LOG_CAT_DU_OBJECT) << value << "is above current element count and was not set";
        return;
    }

    m_maxSize = value;
}

template <class T>
bool DuArray<T>::append(const QSharedPointer<T> &element)
{
    if (m_maxSize != -1 && m_array.count() == m_maxSize)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                   << "the element was not appended\n"
                   << "the array already reached its"
                   << "maximum size" << m_maxSize;

        return false;
    }

    m_array.append(element);
    return true;
}

template <class T>
bool DuArray<T>::append(T *element)
{
    return append(QSharedPointer<T>(element));
}

template <class T>
void DuArray<T>::insert(int index, const QSharedPointer<T> &element)
{
    if (index > m_array.count())
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                   << index << "is above element count\n"
                   << "the element was appended to the array";

        m_array.append(element);
        return;
    }

    m_array.insert(index, element);
}

template <class T>
void DuArray<T>::removeAt(int index)
{
    if (index >= m_array.count())
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "index" << index << "is invalid";
        return;
    }

    m_array.removeAt(index);
}

template <class T>
void DuArray<T>::replace(int index, const QSharedPointer<T> &element)
{
    if (index >= m_array.count())
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                   << index << "is above element count\n"
                   << "the element was appended to the array";

        m_array.append(element);
        return;
    }

    m_array.replace(index, element);
}

template <class T>
int DuArray<T>::count() const
{
    return m_array.count();
}

template <class T>
bool DuArray<T>::isEmpty() const
{
    return m_array.isEmpty();
}

template <class T>
QSharedPointer<T> DuArray<T>::at(int index)
{
    if (index >= m_array.count())
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                   << index << "is above element count\n"
                   << "last element returned";

        return m_array.last();
    }

    return m_array.at(index);
}

template <class T>
QSharedPointer<const T> DuArray<T>::at(int index) const
{
    if (index >= m_array.count())
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                   << index << "is above element count\n"
                   << "last element returned";

        return m_array.last();
    }

    return m_array.at(index);
}

template <class T>
QSharedPointer<T> DuArray<T>::operator[](int index)
{
    if (index >= m_array.count())
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                   << index << "is above element count\n"
                   << "default constructed value returned";

        return DuObjectPtr();
    }

    return m_array[index];
}

template <class T>
const QList< QSharedPointer<T> > &DuArray<T>::getArray() const
{
    return m_array;
}

#endif // DUARRAY_H
