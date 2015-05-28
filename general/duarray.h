#ifndef DUARRAY_H
#define DUARRAY_H

#include "duobject.h"
#include <QList>


DU_OBJECT(DuArray)

class DuArray : public DuObject
{
public:
    explicit DuArray(int maxSize = -1);
    ~DuArray();

protected:
    explicit DuArray(const DuArray &other);

public:
    virtual DuObjectPtr clone() const;

    QJsonValue toJson() const;
    QByteArray toDuMusicBinary() const;

    QDebug debugPrint(QDebug dbg) const;

    int getMaxSize() const;
    void setMaxSize(int value);

    int size() const;

    void append(const DuObjectPtr& element);
    void insert(int index, const DuObjectPtr& element);

    void removeAt(int index);
    void replace(int index, const DuObjectPtr& element);

    int count() const;

    DuObjectPtr at(int index);
    DuObjectConstPtr at(int index) const;
    DuObjectPtr operator[](int index);

private:
    QList<DuObjectPtr> array;
    int maxSize;
};

#endif // DUARRAY_H
