#include "dudevicelist.h"

#include "dudevice.h"

#include "general/dustring.h"


DU_OBJECT_IMPL(DuDeviceList)

DuDeviceList::DuDeviceList(const DuDeviceConstPtr& device) :
    DuList(device->getDisplayName(), Device),
    connected(device->getConnected()),
    busy(device->getBusy())
{
    addChild(KEY_DEVICE_LIST_SERIAL_NUMBER, new DuString(device->getSerialNumber()));
}

DuObjectPtr DuDeviceList::clone() const
{
    return DuDeviceListPtr(new DuDeviceList(*this));
}

bool DuDeviceList::equals(const DuListConstPtr& other) const
{
    if (other->getType() != Device)
    {
        return false;
    }

    const DuDeviceListConstPtr& casted = other.dynamicCast<const DuDeviceList>();
    if (casted == NULL)
    {
        return false;
    }

    return this->getSerialNumber() == casted->getSerialNumber();
}

QString DuDeviceList::getSerialNumber() const
{
    const DuStringConstPtr serialNumber = getChildAs<DuString>(KEY_DEVICE_LIST_SERIAL_NUMBER);

    if (serialNumber == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Unable to cast" << KEY_DEVICE_LIST_SERIAL_NUMBER << "to DuString*";
        return QString();
    }

    return serialNumber->getString();
}

bool DuDeviceList::setSerialNumber(const QString &value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_DEVICE_LIST_SERIAL_NUMBER);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

bool DuDeviceList::getConnected() const
{
    return connected;
}

void DuDeviceList::setConnected(bool value)
{
    connected = value;
}

bool DuDeviceList::getBusy() const
{
    return busy;
}

void DuDeviceList::setBusy(bool value)
{
    busy = value;
}

bool DuDeviceList::setType(Type value)
{
    Q_UNREACHABLE();
    Q_UNUSED(value)
    return false;
}
