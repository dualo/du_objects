#ifndef DUADSR_H
#define DUADSR_H

#include "dueffectsettings.h"


DU_OBJECT(DuAdsr)

class DuAdsr : public DuEffectSettings
{
public:
    explicit DuAdsr();
    ~DuAdsr();

    virtual DuObjectPtr clone() const;

    static DuAdsrPtr fromDuMusicBinary(const FX_adsr &du_adsr);
    static DuAdsrPtr fromJson(const QJsonObject &jsonAdsr);

    QByteArray toDuMusicBinary() const;

    int size() const;


    DU_KEY_ACCESSORS(EnvelopeAttackTime,               int)
    DU_KEY_ACCESSORS(EnvelopeDecayTime,                int)
    DU_KEY_ACCESSORS(TimeVariantFilterCutoffResonance, int)
    DU_KEY_ACCESSORS(TimeVariantFilterCutoffFrequency, int)
    DU_KEY_ACCESSORS(EnvelopeReleaseTime,              int)
    DU_KEY_ACCESSORS(EffectName,                       QString)
};

#endif // DUADSR_H
