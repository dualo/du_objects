#ifndef DUDEVICE_H
#define DUDEVICE_H

#include "general/ducontainer.h"

#include <Util/Version.h>


DU_OBJECT(DuVersion);

DU_OBJECT(DuDevice);

class DuDevice : public DuContainer
{
    Q_GADGET

public:
    enum Type
    {
        Type_Unknown = 0,
        Type_DuTouchPro,
        Type_DuTouchS,
        Type_DuTouchL,

        Type_Other
    };
    Q_ENUM(Type)

    static QString typeToString(Type type);

    DuDevice(Type type = Type_Unknown);
    virtual ~DuDevice() = default;

    virtual DuObjectPtr clone() const;

    virtual QHttpMultiPart* toHttpMultiPart(const QByteArray &boundary) const;

    QString getDisplayName() const;

    bool getPlugged() const;
    void setPlugged(bool value);

    bool getConnected() const;
    bool setConnected(bool value);

    bool getBusy() const;
    bool setBusy(bool value);

    bool getIsInFlashMeState() const;
    void setIsInFlashMeState(bool value);

    DU_KEY_ACCESSORS(Type,            Type)
    DU_KEY_ACCESSORS(SerialNumber,    QString)
    DU_KEY_ACCESSORS(Name,            QString)
    DU_KEY_ACCESSORS(Owner,           QString)
    DU_KEY_ACCESSORS(OwnerId,         int)
    DU_KEY_ACCESSORS(DissocScheduled, bool)
    DU_KEY_ACCESSORS(ApiVersion,      Version)
    DU_KEY_ACCESSORS(Version,         Version)
    DU_KEY_ACCESSORS(UpdateDate,      QDateTime)

private:
    bool plugged;
    bool connected;
    bool busy;
    bool isInFlashMeState;
};

Q_DECLARE_METATYPE(DuDevicePtr)
Q_DECLARE_METATYPE(DuDeviceConstPtr)

#endif // DUDEVICE_H
