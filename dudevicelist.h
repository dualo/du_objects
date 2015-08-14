#ifndef DUDEVICELIST_H
#define DUDEVICELIST_H

#include "dulist.h"

#define KEY_DEVICE_LIST_SERIAL_NUMBER "SerialNumber"


DU_OBJECT(DuDevice)

DU_OBJECT(DuDeviceList)

class DuDeviceList : public DuList
{
public:
    DuDeviceList(const DuDeviceConstPtr &device);

    virtual DuObjectPtr clone() const;

    bool equals(const DuListConstPtr& other) const;

    QString getSerialNumber() const;
    bool setSerialNumber(const QString& value);

    bool getConnected() const;
    void setConnected(bool value);

    bool getBusy() const;
    void setBusy(bool value);

    bool setType(Type value);

private:
    bool connected;
    bool busy;
};

#endif // DUDEVICELIST_H
