#include "duurl.h"

#include <QJsonValue>



DU_OBJECT_IMPL(DuUrl)

DuUrl::DuUrl(const QUrl &url) :
    DuValue(url)
{
}


DuObjectPtr DuUrl::clone() const
{
    return DuUrlPtr(new DuUrl(*this));
}


QByteArray DuUrl::toDuMusicBinary() const
{
    Q_UNIMPLEMENTED();
    return QByteArray();
}

QJsonValue DuUrl::toJson() const
{
    return QJsonValue(getUrl().toString());
}


QDebug DuUrl::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuUrl(";
    dbg.nospace() << getUrl();
    dbg.nospace() << ")";

    return dbg.space();
}

QVariant DuUrl::checkValue(const QVariant &value, bool &success)
{
    if (!value.canConvert<QUrl>())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "value is not of type QUrl:" << value;
        success = false;
        return QVariant();
    }

    success = true;
    return value;
}

QUrl DuUrl::getUrl() const
{
    return getValue().toUrl();
}

bool DuUrl::setUrl(const QUrl &url)
{
    return setValue(url);
}
