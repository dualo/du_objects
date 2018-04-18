#ifndef DUGAMEMETADATA_H
#define DUGAMEMETADATA_H

#include "../general/ducontainer.h"

#ifdef Q_OS_WIN
#include "../general/duarray.h"
#else
DU_OBJECT_TEMPLATE(DuArray);
#endif
DU_OBJECT(DuGameEvent);
DU_OBJECT(DuSystemSoundIdentifier);

DU_OBJECT(DuGameMetadata);

class DuGameMetadata : public DuContainer
{
public:
    DuGameMetadata();
    virtual ~DuGameMetadata() = default;

    static DuGameMetadataPtr fromBinary(const QByteArray& data, quint32 version);

    // DuObject interface
public:
    virtual DuObjectPtr clone() const Q_DECL_OVERRIDE;
    virtual QByteArray toDuMusicBinary() const Q_DECL_OVERRIDE;
    virtual int size() const Q_DECL_OVERRIDE;

    bool getIsFinished() const;
    int getNbStars() const;

    DU_KEY_ACCESSORS(Grade, int)

    DU_KEY_ACCESSORS(FirstStarEvent, int)
    DU_KEY_ACCESSORS(SecondStarEvent, int)
    DU_KEY_ACCESSORS(ThirdStarEvent, int)

    DU_KEY_ACCESSORS(Version, int)
    DU_KEY_ACCESSORS(GameId, int)
    DU_KEY_ACCESSORS_OBJECT_TEMPLATE(Sounds, DuArray, DuSystemSoundIdentifier)
    DU_KEY_ACCESSORS_OBJECT_TEMPLATE(Events, DuArray, DuGameEvent)
};

#endif // DUGAMEMETADATA_H
