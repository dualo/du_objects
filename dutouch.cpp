#include "dutouch.h"

#include "general/dudate.h"
#include "general/duversion.h"


DU_OBJECT_IMPL(DuTouch)

DuTouch::DuTouch() :
    DuDevice()
{
    addChild(KeySoundbankVersion,    new DuVersion());
    addChild(KeySoundbankUpdateDate, new DuDate());
}

DuObjectPtr DuTouch::clone() const
{
    return DuTouchPtr(new DuTouch(*this));
}

QHttpMultiPart *DuTouch::toHttpMultiPart(const QByteArray& boundary) const
{
    QHttpMultiPart* multiPart = DuDevice::toHttpMultiPart(boundary);

    multiPart->append(getChild(KeySoundbankVersion)->toHttpPart(QStringLiteral("soundbank_version")));

    return multiPart;
}

bool DuTouch::setSoundbankVersion(const QString &value)
{
    DuVersionPtr tmp = getSoundbankVersion();

    if (tmp == NULL)
        return false;

    return tmp->setVersion(value);
}

bool DuTouch::setSoundbankVersion(int major, int minor, int patch)
{
    DuVersionPtr tmp = getSoundbankVersion();

    if (tmp == NULL)
        return false;

    return tmp->setVersion(major, minor, patch);
}

DU_KEY_ACCESSORS_OBJECT_IMPL(DuTouch, SoundbankVersion, DuVersion)
DU_KEY_ACCESSORS_IMPL(DuTouch, SoundbankUpdateDate, Date, QDateTime, QDateTime())
