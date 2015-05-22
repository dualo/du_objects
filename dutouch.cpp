#include "dutouch.h"

#include <QDebug>

DuTouch::DuTouch() :
    DuDevice()
{
    addChild(KEY_TOUCH_SOUNDBANK_VERSION,       new DuVersion());
    addChild(KEY_TOUCH_SOUNDBANK_UPDATE_DATE,   new DuDate());
}

QHttpMultiPart *DuTouch::toHttpMultiPart(const QByteArray& boundary) const
{
    QHttpMultiPart* multiPart = DuDevice::toHttpMultiPart(boundary);

    multiPart->append(getSoundbankVersionObject()->toHttpPart("soundbank_version"));

    return multiPart;
}

const QSharedPointer<DuVersion> DuTouch::getSoundbankVersion() const
{
    const QSharedPointer<DuVersion>& version = getSoundbankVersionObject();

    if (version == NULL)
    {
        qCritical() << "Unable to cast" << KEY_TOUCH_SOUNDBANK_VERSION << "to DuVersion*";
        return QSharedPointer<DuVersion>();
    }

    return version;
}

bool DuTouch::setSoundbankVersion(const QString &value)
{
    QSharedPointer<DuVersion> tmp = getChildAs<DuVersion>(KEY_TOUCH_SOUNDBANK_VERSION);

    if (tmp == NULL)
        return false;

    return tmp->setVersion(value);
}

bool DuTouch::setSoundbankVersion(int major, int minor, int patch)
{
    QSharedPointer<DuVersion> tmp = getChildAs<DuVersion>(KEY_TOUCH_SOUNDBANK_VERSION);

    if (tmp == NULL)
        return false;

    return tmp->setVersion(major, minor, patch);
}

QDateTime DuTouch::getSoundbankUpdateDate() const
{
    const QSharedPointer<DuDate>& updateDate = getSoundbankUpdateDateObject();

    if (updateDate == NULL)
    {
        qCritical() << "Unable to cast" << KEY_TOUCH_SOUNDBANK_UPDATE_DATE << "to DuDate*";
        return QDateTime();
    }

    return updateDate->getDate();
}

bool DuTouch::setSoundbankUpdateDate(const QDateTime &value)
{
    QSharedPointer<DuDate> tmp = getChildAs<DuDate>(KEY_TOUCH_SOUNDBANK_UPDATE_DATE);

    if (tmp == NULL)
        return false;

    tmp->setDate(value);
    return true;
}

const QSharedPointer<DuVersion> DuTouch::getSoundbankVersionObject() const
{
    return getChildAs<DuVersion>(KEY_TOUCH_SOUNDBANK_VERSION);
}

const QSharedPointer<DuDate> DuTouch::getSoundbankUpdateDateObject() const
{
    return getChildAs<DuDate>(KEY_TOUCH_SOUNDBANK_UPDATE_DATE);
}
