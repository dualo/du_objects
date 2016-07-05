#ifndef DUTOUCH_H
#define DUTOUCH_H

#include "dudevice.h"


DU_OBJECT(DuTouch);

class DuTouch : public DuDevice
{
public:
    DuTouch();

    virtual DuObjectPtr clone() const;

    QHttpMultiPart *toHttpMultiPart(const QByteArray &boundary) const;

    bool setSoundbankVersion(const QString& value);
    bool setSoundbankVersion(int major, int minor, int patch);

    DU_KEY_ACCESSORS_OBJECT(SoundbankVersion, DuVersion)
    DU_KEY_ACCESSORS(SoundbankUpdateDate, QDateTime)
};

#endif // DUTOUCH_H
