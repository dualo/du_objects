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
    QByteArray toDuMusicFile() const;

    QDebug debugPrint(QDebug dbg) const;

    int getMaxSize() const;
    void setMaxSize(int value);

    int size() const;

    void append(DuObject *element);
    void insert(int index, DuObject *element);

    void removeAt(int index);
    void replace(int index, DuObject *element);

    int count() const;

    DuObject *at(int index);
    DuObject *operator[](int index);

private:
    QList<DuObject *> array;
    int maxSize;
};

#endif // DUARRAY_H
