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


DU_OBJECT(DuChorus)

class DuChorus : public DuEffectSettings
{
public:
    explicit DuChorus();
    ~DuChorus();

    virtual DuObjectPtr clone() const;

    static DuChorusPtr fromDuMusicFile(const FX_chorus &du_chorus);
    static DuChorusPtr fromJson(const QJsonObject &jsonChorus);

    QByteArray toDuMusicFile() const;

    int size() const;

    int getMode() const;
    bool setMode(int value);

    int getEffectLevel() const;
    bool setEffectLevel(int value);

    int getDelayTime() const;
    bool setDelayTime(int value);

    int getFeedback() const;
    bool setFeedback(int value);

    int getInputHighPassFilterFrequency() const;
    bool setInputHighPassFilterFrequency(int value);

    int getHDAmp() const;
    bool setHDAmp(int value);

    int getModulationDepth() const;
    bool setModulationDepth(int value);

    int getModulationRate() const;
    bool setModulationRate(int value);

    int getTremoloShape() const;
    bool setTremoloShape(int value);

    int getRotarySpeed() const;
    bool setRotarySpeed(int value);

    QString getEffectName() const;
    bool setEffectName(const QString &value);
};

#endif // DUCHORUS_H
