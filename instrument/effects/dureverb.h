#ifndef DUREVERB_H
#define DUREVERB_H

#include "dueffectsettings.h"


DU_OBJECT(DuReverb);

class DuReverb : public DuEffectSettings
{
public:
    DuReverb();
    virtual ~DuReverb() = default;

    virtual DuObjectPtr clone() const;

    static DuReverbPtr fromDuMusicBinary(const FX_reverb &du_reverb);

    QByteArray toDuMusicBinary() const;

    int size() const;


    DU_KEY_ACCESSORS(Level,             int)
    DU_KEY_ACCESSORS(DirectLevel,       int)
    DU_KEY_ACCESSORS(ReverbSend,        int)

    DU_KEY_ACCESSORS(ToneGain,          int)
    DU_KEY_ACCESSORS(ToneFrequency,     int)
    DU_KEY_ACCESSORS(PreHighPassFilter, int)

    DU_KEY_ACCESSORS(Time,              int)
    DU_KEY_ACCESSORS(EchoFeedback,      int)
    DU_KEY_ACCESSORS(HDAmp,             int)
    DU_KEY_ACCESSORS(ThresholdGate,     int)
    DU_KEY_ACCESSORS(PreDelayTime,      int)

    DU_KEY_ACCESSORS(EffectName,        QString)
};

#endif // DUREVERB_H
