#include "duversion.h"

#include <QJsonValue>


DU_OBJECT_IMPL(DuVersion)


DuVersion::DuVersion(const Version &version) :
    DuValue()
{
    setVersion(version);
}

DuVersion::DuVersion() :
    DuValue()
{
}

int DuVersion::major() const
{
    return getVersion().major;
}

int DuVersion::minor() const
{
    return getVersion().minor;
}

int DuVersion::patch() const
{
    return getVersion().patch;
}

Version DuVersion::getVersion() const
{
    return getValue().value<Version>();
}

bool DuVersion::setVersion(const Version &version)
{
    return setValue(QVariant::fromValue(version));
}

DuObjectPtr DuVersion::clone() const
{
    return DuVersionPtr(new DuVersion(*this));
}

QByteArray DuVersion::toDuMusicBinary() const
{
    return getVersion().toString().toLatin1();
}

QByteArray DuVersion::toMidiBinary() const
{
    return getVersion().toString().toLocal8Bit();
}

QJsonValue DuVersion::toJson() const
{
    return QJsonValue(getVersion().toString());
}

QHttpPart DuVersion::toHttpPart(const QString &name) const
{
    QHttpPart part;
    part.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"" + name + "\"");
    part.setBody(getVersion().toString().toUtf8());

    return part;
}

QDebug DuVersion::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuVersion(" << getVersion() << ")";

    return dbg.space();
}

QVariant DuVersion::checkValue(const QVariant &value, bool &success)
{
    if (!value.canConvert<Version>())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "value is not of type Version:" << value;
        success = false;
        return QVariant();
    }

    success = true;
    return value;
}
