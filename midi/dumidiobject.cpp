#include "dumidiobject.h"

#include <QDebug>

DU_OBJECT_IMPL(DuMidiObject)


DuMidiObject::DuMidiObject() :
    DuObject()
{
}

DuMidiObject::~DuMidiObject()
{
}

DuMidiObject::DuMidiObject(const DuMidiObject &other) :
    DuObject(other)
{
}


QByteArray DuMidiObject::toDuMusicFile() const
{
    return QByteArray();
}

QJsonValue DuMidiObject::toJson() const
{
    return QJsonValue();
}


QDebug DuMidiObject::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuMidiObject(" << (void*)this << ")";

    return dbg.space();
}
