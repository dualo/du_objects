#ifndef DUREVERB_H
#define DUREVERB_H

#include "dueffectsettings.h"


#define KEY_REVERB_LEVEL                "Level"
#define KEY_REVERB_DIRECTLEVEL          "DirectLevel"
#define KEY_REVERB_REVERBSEND           "ReverbSend"

#define KEY_REVERB_TONEGAIN             "ToneGain"
#define KEY_REVERB_TONEFREQUENCY        "ToneFrequency"
#define KEY_REVERB_PREHPFILTER          "PreHighPassFilter"

#define KEY_REVERB_TIME                 "Time"
#define KEY_REVERB_ECHOFEEDBACK         "EchoFeedback"
#define KEY_REVERB_HDAMP                "HDAmp"
#define KEY_REVERB_THRESHOLDGATE        "ThresholdGate"
#define KEY_REVERB_PREDELAYTIME         "PreDelayTime"

#define KEY_REVERB_EFFECTNAME           "EffectName"


DU_OBJECT(DuReverb)

class DuReverb : public DuEffectSettings
{
public:
    explicit DuReverb();
    ~DuReverb();

    virtual DuObjectPtr clone() const;

    static DuReverbPtr fromDuMusicBinary(const FX_reverb &du_reverb);
    static DuReverbPtr fromJson(const QJsonObject &jsonReverb);

    QByteArray toDuMusicBinary() const;

    int size() const;

    int getLevel() const;
    bool setLevel(int value);

    int getDirectLevel() const;
    bool setDirectLevel(int value);

    int getReverbSend() const;
    bool setReverbSend(int value);

    int getToneGain() const;
    bool setToneGain(int value);

    int getToneFrequency() const;
    bool setToneFrequency(int value);

    int getPreHPFilter() const;
    bool setPreHPFilter(int value);

    int getTime() const;
    bool setTime(int value);

    int getEchoFeedback() const;
    bool setEchoFeedback(int value);

    int getHDAmp() const;
    bool setHDAmp(int value);

    int getThresholdGate() const;
    bool setThresholdGate(int value);

    int getPreDelayTime() const;
    bool setPreDelayTime(int value);

    QString getEffectName() const;
    bool setEffectName(const QString &value);
};

#endif // DUREVERB_H
