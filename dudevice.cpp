#include "dudevice.h"


DU_OBJECT_IMPL(DuDevice)

DuDevice::DuDevice() :
    DuContainer(),
    plugged(false),
    connected(false),
    busy(false)
{
    addChild(KeySerialNumber,  new DuString);
    addChild(KeyName,          new DuString);
    addChild(KeyOwner,         new DuString);
    addChild(KeyOwnerId,       new DuNumeric(0));

    addChild(KeyVersion,       new DuVersion());
    addChild(KeyUpdateDate,    new DuDate());
}

DuObjectPtr DuDevice::clone() const
{
    return DuDevicePtr(new DuDevice(*this));
}

QHttpMultiPart *DuDevice::toHttpMultiPart(const QByteArray& boundary) const
{
    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    multiPart->setBoundary(boundary);

    multiPart->append(getChild(KeySerialNumber)->toHttpPart(QStringLiteral("serial_number")));
    multiPart->append(getChild(KeyName)->toHttpPart(QStringLiteral("name")));
    multiPart->append(getChild(KeyVersion)->toHttpPart(QStringLiteral("version")));
    multiPart->append(getChild(KeyOwnerId)->toHttpPart(QStringLiteral("user_id")));

    return multiPart;
}

QString DuDevice::getDisplayName() const
{
    const QString& name = getName();
    return name.isEmpty() ? getSerialNumber() : name;
}

bool DuDevice::setVersion(const QString &value)
{
    DuVersionPtr tmp = getVersion();

    if (tmp == NULL)
        return false;

    return tmp->setVersion(value);
}

bool DuDevice::setVersion(int major, int minor, int patch)
{
    DuVersionPtr tmp = getVersion();

    if (tmp == NULL)
        return false;

    return tmp->setVersion(major, minor, patch);
}

bool DuDevice::getPlugged() const
{
    return plugged;
}

void DuDevice::setPlugged(bool value)
{
    plugged = value;
}

bool DuDevice::getConnected() const
{
    return connected;
}

void DuDevice::setConnected(bool value)
{
    connected = value;
}

bool DuDevice::getBusy() const
{
    return busy;
}

void DuDevice::setBusy(bool value)
{
    busy = value;
}

DU_KEY_ACCESSORS_IMPL(DuDevice, SerialNumber, String, QString, QString())
DU_KEY_ACCESSORS_IMPL(DuDevice, Name,         String, QString, QString())
DU_KEY_ACCESSORS_IMPL(DuDevice, Owner,        String, QString, QString())
DU_KEY_ACCESSORS_IMPL(DuDevice, OwnerId,      Numeric, int, -1)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuDevice, Version, DuVersion)

DU_KEY_ACCESSORS_IMPL(DuDevice, UpdateDate, Date, QDateTime, QDateTime())
