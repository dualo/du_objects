#ifndef DUSYSTEMSOUNDIDENTIFIER_H
#define DUSYSTEMSOUNDIDENTIFIER_H

#include "../general/ducontainer.h"

#pragma pack(push, 4)
#include "../du-touch/dualo_structs/dugame_structs.h"
#pragma pack(pop)

DU_OBJECT(DuSystemSoundIdentifier);

class DuSystemSoundIdentifier : public DuContainer
{
public:
    DuSystemSoundIdentifier();
    virtual ~DuSystemSoundIdentifier() = default;

    static DuSystemSoundIdentifierPtr fromBinary(const s_dugame_sound& data);

    // DuObject interface
public:
    virtual DuObjectPtr clone() const Q_DECL_OVERRIDE;
    virtual QByteArray toDuMusicBinary() const Q_DECL_OVERRIDE;
    virtual int size() const Q_DECL_OVERRIDE;

    DU_KEY_ACCESSORS(ID, int)
    DU_KEY_ACCESSORS(UserID, int)
};

#endif // DUSYSTEMSOUNDIDENTIFIER_H
