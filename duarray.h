#ifndef DUARRAY_H
#define DUARRAY_H

#include "duobject.h"
#include <QList>


class DuArray : public DuObject
{
public:
    explicit DuArray(int maxSize);
    ~DuArray();

    QJsonValue toJson() const;

    void append(DuObject *element);
    int count();

    const QList<DuObject *> &getArray() const;

    DuObject *at(int index);
    DuObject *operator[](int index);

private:
    QList<DuObject *> array;
    int maxSize;
};

#endif // DUARRAY_H
