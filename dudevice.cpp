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
    const DuString* serialNumber = getSerialNumberObject();

    if (serialNumber == NULL)
    {
        qCritical() << "Unable to cast" << KEY_DEVICE_SERIAL_NUMBER << "to DuString*";
        return QString();
    }

    return serialNumber->getString();
}

bool DuDevice::setSerialNumber(const QString &value)
{
    DuString *tmp = getChildAs<DuString*>(KEY_DEVICE_SERIAL_NUMBER);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuDevice::getName() const
{
    const DuString* name = getNameObject();

    if (name == NULL)
    {
        qCritical() << "Unable to cast" << KEY_DEVICE_NAME << "to DuString*";
        return QString();
    }

    return name->getString();
}

bool DuDevice::setName(const QString &value)
{
    DuString *tmp = getChildAs<DuString*>(KEY_DEVICE_NAME);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuDevice::getOwner() const
{
    const DuString* owner = getOwnerObject();

    if (owner == NULL)
    {
        qCritical() << "Unable to cast" << KEY_DEVICE_OWNER << "to DuString*";
        return QString();
    }

    return owner->getString();
}

bool DuDevice::setOwner(const QString &value)
{
    DuString *tmp = getChildAs<DuString*>(KEY_DEVICE_OWNER);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

int DuDevice::getOwnerId() const
{
    const DuNumeric* ownerId = getOwnerIdObject();

    if (ownerId == NULL)
    {
        qCritical() << "Unable to cast" << KEY_DEVICE_OWNER_ID << "to DuNumeric*";
        return -1;
    }

    return ownerId->getNumeric();
}

bool DuDevice::setOwnerId(int value)
{
    DuNumeric *tmp = getChildAs<DuNumeric*>(KEY_DEVICE_OWNER_ID);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

const DuVersion *DuDevice::getVersion() const
{
    const DuVersion* version = getVersionObject();

    if (version == NULL)
    {
        qCritical() << "Unable to cast" << KEY_DEVICE_VERSION << "to DuVersion*";
        return NULL;
    }

    return version;
}

bool DuDevice::setVersion(const QString &value)
{
    DuVersion *tmp = getChildAs<DuVersion*>(KEY_DEVICE_VERSION);

    if (tmp == NULL)
        return false;

    return tmp->setVersion(value);
}

bool DuDevice::setVersion(int major, int minor, int patch)
{
    DuVersion *tmp = getChildAs<DuVersion*>(KEY_DEVICE_VERSION);

    if (tmp == NULL)
        return false;

    return tmp->setVersion(major, minor, patch);
}

QDateTime DuDevice::getUpdateDate() const
{
    const DuDate* updateDate = getUpdateDateObject();

    if (updateDate == NULL)
    {
        qCritical() << "Unable to cast" << KEY_DEVICE_UPDATE_DATE << "to DuDate*";
        return QDateTime();
    }

    return updateDate->getDate();
}

bool DuDevice::setUpdateDate(const QDateTime &value)
{
    DuDate *tmp = getChildAs<DuDate*>(KEY_DEVICE_UPDATE_DATE);

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

const DuString *DuDevice::getSerialNumberObject() const
{
    return getChildAs<DuString*>(KEY_DEVICE_SERIAL_NUMBER);
}

const DuString *DuDevice::getNameObject() const
{
    return getChildAs<DuString*>(KEY_DEVICE_NAME);
}

const DuString *DuDevice::getOwnerObject() const
{
    return getChildAs<DuString*>(KEY_DEVICE_OWNER);
}

const DuNumeric *DuDevice::getOwnerIdObject() const
{
    return getChildAs<DuNumeric*>(KEY_DEVICE_OWNER_ID);
}

const DuVersion *DuDevice::getVersionObject() const
{
    return getChildAs<DuVersion*>(KEY_DEVICE_VERSION);
}

const DuDate *DuDevice::getUpdateDateObject() const
{
    return getChildAs<DuDate*>(KEY_DEVICE_UPDATE_DATE);
}
