#ifndef DUARRAY_H
#define DUARRAY_H

#include "duobject.h"
#include <QList>


DU_OBJECT(DuArray)

class DuArray : public DuObject
{
public:
    explicit DuArray(int maxSize = -1);
    virtual ~DuArray();

protected:
    explicit DuArray(const DuArray &other);

public:
    virtual DuObjectPtr clone() const;

    virtual QByteArray toDuMusicBinary() const;
    QJsonValue toJson() const;

    int size() const;

    QDebug debugPrint(QDebug dbg) const;

    int getMaxSize() const;
    void setMaxSize(int value);

    void append(const DuObjectPtr &element);
    void insert(int index, const DuObjectPtr &element);

    void removeAt(int index);
    void replace(int index, const DuObjectPtr &element);

    int count() const;
    bool isEmpty() const;

    DuObjectPtr at(int index);
    DuObjectConstPtr at(int index) const;
    DuObjectPtr operator[](int index);

protected:
    const QList<DuObjectPtr> &getArray() const;

private:
    QList<DuObjectPtr> array;
    int maxSize;
};

#endif // DUARRAY_H
