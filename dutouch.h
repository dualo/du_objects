#ifndef DUTOUCH_H
#define DUTOUCH_H

#include "dudevice.h"

#define KEY_TOUCH_SOUNDBANK_VERSION     "SoundbankVersion"
#define KEY_TOUCH_SOUNDBANK_UPDATE_DATE "SoundbankUpdateDate"

class DuTouch : public DuDevice
{
public:
    DuTouch();

    QHttpMultiPart *toHttpMultiPart(const QByteArray &boundary) const;

    const DuVersion* getSoundbankVersion() const;
    bool setSoundbankVersion(const QString& value);
    bool setSoundbankVersion(int major, int minor, int patch);

    QDateTime getSoundbankUpdateDate() const;
    bool setSoundbankUpdateDate(const QDateTime& value);

private:
    const DuVersion* getSoundbankVersionObject() const;
    const DuDate* getSoundbankUpdateDateObject() const;
};

#endif // DUTOUCH_H
