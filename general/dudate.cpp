#include "dudate.h"

#include <QJsonValue>



DU_OBJECT_IMPL(DuDate)

DuDate::DuDate(const QDateTime &date) :
    DuValue(date)
{
}

DuObjectPtr DuDate::clone() const
{
    return DuDatePtr(new DuDate(*this));
}


QByteArray DuDate::toDuMusicBinary() const
{
    Q_UNIMPLEMENTED();
    return QByteArray();
}

QJsonValue DuDate::toJson() const
{
    return QJsonValue(getDate().toString());
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
