#ifndef DUDEVICEMUSICLIST_H
#define DUDEVICEMUSICLIST_H

#include "dudevice.h"
#include "dumusiclist.h"

#define KEY_DEVICE_MUSIC_LIST_SERIAL_NUMBER "SerialNumber"

class DuDeviceMusicList : public DuMusicList
{
public:
    DuDeviceMusicList(const DuDevice* device);

    bool equals(const DuMusicList *other) const;

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

#endif // DUDEVICEMUSICLIST_H
