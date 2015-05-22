#include "dudevice.h"

#include <QDebug>

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

    multiPart->append(getSerialNumberObject()->toHttpPart("serial_number"));
    multiPart->append(getNameObject()->toHttpPart("name"));
    multiPart->append(getVersionObject()->toHttpPart("version"));
    multiPart->append(getOwnerIdObject()->toHttpPart("user_id"));

    return multiPart;
}

QString DuDevice::getSerialNumber() const
{
    const QSharedPointer<DuString>& serialNumber = getSerialNumberObject();

    if (serialNumber == NULL)
    {
        qCritical() << "Unable to cast" << KEY_DEVICE_SERIAL_NUMBER << "to DuString*";
        return QString();
    }

    return serialNumber->getString();
}

bool DuDevice::setSerialNumber(const QString &value)
{
    QSharedPointer<DuString> tmp = getChildAs<DuString>(KEY_DEVICE_SERIAL_NUMBER);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuDevice::getName() const
{
    const QSharedPointer<DuString>& name = getNameObject();

    if (name == NULL)
    {
        qCritical() << "Unable to cast" << KEY_DEVICE_NAME << "to DuString*";
        return QString();
    }

    return name->getString();
}

bool DuDevice::setName(const QString &value)
{
    QSharedPointer<DuString> tmp = getChildAs<DuString>(KEY_DEVICE_NAME);

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
    const QSharedPointer<DuString>& owner = getOwnerObject();

    if (owner == NULL)
    {
        qCritical() << "Unable to cast" << KEY_DEVICE_OWNER << "to DuString*";
        return QString();
    }

    return owner->getString();
}

bool DuDevice::setOwner(const QString &value)
{
    QSharedPointer<DuString> tmp = getChildAs<DuString>(KEY_DEVICE_OWNER);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

int DuDevice::getOwnerId() const
{
    const QSharedPointer<DuNumeric>& ownerId = getOwnerIdObject();

    if (ownerId == NULL)
    {
        qCritical() << "Unable to cast" << KEY_DEVICE_OWNER_ID << "to DuNumeric*";
        return -1;
    }

    return ownerId->getNumeric();
}

bool DuDevice::setOwnerId(int value)
{
    QSharedPointer<DuNumeric> tmp = getChildAs<DuNumeric>(KEY_DEVICE_OWNER_ID);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

const QSharedPointer<DuVersion> DuDevice::getVersion() const
{
    const QSharedPointer<DuVersion>& version = getVersionObject();

    if (version == NULL)
    {
        qCritical() << "Unable to cast" << KEY_DEVICE_VERSION << "to DuVersion*";
        return QSharedPointer<DuVersion>();
    }

    return version;
}

bool DuDevice::setVersion(const QString &value)
{
    QSharedPointer<DuVersion> tmp = getChildAs<DuVersion>(KEY_DEVICE_VERSION);

    if (tmp == NULL)
        return false;

    return tmp->setVersion(value);
}

bool DuDevice::setVersion(int major, int minor, int patch)
{
    QSharedPointer<DuVersion> tmp = getChildAs<DuVersion>(KEY_DEVICE_VERSION);

    if (tmp == NULL)
        return false;

    return tmp->setVersion(major, minor, patch);
}

QDateTime DuDevice::getUpdateDate() const
{
    const QSharedPointer<DuDate>& updateDate = getUpdateDateObject();

    if (updateDate == NULL)
    {
        qCritical() << "Unable to cast" << KEY_DEVICE_UPDATE_DATE << "to DuDate*";
        return QDateTime();
    }

    return updateDate->getDate();
}

bool DuDevice::setUpdateDate(const QDateTime &value)
{
    QSharedPointer<DuDate> tmp = getChildAs<DuDate>(KEY_DEVICE_UPDATE_DATE);

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

const QSharedPointer<DuString> DuDevice::getSerialNumberObject() const
{
    return getChildAs<DuString>(KEY_DEVICE_SERIAL_NUMBER);
}

const QSharedPointer<DuString> DuDevice::getNameObject() const
{
    return getChildAs<DuString>(KEY_DEVICE_NAME);
}

const QSharedPointer<DuString> DuDevice::getOwnerObject() const
{
    return getChildAs<DuString>(KEY_DEVICE_OWNER);
}

const QSharedPointer<DuNumeric> DuDevice::getOwnerIdObject() const
{
    return getChildAs<DuNumeric>(KEY_DEVICE_OWNER_ID);
}

const QSharedPointer<DuVersion> DuDevice::getVersionObject() const
{
    return getChildAs<DuVersion>(KEY_DEVICE_VERSION);
}

const QSharedPointer<DuDate> DuDevice::getUpdateDateObject() const
{
    return getChildAs<DuDate>(KEY_DEVICE_UPDATE_DATE);
}
