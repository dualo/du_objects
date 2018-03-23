#include "dudevice.h"

#include "general/duboolean.h"
#include "general/dudate.h"
#include "general/dunumeric.h"
#include "general/dustring.h"
#include "general/duversion.h"


DU_OBJECT_IMPL(DuDevice)

QString DuDevice::typeToString(DuDevice::Type type)
{
    switch (type)
    {
        case Type_Unknown:
            return QStringLiteral("Unknown");
        case Type_DuTouchPro:
            return QStringLiteral("du-touch Pro");
        case Type_DuTouchS:
            return QStringLiteral("du-touch S");
        case Type_DuTouchL:
            return QStringLiteral("du-touch L");
        case Type_Other:
            return QStringLiteral("Other");
    }

    Q_UNREACHABLE();
    return QString();
}

DuDevice::DuDevice(Type type) :
    DuContainer(),
    plugged(false),
    connected(false),
    busy(false),
    isInFlashMeState(false)
{
    addChild(KeyType,            new DuNumeric(type));
    addChild(KeySerialNumber,    new DuString);
    addChild(KeyName,            new DuString);
    addChild(KeyOwner,           new DuString);
    addChild(KeyOwnerId,         new DuNumeric(0));
    addChild(KeyDissocScheduled, new DuBoolean(false));

    addChild(KeyVersion,         new DuVersion());
    addChild(KeyUpdateDate,      new DuDate());
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
    multiPart->append(getChild(KeyOwnerId)->toHttpPart(QStringLiteral("user")));

    return multiPart;
}

QString DuDevice::getDisplayName() const
{
    const QString& name = getName();
    return name.isEmpty() ? getSerialNumber() : name;
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

bool DuDevice::setConnected(bool value)
{
    connected = value;
    return true;
}

bool DuDevice::getBusy() const
{
    return busy;
}

bool DuDevice::setBusy(bool value)
{
    busy = value;
    return true;
}

bool DuDevice::getIsInFlashMeState() const
{
    return isInFlashMeState;
}

void DuDevice::setIsInFlashMeState(bool value)
{
    isInFlashMeState = value;
}

DU_KEY_ACCESSORS_IMPL(DuDevice, Type,            Numeric, DuDevice::Type, Type_Unknown)
DU_KEY_ACCESSORS_IMPL(DuDevice, SerialNumber,    String, QString, QString())
DU_KEY_ACCESSORS_IMPL(DuDevice, Name,            String, QString, QString())
DU_KEY_ACCESSORS_IMPL(DuDevice, Owner,           String, QString, QString())
DU_KEY_ACCESSORS_IMPL(DuDevice, OwnerId,         Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuDevice, DissocScheduled, Boolean, bool, false)
DU_KEY_ACCESSORS_IMPL(DuDevice, Version,         Version, Version, Version())
DU_KEY_ACCESSORS_IMPL(DuDevice, UpdateDate,      Date, QDateTime, QDateTime())
