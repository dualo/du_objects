#ifndef DUDEVICELIST_H
#define DUDEVICELIST_H

#include "dudevice.h"
#include "dulist.h"


DU_OBJECT(DuDeviceList)

class DuDeviceList : public DuList
{
public:
    explicit DuDeviceList(const DuDeviceConstPtr &device);

    virtual DuObjectPtr clone() const;

    bool equals(const DuListConstPtr& other) const;

    bool getConnected() const;
    void setConnected(bool value);

    bool getBusy() const;
    void setBusy(bool value);

    bool setType(Type value);

    DU_KEY_ACCESSORS(SerialNumber, QString)

private:
    bool connected;
    bool busy;
};

#endif // DUDEVICELIST_H
