#include "duboolean.h"

#include <QJsonValue>

DuBoolean::DuBoolean(bool value) :
    DuValue(value, 1)
{
}

DuObjectPtr DuBoolean::clone() const
{
    return DuBooleanPtr(new DuBoolean(*this));
}

QByteArray DuBoolean::toDuMusicBinary() const
{
    return QByteArray::number(getBoolean());
}

QByteArray DuBoolean::toMidiBinary() const
{
    return QByteArray::number(getBoolean());
}

QJsonValue DuBoolean::toJson() const
{
    return QJsonValue(getBoolean());
}

QHttpPart DuBoolean::toHttpPart(const QString &name) const
{
    QHttpPart part;
    part.setHeader(QNetworkRequest::ContentDispositionHeader,
                   "form-data; name=\"" + name + "\"");
    part.setBody(QByteArray::number(getBoolean()));

    return part;
}

QDebug DuBoolean::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuBoolean(" << getBoolean() << ")";

    return dbg.space();
}

QVariant DuBoolean::checkValue(const QVariant &value, bool &success)
{
    if (!value.canConvert<bool>())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "value is not of type bool:" << value;
        success = false;
        return QVariant();
    }

    success = true;
    return value;
}

bool DuBoolean::getBoolean() const
{
    return getValue().toBool();
}

bool DuBoolean::setBoolean(bool value)
{
    return setValue(value);
}
