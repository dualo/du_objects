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

const DuVersion* DuTouch::getSoundbankVersion() const
{
    const DuVersion* version = getSoundbankVersionObject();

    if (version == NULL)
    {
        qCritical() << "Unable to cast" << KEY_TOUCH_SOUNDBANK_VERSION << "to DuVersion*";
        return NULL;
    }

    return version;
}

bool DuTouch::setSoundbankVersion(const QString &value)
{
    DuVersion *tmp = dynamic_cast<DuVersion*>(getChild(KEY_TOUCH_SOUNDBANK_VERSION));

    if (tmp == NULL)
        return false;

    return tmp->setVersion(value);
}

bool DuTouch::setSoundbankVersion(int major, int minor, int patch)
{
    DuVersion *tmp = dynamic_cast<DuVersion*>(getChild(KEY_TOUCH_SOUNDBANK_VERSION));

    if (tmp == NULL)
        return false;

    return tmp->setVersion(major, minor, patch);
}

QDateTime DuTouch::getSoundbankUpdateDate() const
{
    const DuDate* updateDate = getSoundbankUpdateDateObject();

    if (updateDate == NULL)
    {
        qCritical() << "Unable to cast" << KEY_TOUCH_SOUNDBANK_UPDATE_DATE << "to DuDate*";
        return QDateTime();
    }

    return updateDate->getDate();
}

bool DuTouch::setSoundbankUpdateDate(const QDateTime &value)
{
    DuDate *tmp = dynamic_cast<DuDate*>(getChild(KEY_TOUCH_SOUNDBANK_UPDATE_DATE));

    if (tmp == NULL)
        return false;

    tmp->setDate(value);
    return true;
}

const DuVersion *DuTouch::getSoundbankVersionObject() const
{
    return dynamic_cast<DuVersion*>(getChild(KEY_TOUCH_SOUNDBANK_VERSION));
}

const DuDate *DuTouch::getSoundbankUpdateDateObject() const
{
    return dynamic_cast<DuDate*>(getChild(KEY_TOUCH_SOUNDBANK_UPDATE_DATE));
}
