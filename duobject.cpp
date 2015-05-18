#include "duobject.h"

#include <QDebug>

DuObject::DuObject()
{
}

DuObject::~DuObject()
{
}

QDebug DuObject::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuObject(" << (void*)this << ")";

    return dbg.space();
}

QDebug operator<<(QDebug dbg, const DuObject *obj)
{
    if (!obj)
        return dbg << "DuObject(0x0) ";

    return obj->debugPrint(dbg);
}
