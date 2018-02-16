#ifndef DUWORLDUSERDATA_H
#define DUWORLDUSERDATA_H

#include "../general/ducontainer.h"

DU_OBJECT(DuWorldUserData);

class DuWorldUserData : public DuContainer
{
public:
    DuWorldUserData();
    virtual ~DuWorldUserData() = default;

    void mergeWith(const DuWorldUserDataConstPtr& other, bool& outNewInThis, bool& outNewInOther);

    static DuWorldUserDataPtr fromJsonApi(const QJsonValue &jsonValue);

    // DuObject interface
public:
    virtual DuObjectPtr clone() const Q_DECL_OVERRIDE;

    // DuContainer interface
public:
    virtual QHttpMultiPart *toHttpMultiPart(const QByteArray &boundary) const Q_DECL_OVERRIDE;

    DU_KEY_ACCESSORS(FinishedGames, QList<int>)
    DU_KEY_ACCESSORS(UnlockedGames, QList<int>)
};

Q_DECLARE_METATYPE(DuWorldUserDataPtr)

#endif // DUWORLDUSERDATA_H
