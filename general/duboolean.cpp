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

bool DuBoolean::getBoolean() const
{
    return getValue().toBool();
}

bool DuBoolean::setBoolean(bool value)
{
    setValue(value);
    return true;
}
