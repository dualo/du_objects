#ifndef DUDEVICE_H
#define DUDEVICE_H

#include "ducontainer.h"
#include "dudate.h"
#include "dunumeric.h"
#include "duversion.h"

#include <QSharedPointer>

#define KEY_DEVICE_SERIAL_NUMBER    "SerialNumber"
#define KEY_DEVICE_NAME             "Name"
#define KEY_DEVICE_OWNER            "Owner"
#define KEY_DEVICE_OWNER_ID         "OwnerId"

#define KEY_DEVICE_VERSION          "Version"
#define KEY_DEVICE_UPDATE_DATE      "UpdateDate"

class DuDevice : public DuContainer
{
public:
    DuDevice();

    virtual QHttpMultiPart* toHttpMultiPart(const QByteArray &boundary) const;

    QString getSerialNumber() const;
    bool setSerialNumber(const QString& value);

    QString getName() const;
    bool setName(const QString& value);

    QString getDisplayName() const;

    QString getOwner() const;
    bool setOwner(const QString& value);

    int getOwnerId() const;
    bool setOwnerId(int value);

    const DuVersion* getVersion() const;
    bool setVersion(const QString& value);
    bool setVersion(int major, int minor, int patch);

    QDateTime getUpdateDate() const;
    bool setUpdateDate(const QDateTime& value);

    bool getPlugged() const;
    void setPlugged(bool value);

    bool getConnected() const;
    void setConnected(bool value);

    bool getBusy() const;
    void setBusy(bool value);

private:
    const DuString* getSerialNumberObject() const;
    const DuString* getNameObject() const;
    const DuString* getOwnerObject() const;
    const DuNumeric* getOwnerIdObject() const;
    const DuVersion* getVersionObject() const;
    const DuDate* getUpdateDateObject() const;

private:
    bool plugged;
    bool connected;
    bool busy;
};

Q_DECLARE_METATYPE(DuDevice*)
Q_DECLARE_METATYPE(QSharedPointer<DuDevice>)

#endif // DUDEVICE_H
