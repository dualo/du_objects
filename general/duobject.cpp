#include "duobject.h"

Q_LOGGING_CATEGORY(LOG_CAT_DU_OBJECT, "du-object")
Q_LOGGING_CATEGORY(LOG_CAT_MIDI, "midi")

DU_OBJECT_IMPL(DuObject)


DuObject::DuObject()
{
}

DuObject::DuObject(const DuObject &other)
{
    Q_UNUSED(other)
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
    dbg.nospace() << "DuObject(" << reinterpret_cast<const void*>(this) << ")";

    return dbg.space();
}

bool operator==(const DuObject &lhs, const DuObject &rhs)
{
    return lhs.equals(rhs);
}

bool operator!=(const DuObject &lhs, const DuObject &rhs)
{
    return !(lhs == rhs);
}

bool DuObject::equals(const DuObject &other) const
{
    return typeid(*this) == typeid(other);
}
