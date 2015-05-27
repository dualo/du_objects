#ifndef DUTOUCH_H
#define DUTOUCH_H

#include "dudevice.h"

#define KEY_TOUCH_SOUNDBANK_VERSION     "SoundbankVersion"
#define KEY_TOUCH_SOUNDBANK_UPDATE_DATE "SoundbankUpdateDate"


DU_OBJECT(DuTouch)

class DuTouch : public DuDevice
{
public:
    DuTouch();

    virtual DuObjectPtr clone() const;

    QHttpMultiPart *toHttpMultiPart(const QByteArray &boundary) const;

    DuVersionConstPtr getSoundbankVersion() const;
    bool setSoundbankVersion(const QString& value);
    bool setSoundbankVersion(int major, int minor, int patch);

    QDateTime getSoundbankUpdateDate() const;
    bool setSoundbankUpdateDate(const QDateTime& value);
};

#endif // DUTOUCH_H
