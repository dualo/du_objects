#include "dudevicelist.h"

#include "dudevice.h"

#include "general/dustring.h"


DU_OBJECT_IMPL(DuDeviceList)

DuDeviceList::DuDeviceList(const DuDeviceConstPtr& device) :
    DuList(device->getDisplayName(), Device),
    connected(device->getConnected()),
    busy(device->getBusy())
{
    addChild(KeySerialNumber, new DuString(device->getSerialNumber()));
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

DU_KEY_ACCESSORS_IMPL(DuDeviceList, SerialNumber, String, QString, QString())
