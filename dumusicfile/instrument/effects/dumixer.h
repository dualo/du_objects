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


DU_OBJECT(DuMixer)

class DuMixer : public DuEffectSettings
{
public:
    explicit DuMixer();
    ~DuMixer();

    virtual DuObjectPtr clone() const;

    static DuMixerPtr fromDuMusicFile(const FX_mix &du_mixer);
    static DuMixerPtr fromJson(const QJsonObject &jsonMixer);

    QByteArray toDuMusicFile() const;

    int size() const;

    int getInputGain() const;
    bool setInputGain(int value);

    int getLowCutFilterFrequency() const;
    bool setLowCutFilterFrequency(int value);

    int getHighCutFilterFrequency() const;
    bool setHighCutFilterFrequency(int value);

    int getOutputLevel() const;
    bool setOutputLevel(int value);

    int getOutputPanning() const;
    bool setOutputPanning(int value);

    int getOutputFrontRear() const;
    bool setOutputFrontRear(int value);

    int getSendToReverb() const;
    bool setSendToReverb(int value);

    int getSendToChorus() const;
    bool setSendToChorus(int value);
};

#endif // DUMIXER_H
