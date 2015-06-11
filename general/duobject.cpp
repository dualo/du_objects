#include "duobject.h"

#include <QDebug>

DU_OBJECT_IMPL(DuObject)


DuObject::DuObject()
{
}

DuObject::DuObject(const DuObject &other)
{
    Q_UNUSED(other)
}

DuObject::~DuObject()
{
}


QByteArray DuObject::toMidiBinary() const
{
    Q_UNIMPLEMENTED();
    return QByteArray();
}


QHttpPart DuObject::toHttpPart(const QString &name) const
{
    Q_UNUSED(name)
    return QHttpPart();
}

QDebug DuObject::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuObject(" << (void*)this << ")";

    return dbg.space();
}