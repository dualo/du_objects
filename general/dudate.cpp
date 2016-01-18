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

QVariant DuDate::checkValue(const QVariant &value, bool &success)
{
    if (!value.canConvert<QDateTime>())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "value is not of type QDateTime:" << value;
        success = false;
        return QVariant();
    }

    success = true;
    return value;
}

QDateTime DuDate::getDate() const
{
    return getValue().toDateTime();
}

bool DuDate::setDate(const QDateTime &date)
{
    return setValue(date);
}
