#ifndef DUMUSICMETADATA_H
#define DUMUSICMETADATA_H

#include "../general/ducontainer.h"

DU_OBJECT(DuGame);
DU_OBJECT(DuSound);

DU_OBJECT(DuMusicMetadata);

class DuMusicMetadata : public DuContainer
{
public:
    DuMusicMetadata();
    virtual ~DuMusicMetadata() = default;

    static DuMusicMetadataPtr fromBinary(const QByteArray &data);

    QByteArray toBinary(const QVector<DuSoundConstPtr> &systemSounds = {}) const;

    // DuObject interface
public:
    virtual DuObjectPtr clone() const Q_DECL_OVERRIDE;
    virtual int size() const Q_DECL_OVERRIDE;

    DU_KEY_ACCESSORS_OBJECT(Game, DuGame)
};

#endif // DUMUSICMETADATA_H
