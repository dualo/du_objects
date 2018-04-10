#ifndef DUWORLDUSERDATA_H
#define DUWORLDUSERDATA_H

#include "../general/ducontainer.h"

#include "FinishedGameData.h"

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

    typedef QMap<int, FinishedGameData> FinishedGamesMap;
    DU_KEY_ACCESSORS(FinishedGames, FinishedGamesMap)
    DU_KEY_ACCESSORS(UnlockedGames, QList<int>)
};

Q_DECLARE_METATYPE(DuWorldUserDataPtr)
Q_DECLARE_METATYPE(DuWorldUserData::FinishedGamesMap)

#endif // DUWORLDUSERDATA_H
