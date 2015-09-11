#ifndef DUDISTORTION_H
#define DUDISTORTION_H

#include "dueffectsettings.h"


DU_OBJECT(DuDistortion)

class DuDistortion : public DuEffectSettings
{
public:
    explicit DuDistortion();
    ~DuDistortion();

    virtual DuObjectPtr clone() const;

    static DuDistortionPtr fromDuMusicBinary(const FX_distortion &du_distortion);
    static DuDistortionPtr fromJson(const QJsonObject &jsonDistortion);

    QByteArray toDuMusicBinary() const;

    int size() const;


    DU_KEY_ACCESSORS(OnOff,                  int)
    DU_KEY_ACCESSORS(PreGain,                int)
    DU_KEY_ACCESSORS(EffectType,             int)
    DU_KEY_ACCESSORS(LowPassFilterFrequency, int)
    DU_KEY_ACCESSORS(LowPassFilterResonance, int)
    DU_KEY_ACCESSORS(PostGain,               int)
    DU_KEY_ACCESSORS(Drive,                  int)
    DU_KEY_ACCESSORS(EffectName,             QString)
};

#endif // DUDISTORTION_H
