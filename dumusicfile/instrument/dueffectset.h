#ifndef DUEFFECTSET_H
#define DUEFFECTSET_H

#include "../../general/ducontainer.h"


#define EFFECTSET_SIZE              8 + 11 + 5 + 9 + 59
#define EFFECTSET_PRESET_OFFSET     36

DU_OBJECT(DuArray);

DU_OBJECT(DuEffectSet);

class DuEffectSet : public DuContainer
{
public:
    explicit DuEffectSet();
    ~DuEffectSet();

    DuObjectPtr clone() const;

    static DuEffectSetPtr fromDuMusicBinary(const preset_instr &du_preset);
    static DuEffectSetPtr fromJson(const QJsonObject &jsonEffectSet);

    QByteArray toDuMusicBinary() const;

    int size() const;

    DU_KEY_ACCESSORS(CompressorOnOff,   int)
    DU_KEY_ACCESSORS(DelayOnOff,        int)
    DU_KEY_ACCESSORS(DistortionOnOff,   int)
    DU_KEY_ACCESSORS(EqualizerOnOff,    int)
    DU_KEY_ACCESSORS(ChorusOnOff,       int)

    DU_KEY_ACCESSORS(MultinoteAct,      int)
    DU_KEY_ACCESSORS_OBJECT(Multinote,  DuArray)

    DU_KEY_ACCESSORS(Pitch,             int)

    DU_KEY_ACCESSORS(AutopitchRate,     int)
    DU_KEY_ACCESSORS(AutopitchRange,    int)
    DU_KEY_ACCESSORS(TremoloRate,       int)
    DU_KEY_ACCESSORS(TremoloRange,      int)
    DU_KEY_ACCESSORS(AutopanRate,       int)
    DU_KEY_ACCESSORS(AutopanRange,      int)
    DU_KEY_ACCESSORS(AutowahRate,       int)
    DU_KEY_ACCESSORS(AutowahRange,      int)

    DU_KEY_ACCESSORS(AdsrAttack,        int)
    DU_KEY_ACCESSORS(AdsrRelease,       int)
    DU_KEY_ACCESSORS(WahType,           int)
    DU_KEY_ACCESSORS(WahFrequency,      int)
    DU_KEY_ACCESSORS(WahResonance,      int)
};

#endif // DUEFFECTSET_H
