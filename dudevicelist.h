#ifndef DUDEVICELIST_H
#define DUDEVICELIST_H

#include "dulist.h"
#include "dudevice.h"

DU_OBJECT(DuDeviceList);

class DuDeviceList : public DuList
{
public:
    explicit DuDeviceList(const DuDeviceConstPtr &device);
    virtual ~DuDeviceList() = default;

    virtual DuObjectPtr clone() const Q_DECL_OVERRIDE;

    bool hasSameAttributes(const DuListConstPtr& other) const Q_DECL_OVERRIDE;

    bool setType(Type value);

    int getNumButtonKeyboard() const;

    DU_KEY_ACCESSORS_OBJECT(Device, DuDevice)

    DU_KEY_ACCESSORS_IN_CHILD(DeviceType,   DuDevice::Type)
    DU_KEY_ACCESSORS_IN_CHILD(SerialNumber, QString)
    DU_KEY_ACCESSORS_IN_CHILD(Connected, bool)
    DU_KEY_ACCESSORS_IN_CHILD(Busy, bool)
};

#endif // DUDEVICELIST_H
