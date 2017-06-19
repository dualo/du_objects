#include "dutouch.h"

#include "general/dudate.h"
#include "general/duversion.h"


DU_OBJECT_IMPL(DuTouch)

DuTouch::DuTouch(Type type) :
    DuDevice(type)
{
    addChild(KeySoundbankVersion,    new DuVersion());
    addChild(KeySoundbankUpdateDate, new DuDate());
}

DuObjectPtr DuTouch::clone() const
{
    return DuTouchPtr(new DuTouch(*this));
}

DuTouchPtr DuTouch::fromJsonApi(const JsonApiResourceObject &jsonDuTouch)
{
    if (jsonDuTouch.isNull())
        return DuTouchPtr();

    // CHECK TYPE
    if (jsonDuTouch.type() != "dutouchs")
        return DuTouchPtr();

    // ATTRIBUTES
    const QJsonObject& dataObject = jsonDuTouch.attributes();

    const QJsonValue& serialValue               = dataObject.value(QStringLiteral("serial_number"));
    const QJsonValue& nameValue                 = dataObject.value(QStringLiteral("name"));
    const QJsonValue& firmwareValue             = dataObject.value(QStringLiteral("firmware_version"));
    const QJsonValue& firmwareUpdateDateValue   = dataObject.value(QStringLiteral("last_firmware_update"));
    const QJsonValue& soundbankValue            = dataObject.value(QStringLiteral("soundbank_version"));
    const QJsonValue& soundbankUpdateDateValue  = dataObject.value(QStringLiteral("last_soundbank_update"));
    const QJsonValue& dissocScheduledValue      = dataObject.value(QStringLiteral("dissociation_scheduled"));
    if (!serialValue.isString()
            || !nameValue.isString()
            || !dissocScheduledValue.isBool())
        return DuTouchPtr();

    const QString& serial               = serialValue.toString();
    const QString& name                 = nameValue.toString();
    const QString& firmware             = firmwareValue.toString();
    const QString& firmwareUpdateDate   = firmwareUpdateDateValue.toString();
    const QString& soundbank            = soundbankValue.toString();
    const QString& soundbankUpdateDate  = soundbankUpdateDateValue.toString();
    bool dissocScheduled      = dissocScheduledValue.toBool();

    Type type = Type_Unknown;
    if (serial.startsWith("DT"))
    {
        type = Type_DuTouchPro;
    }
    else if (serial.startsWith("DS"))
    {
        type = Type_DuTouchS;
    }
    else if (serial.startsWith("DL"))
    {
        type = Type_DuTouchL;
    }

    DuTouchPtr device(new DuTouch(type));
    device->setSerialNumber(serial);
    device->setName(name);
    device->setPlugged(false);
    device->setConnected(false);
    device->setBusy(false);
    device->setVersion(firmware);
    device->setUpdateDate(QDateTime::fromString(firmwareUpdateDate, QStringLiteral("yyyy-MM-dd HH:mm:ss")));
    device->setSoundbankVersion(soundbank);
    device->setSoundbankUpdateDate(QDateTime::fromString(soundbankUpdateDate, QStringLiteral("yyyy-MM-dd HH:mm:ss")));
    device->setDissocScheduled(dissocScheduled);

    return device;
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
