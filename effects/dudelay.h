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


DU_OBJECT(DuDelay)

class DuDelay : public DuEffectSettings
{
public:
    explicit DuDelay();
    ~DuDelay();

    static DuDelayPtr fromDuMusicFile(const FX_delay &du_delay);
    static DuDelayPtr fromJson(const QJsonObject &jsonDelay);

    QByteArray toDuMusicFile() const;

    int size() const;

    int getOnOff() const;
    bool setOnOff(int value);

    int getMode() const;
    bool setMode(int value);

    int getPreLowPassFilter() const;
    bool setPreLowPassFilter(int value);

    int getEffectLevel() const;
    bool setEffectLevel(int value);

    int getEffectTime() const;
    bool setEffectTime(int value);

    int getFeedback() const;
    bool setFeedback(int value);

    int getHDAmp() const;
    bool setHDAmp(int value);

    QString getEffectName() const;
    bool setEffectName(const QString &value);
};

#endif // DUDELAY_H
