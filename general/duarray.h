#ifndef DUARRAY_H
#define DUARRAY_H

#include "duobject.h"
#include <QList>


DU_OBJECT(DuArray);

class DuArray : public DuObject
{
public:
    explicit DuArray(int maxSize = -1);
    virtual ~DuArray();

protected:
    explicit DuArray(const DuArray &other);

public:
    virtual DuObjectPtr clone() const;

    QByteArray toDuMusicBinary() const;
    QByteArray toMidiBinary() const;
    QJsonValue toJson() const;

    int size() const;

    QDebug debugPrint(QDebug dbg) const;

    int getMaxSize() const;
    void setMaxSize(int value);

    bool append(const DuObjectPtr &element);
    bool append(DuObject* element);
    void insert(int index, const DuObjectPtr &element);

    void removeAt(int index);
    void replace(int index, const DuObjectPtr &element);

    int count() const;
    bool isEmpty() const;

    DuObjectPtr at(int index);
    DuObjectConstPtr at(int index) const;

    template <class T>
    QSharedPointer<T> atAs(int index);
    template <class T>
    QSharedPointer<const T> atAs(int index) const;

    DuObjectPtr operator[](int index);

protected:
    const QVector<DuObjectPtr> &getArray() const;

private:
    QVector<DuObjectPtr> array;
    int maxSize;
};

template <class T>
inline QSharedPointer<T> DuArray::atAs(int index)
{
    return at(index).dynamicCast<T>();
}

template <class T>
inline QSharedPointer<const T> DuArray::atAs(int index) const
{
    return at(index).dynamicCast<const T>();
}

#endif // DUARRAY_H
