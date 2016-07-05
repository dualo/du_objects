#ifndef DUEFFECTSET_H
#define DUEFFECTSET_H

#include "../general/ducontainer.h"


#define EFFECTSET_SIZE              8 + 11 + 5 + 9 + 59
#define EFFECTSET_PRESET_OFFSET     36

#define DuEffectSet_Children \
    X(CompressorOnOff, Numeric, int, -1) \
    X(DelayOnOff,      Numeric, int, -1) \
    X(DistortionOnOff, Numeric, int, -1) \
    X(EqualizerOnOff,  Numeric, int, -1) \
    X(ChorusOnOff,     Numeric, int, -1) \
    X(ReverbOnOff,     Numeric, int, -1) \
    \
    X(MultinoteAct,    Numeric, int, -1) \
    \
    X(Pitch,           Numeric, int, -1) \
    \
    X(AutopitchRate,   Numeric, int, -1) \
    X(AutopitchRange,  Numeric, int, -1) \
    X(TremoloRate,     Numeric, int, -1) \
    X(TremoloRange,    Numeric, int, -1) \
    X(AutopanRate,     Numeric, int, -1) \
    X(AutopanRange,    Numeric, int, -1) \
    X(AutowahRate,     Numeric, int, -1) \
    X(AutowahRange,    Numeric, int, -1) \
    \
    X(AdsrAttack,      Numeric, int, -1) \
    X(AdsrRelease,     Numeric, int, -1) \
    X(WahType,         Numeric, int, -1) \
    X(WahFrequency,    Numeric, int, -1) \
    X(WahResonance,    Numeric, int, -1)

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

#define X(key, dutype, type, defaultReturn) DU_KEY_ACCESSORS(key, type)
    DuEffectSet_Children
#undef X

    DU_KEY_ACCESSORS_OBJECT(Multinote,  DuArray)
};

#endif // DUEFFECTSET_H
