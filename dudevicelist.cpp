#include "dudevicelist.h"

#include "dudevice.h"

#include "general/dustring.h"


DU_OBJECT_IMPL(DuDeviceList)

DuDeviceList::DuDeviceList(const DuDeviceConstPtr &device) :
    DuList(device->getDisplayName(), Device)
{
    addChild(KeyDevice, device->cloneAs<DuDevice>());
}

DuObjectPtr DuDeviceList::clone() const
{
    return DuDeviceListPtr(new DuDeviceList(*this));
}

bool DuDeviceList::hasSameAttributes(const DuListConstPtr& other) const
{
    if (other->getType() != Device)
    {
        return false;
    }

    const DuDeviceListConstPtr& casted = other.dynamicCast<const DuDeviceList>();
    if (casted == Q_NULLPTR)
    {
        return false;
    }

    return this->getSerialNumber() == casted->getSerialNumber();
}

bool DuDeviceList::setType(Type value)
{
    Q_ASSERT_X(false, "setType", "unreachable");
    Q_UNUSED(value)
    return false;
}

DU_KEY_ACCESSORS_OBJECT_IMPL(DuDeviceList, Device, DuDevice)

DU_KEY_ACCESSORS_IN_CHILD_IMPL_DIFF_KEYS(DuDeviceList, DeviceType, Type, DuDevice, Device, DuDevice::Type, DuDevice::Type_Unknown)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuDeviceList, SerialNumber, DuDevice, Device, QString, QString())
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuDeviceList, Connected, DuDevice, Device, bool, false)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuDeviceList, Busy, DuDevice, Device, bool, false)
