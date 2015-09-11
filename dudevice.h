#ifndef DUDEVICE_H
#define DUDEVICE_H

#include "general/ducontainer.h"
#include "general/dudate.h"
#include "general/dunumeric.h"
#include "general/duversion.h"


DU_OBJECT(DuDevice)

class DuDevice : public DuContainer
{
public:
    DuDevice();

    virtual DuObjectPtr clone() const;

    virtual QHttpMultiPart* toHttpMultiPart(const QByteArray &boundary) const;

    QString getDisplayName() const;

    bool setVersion(const QString& value);
    bool setVersion(int major, int minor, int patch);

    bool getPlugged() const;
    void setPlugged(bool value);

    bool getConnected() const;
    void setConnected(bool value);

    bool getBusy() const;
    void setBusy(bool value);

    DU_KEY_ACCESSORS(SerialNumber, QString)
    DU_KEY_ACCESSORS(Name,         QString)
    DU_KEY_ACCESSORS(Owner,        QString)
    DU_KEY_ACCESSORS(OwnerId,      int)

    DU_KEY_ACCESSORS_OBJECT(Version, DuVersion)

    DU_KEY_ACCESSORS(UpdateDate, QDateTime)

private:
    bool plugged;
    bool connected;
    bool busy;
};

Q_DECLARE_METATYPE(DuDevicePtr)
Q_DECLARE_METATYPE(DuDeviceConstPtr)

#endif // DUDEVICE_H
