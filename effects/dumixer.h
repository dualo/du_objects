#ifndef DUMIXER_H
#define DUMIXER_H

#include "dueffectsettings.h"


#define KEY_MIXER_INPUTGAIN                     "InputGain"
#define KEY_MIXER_LOWCUTFILTERFREQUENCY         "LowCutFilterFrequency"
#define KEY_MIXER_HIGHCUTFILTERFREQUENCY        "HighCutFilterFrequency"
#define KEY_MIXER_OUTPUTLEVEL                   "OutputLevel"
#define KEY_MIXER_OUTPUTPANNING                 "OutputPanning"
#define KEY_MIXER_OUTPUTFRONTREAR               "OutputFrontRear"
#define KEY_MIXER_SENDTOREVERB                  "SendToReverb"
#define KEY_MIXER_SENDTOCHORUS                  "SendToChorus"


class DuMixer : public DuEffectSettings
{
public:
    explicit DuMixer();
    ~DuMixer();

    static DuMixer *fromDuMusicFile(const FX_mix &du_mixer);

    static DuMixer *fromJson(const QJsonObject &jsonMixer);

    int getInputGain() const;
    void setInputGain(int value);

    int getLowCutFilterFrequency() const;
    void setLowCutFilterFrequency(int value);

    int getHighCutFilterFrequency() const;
    void setHighCutFilterFrequency(int value);

    int getOutputLevel() const;
    void setOutputLevel(int value);

    int getOutputPanning() const;
    void setOutputPanning(int value);

    int getOutputFrontRear() const;
    void setOutputFrontRear(int value);

    int getSendToReverb() const;
    void setSendToReverb(int value);

    int getSendToChorus() const;
    void setSendToChorus(int value);
};

#endif // DUMIXER_H
