#ifndef DUCHORUS_H
#define DUCHORUS_H

#include "dueffectsettings.h"


#define KEY_CHORUS_MODE                     "Mode"
#define KEY_CHORUS_EFFECTLEVEL              "EffectLevel"
#define KEY_CHORUS_DELAYTIME                "DelayTime"
#define KEY_CHORUS_FEEDBACK                 "Feedback"
#define KEY_CHORUS_INPUTHIGHPASSFILTER      "InputHighPassFilterFrequency"
#define KEY_CHORUS_HDAMP                    "HDAmp"
#define KEY_CHORUS_MODULATIONDEPTH          "ModulationDepth"
#define KEY_CHORUS_MODULATIONRATE           "ModulationRate"
#define KEY_CHORUS_TREMOLOSHAPE             "TremoloModulationShape"
#define KEY_CHORUS_ROTARYSPEED              "RotarySpeed"
#define KEY_CHORUS_EFFECTNAME               "EffectName"


class DuChorus : public DuEffectSettings
{
public:
    explicit DuChorus();
    ~DuChorus();

    static DuChorus *fromDuMusicFile(const FX_chorus &du_chorus);

    static DuChorus *fromJson(const QJsonObject &jsonChorus);

    int getMode() const;
    void setMode(int value);

    int getEffectLevel() const;
    void setEffectLevel(int value);

    int getDelayTime() const;
    void setDelayTime(int value);

    int getFeedback() const;
    void setFeedback(int value);

    int getInputHighPassFilterFrequency() const;
    void setInputHighPassFilterFrequency(int value);

    int getHDAmp() const;
    void setHDAmp(int value);

    int getModulationDepth() const;
    void setModulationDepth(int value);

    int getModulationRate() const;
    void setModulationRate(int value);

    int getTremoloShape() const;
    void setTremoloShape(int value);

    int getRotarySpeed() const;
    void setRotarySpeed(int value);

    QString getEffectName() const;
    void setEffectName(QString &value);
};

#endif // DUCHORUS_H
