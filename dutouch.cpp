#include "dutouch.h"

#include "general/dudate.h"
#include "general/duversion.h"


DU_OBJECT_IMPL(DuTouch)

DuTouch::DuTouch() :
    DuDevice()
{
    addChild(KEY_TOUCH_SOUNDBANK_VERSION,       new DuVersion());
    addChild(KEY_TOUCH_SOUNDBANK_UPDATE_DATE,   new DuDate());
}

DuObjectPtr DuTouch::clone() const
{
    return DuTouchPtr(new DuTouch(*this));
}

QHttpMultiPart *DuTouch::toHttpMultiPart(const QByteArray& boundary) const
{
    QHttpMultiPart* multiPart = DuDevice::toHttpMultiPart(boundary);

    multiPart->append(getChild(KEY_TOUCH_SOUNDBANK_VERSION)->toHttpPart("soundbank_version"));

    return multiPart;
}

DuVersionConstPtr DuTouch::getSoundbankVersion() const
{
    const DuVersionConstPtr& version = getChildAs<DuVersion>(KEY_TOUCH_SOUNDBANK_VERSION);

    if (version == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Unable to cast" << KEY_TOUCH_SOUNDBANK_VERSION << "to DuVersion*";
        return DuVersionConstPtr();
    }

    return version;
}

bool DuTouch::setSoundbankVersion(const QString &value)
{
    DuVersionPtr tmp = getChildAs<DuVersion>(KEY_TOUCH_SOUNDBANK_VERSION);

    if (tmp == NULL)
        return false;

    return tmp->setVersion(value);
}

bool DuTouch::setSoundbankVersion(int major, int minor, int patch)
{
    DuVersionPtr tmp = getChildAs<DuVersion>(KEY_TOUCH_SOUNDBANK_VERSION);

    if (tmp == NULL)
        return false;

    return tmp->setVersion(major, minor, patch);
}

QDateTime DuTouch::getSoundbankUpdateDate() const
{
    const DuDateConstPtr& updateDate = getChildAs<DuDate>(KEY_TOUCH_SOUNDBANK_UPDATE_DATE);

    if (updateDate == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Unable to cast" << KEY_TOUCH_SOUNDBANK_UPDATE_DATE << "to DuDate*";
        return QDateTime();
    }

    return updateDate->getDate();
}

bool DuTouch::setSoundbankUpdateDate(const QDateTime &value)
{
    DuDatePtr tmp = getChildAs<DuDate>(KEY_TOUCH_SOUNDBANK_UPDATE_DATE);

    if (tmp == NULL)
        return false;

    tmp->setDate(value);
    return true;
}
