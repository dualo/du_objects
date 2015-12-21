#ifndef DUMIXER_H
#define DUMIXER_H

#include "dueffectsettings.h"


DU_OBJECT(DuMixer);

class DuMixer : public DuEffectSettings
{
public:
    explicit DuMixer();
    ~DuMixer();

    virtual DuObjectPtr clone() const;

    static DuMixerPtr fromDuMusicBinary(const FX_mix &du_mixer);
    static DuMixerPtr fromJson(const QJsonObject &jsonMixer);

    QByteArray toDuMusicBinary() const;

    int size() const;


    DU_KEY_ACCESSORS(InputGain,              int)
    DU_KEY_ACCESSORS(LowCutFilterFrequency,  int)
    DU_KEY_ACCESSORS(HighCutFilterFrequency, int)
    DU_KEY_ACCESSORS(OutputLevel,            int)
    DU_KEY_ACCESSORS(OutputPanning,          int)
    DU_KEY_ACCESSORS(OutputFrontRear,        int)
    DU_KEY_ACCESSORS(SendToReverb,           int)
    DU_KEY_ACCESSORS(SendToChorus,           int)
};

#endif // DUMIXER_H
