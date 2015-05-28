#ifndef DUMIDIARRAY_H
#define DUMIDIARRAY_H

#include "dumidiobject.h"
#include <QList>


DU_OBJECT(DuMidiArray)


class DuMidiArray : public DuMidiObject
{
public:
    explicit DuMidiArray(int maxSize = -1);
    ~DuMidiArray();

protected:
    explicit DuMidiArray(const DuMidiArray &other);

public:
    virtual DuObjectPtr clone() const;

    virtual const QByteArray toMidiBinary() const;

    int size() const;

    QDebug debugPrint(QDebug dbg) const;

private:
    QList<DuMidiObjectPtr> array;
    int maxSize;
};

#endif // DUMIDIARRAY_H
