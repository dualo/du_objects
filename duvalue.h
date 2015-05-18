#ifndef DUVALUE_H
#define DUVALUE_H

#include "duobject.h"
#include <QVariant>


class DuValue : public DuObject
{
public:
    explicit DuValue();
    explicit DuValue(int maxSize);
    virtual ~DuValue();

    virtual int size() const;

    QVariant getValue() const;
    void setValue(const QVariant &value);

    int getMaxSize() const;
    void setMaxSize(int value);

    QDebug debugPrint(QDebug dbg) const;

private:
    QVariant value;
    int maxSize;
};

#endif // DUVALUE_H
