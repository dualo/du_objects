#ifndef DUDISTORTION_H
#define DUDISTORTION_H

#include "dueffectsettings.h"


DU_OBJECT(DuDistortion);

class DuDistortion : public DuEffectSettings
{
public:
    DuDistortion();
    virtual ~DuDistortion() = default;

    virtual DuObjectPtr clone() const;

    static DuDistortionPtr fromDuMusicBinary(const FX_distortion &du_distortion);

    QByteArray toDuMusicBinary() const;

    int size() const;


    DU_KEY_ACCESSORS(OnOff,                  int)
    DU_KEY_ACCESSORS(PreGain,                int)
    DU_KEY_ACCESSORS(EffectType,             int)
    DU_KEY_ACCESSORS(LowPassFilterFrequency, int)
    DU_KEY_ACCESSORS(LowPassFilterResonance, int)
    DU_KEY_ACCESSORS(PostGain,               int)
    DU_KEY_ACCESSORS(Drive,                  int)
};

#endif // DUDISTORTION_H
