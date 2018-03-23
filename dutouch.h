#ifndef DUTOUCH_H
#define DUTOUCH_H

#include "dudevice.h"

#include "jsonApi/JsonApiResourceObject.h"


DU_OBJECT(DuTouch);

class DuTouch : public DuDevice
{
public:
    DuTouch(Type type = Type_Unknown);
    virtual ~DuTouch() = default;

    virtual DuObjectPtr clone() const;

    static DuTouchPtr fromJsonApi(const JsonApiResourceObject& jsonDuTouch);

    QHttpMultiPart *toHttpMultiPart(const QByteArray &boundary) const;

    DU_KEY_ACCESSORS(SoundbankVersion, Version)
    DU_KEY_ACCESSORS(SoundbankUpdateDate, QDateTime)
};

#endif // DUTOUCH_H
