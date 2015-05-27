#include "dudate.h"

#include <QDebug>

DU_OBJECT_IMPL(DuDate)

DuDate::DuDate(const QDateTime &date) :
    DuValue(date)
{
}

DuObjectPtr DuDate::clone() const
{
    return DuDatePtr(new DuDate(*this));
}

QJsonValue DuDate::toJson() const
{
    return QJsonValue(getDate().toString());
}

QByteArray DuDate::toDuMusicFile() const
{
    Q_UNIMPLEMENTED();
    return QByteArray();
}

QDebug DuDate::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuDate(";
    dbg.nospace() << getDate();
    dbg.nospace() << ")";

    return dbg.space();
}

QDateTime DuDate::getDate() const
{
    return getValue().toDateTime();
}

void DuDate::setDate(const QDateTime &date)
{
    setValue(date);
}
