#include "dudevicemusiclist.h"

#include "dustring.h"

#include <QDebug>

DuDeviceMusicList::DuDeviceMusicList(const DuDevice *device) :
    DuMusicList(device->getDisplayName(), Device),
    connected(device->getConnected()),
    busy(device->getBusy())
{
    addChild(KEY_DEVICE_MUSIC_LIST_SERIAL_NUMBER, new DuString(device->getSerialNumber()));
}

bool DuDeviceMusicList::equals(const DuMusicList *other) const
{
    if (other->getType() != Device)
    {
        return false;
    }

    const DuDeviceMusicList* casted = dynamic_cast<const DuDeviceMusicList*>(other);
    if (casted == NULL)
    {
        return false;
    }

    return this->getSerialNumber() == casted->getSerialNumber();
}

QString DuDeviceMusicList::getSerialNumber() const
{
    const DuString* serialNumber = getChildAs<DuString*>(KEY_DEVICE_MUSIC_LIST_SERIAL_NUMBER);

    if (serialNumber == NULL)
    {
        qCritical() << "Unable to cast" << KEY_DEVICE_MUSIC_LIST_SERIAL_NUMBER << "to DuString*";
        return QString();
    }

    return serialNumber->getString();
}

bool DuDeviceMusicList::setSerialNumber(const QString &value)
{
    DuString *tmp = getChildAs<DuString*>(KEY_DEVICE_MUSIC_LIST_SERIAL_NUMBER);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

bool DuDeviceMusicList::getConnected() const
{
    return connected;
}

void DuDeviceMusicList::setConnected(bool value)
{
    connected = value;
}

bool DuDeviceMusicList::getBusy() const
{
    return busy;
}

void DuDeviceMusicList::setBusy(bool value)
{
    busy = value;
}

bool DuDeviceMusicList::setType(Type value)
{
    Q_UNREACHABLE();
    Q_UNUSED(value)
    return false;
}
