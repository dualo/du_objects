#ifndef DUEQUALIZER_H
#define DUEQUALIZER_H

#include "dueffectsettings.h"


DU_OBJECT(DuEqualizer);

class DuEqualizer : public DuEffectSettings
{
public:
    DuEqualizer();
    virtual ~DuEqualizer() = default;

    virtual DuObjectPtr clone() const;

    static DuEqualizerPtr fromDuMusicBinary(const FX_equalizer &du_equalizer);

    QByteArray toDuMusicBinary() const;

    int size() const;


    DU_KEY_ACCESSORS(OnOff,                    int)
    DU_KEY_ACCESSORS(LowBandGain,              int)
    DU_KEY_ACCESSORS(LowMidBandGain,           int)
    DU_KEY_ACCESSORS(HighMidBandGain,          int)
    DU_KEY_ACCESSORS(HighBandGain,             int)
    DU_KEY_ACCESSORS(LowBandFrequency,         int)
    DU_KEY_ACCESSORS(LowMidBandFrequency,      int)
    DU_KEY_ACCESSORS(HighMidBandFrequency,     int)
    DU_KEY_ACCESSORS(HighBandFrequency,        int)
    DU_KEY_ACCESSORS(LowMidBandQualityFactor,  int)
    DU_KEY_ACCESSORS(HighMidBandQualityFactor, int)
};

#endif // DUEQUALIZER_H
