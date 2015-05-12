#ifndef DUDELAY_H
#define DUDELAY_H

#include "dueffectsettings.h"


#define KEY_DELAY_ONOFF                 "OnOff"
#define KEY_DELAY_MODE                  "Mode"
#define KEY_DELAY_PRELOWPASSFILTER      "PreLowPassFilter"
#define KEY_DELAY_EFFECTLEVEL           "EffectLevel"
#define KEY_DELAY_EFFECTTIME            "EffectTime"
#define KEY_DELAY_FEEDBACK              "Feedback"
#define KEY_DELAY_HDAMP                 "HDAmp"
#define KEY_DELAY_EFFECTNAME            "EffectName"


class DuDelay : public DuEffectSettings
{
public:
    explicit DuDelay();
    ~DuDelay();

    static DuDelay *fromDuMusicFile(const FX_delay &du_delay);

    static DuDelay *fromJson(const QJsonObject &jsonDelay);

    int getOnOff() const;
    void setOnOff(int value);

    int getMode() const;
    void setMode(int value);

    int getPreLowPassFilter() const;
    void setPreLowPassFilter(int value);

    int getEffectLevel() const;
    void setEffectLevel(int value);

    int getEffectTime() const;
    void setEffectTime(int value);

    int getFeedback() const;
    void setFeedback(int value);

    int getHDAmp() const;
    void setHDAmp(int value);

    QString getEffectName() const;
    void setEffectName(const QString &value);
};

#endif // DUDELAY_H
