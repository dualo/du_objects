#include "dudevice.h"

#include <QDebug>

DU_OBJECT_IMPL(DuDevice)

DuDevice::DuDevice() :
    DuContainer(),
    plugged(false),
    connected(false),
    busy(false)
{
    addChild(KEY_DEVICE_SERIAL_NUMBER,  new DuString(""));
    addChild(KEY_DEVICE_NAME,           new DuString(""));
    addChild(KEY_DEVICE_OWNER,          new DuString(""));
    addChild(KEY_DEVICE_OWNER_ID,       new DuNumeric(0));

    addChild(KEY_DEVICE_VERSION,        new DuVersion());
    addChild(KEY_DEVICE_UPDATE_DATE,    new DuDate());
}

QHttpMultiPart *DuDevice::toHttpMultiPart(const QByteArray& boundary) const
{
    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    multiPart->setBoundary(boundary);

    multiPart->append(getChild(KEY_DEVICE_SERIAL_NUMBER)->toHttpPart("serial_number"));
    multiPart->append(getChild(KEY_DEVICE_NAME)->toHttpPart("name"));
    multiPart->append(getChild(KEY_DEVICE_VERSION)->toHttpPart("version"));
    multiPart->append(getChild(KEY_DEVICE_OWNER_ID)->toHttpPart("user_id"));

    return multiPart;
}

QString DuDevice::getSerialNumber() const
{
    const DuStringConstPtr& serialNumber = getChildAs<DuString>(KEY_DEVICE_SERIAL_NUMBER);

    if (serialNumber == NULL)
    {
        qCritical() << "Unable to cast" << KEY_DEVICE_SERIAL_NUMBER << "to DuString*";
        return QString();
    }

    return serialNumber->getString();
}

bool DuDevice::setSerialNumber(const QString &value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_DEVICE_SERIAL_NUMBER);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuDevice::getName() const
{
    const DuStringConstPtr& name = getChildAs<DuString>(KEY_DEVICE_NAME);

    if (name == NULL)
    {
        qCritical() << "Unable to cast" << KEY_DEVICE_NAME << "to DuString*";
        return QString();
    }

    return name->getString();
}

bool DuDevice::setName(const QString &value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_DEVICE_NAME);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuDevice::getDisplayName() const
{
    const QString& name = getName();
    return name.isEmpty() ? getSerialNumber() : name;
}

QString DuDevice::getOwner() const
{
    const DuStringConstPtr& owner = getChildAs<DuString>(KEY_DEVICE_OWNER);

    if (owner == NULL)
    {
        qCritical() << "Unable to cast" << KEY_DEVICE_OWNER << "to DuString*";
        return QString();
    }

    return owner->getString();
}

bool DuDevice::setOwner(const QString &value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_DEVICE_OWNER);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

int DuDevice::getOwnerId() const
{
    const DuNumericConstPtr& ownerId = getChildAs<DuNumeric>(KEY_DEVICE_OWNER_ID);

    if (ownerId == NULL)
    {
        qCritical() << "Unable to cast" << KEY_DEVICE_OWNER_ID << "to DuNumeric*";
        return -1;
    }

    return ownerId->getNumeric();
}

bool DuDevice::setOwnerId(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_DEVICE_OWNER_ID);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

DuVersionConstPtr DuDevice::getVersion() const
{
    const DuVersionConstPtr& version = getChildAs<DuVersion>(KEY_DEVICE_VERSION);

    if (version == NULL)
    {
        qCritical() << "Unable to cast" << KEY_DEVICE_VERSION << "to DuVersion*";
        return DuVersionConstPtr();
    }

    return version;
}

bool DuDevice::setVersion(const QString &value)
{
    DuVersionPtr tmp = getChildAs<DuVersion>(KEY_DEVICE_VERSION);

    if (tmp == NULL)
        return false;

    return tmp->setVersion(value);
}

bool DuDevice::setVersion(int major, int minor, int patch)
{
    DuVersionPtr tmp = getChildAs<DuVersion>(KEY_DEVICE_VERSION);

    if (tmp == NULL)
        return false;

    return tmp->setVersion(major, minor, patch);
}

QDateTime DuDevice::getUpdateDate() const
{
    const DuDateConstPtr& updateDate = getChildAs<DuDate>(KEY_DEVICE_UPDATE_DATE);

    if (updateDate == NULL)
    {
        qCritical() << "Unable to cast" << KEY_DEVICE_UPDATE_DATE << "to DuDate*";
        return QDateTime();
    }

    return updateDate->getDate();
}

bool DuDevice::setUpdateDate(const QDateTime &value)
{
    DuDatePtr tmp = getChildAs<DuDate>(KEY_DEVICE_UPDATE_DATE);

    if (tmp == NULL)
        return false;

    tmp->setDate(value);
    return true;
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
