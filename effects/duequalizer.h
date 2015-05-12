#ifndef DUEQUALIZER_H
#define DUEQUALIZER_H

#include "dueffectsettings.h"


#define KEY_EQ_ONOFF                        "OnOff"
#define KEY_EQ_LOWBANDGAIN                  "LowBandGain"
#define KEY_EQ_LOWMIDBANDGAIN               "LowMidBandGain"
#define KEY_EQ_HIGHMIDBANDGAIN              "HighMidBandGain"
#define KEY_EQ_HIGHBANDGAIN                 "HighBandGain"
#define KEY_EQ_LOWBANDFREQUENCY             "LowBandFrequency"
#define KEY_EQ_LOWMIDBANDFREQUENCY          "LowMidBandFrequency"
#define KEY_EQ_HIGHMIDBANDFREQUENCY         "HighMidBandFrequency"
#define KEY_EQ_HIGHBANDFREQUENCY            "HighBandFrequency"
#define KEY_EQ_LOWMIDBANDQUALITYFACTOR      "LowMidBandQualityFactor"
#define KEY_EQ_HIGHMIDBANDQUALITYFACTOR     "HighMidBandQualityFactor"
#define KEY_EQ_EFFECTNAME                   "EffectName"


class DuEqualizer : public DuEffectSettings
{
public:
    explicit DuEqualizer();
    ~DuEqualizer();

    static DuEqualizer *fromDuMusicFile(const FX_equalizer du_equalizer);

    static DuEqualizer *fromJson(const QJsonObject &jsonEqualizer);

    int getOnOff() const;
    void setOnOff(int value);

    int getLowBandGain() const;
    void setLowBandGain(int value);

    int getLowMidBandGain() const;
    void setLowMidBandGain(int value);

    int getHighMidBandGain() const;
    void setHighMidBandGain(int value);

    int getHighBandGain() const;
    void setHighBandGain(int value);

    int getLowBandFrequency() const;
    void setLowBandFrequency(int value);

    int getLowMidBandFrequency() const;
    void setLowMidBandFrequency(int value);

    int getHighMidBandFrequency() const;
    void setHighMidBandFrequency(int value);

    int getHighBandFrequency() const;
    void setHighBandFrequency(int value);

    int getLowMidBandQualityFactor() const;
    void setLowMidBandQualityFactor(int value);

    int getHighMidBandQualityFactor() const;
    void setHighMidBandQualityFactor(int value);

    QString getEffectName() const;
    void setEffectName(const QString &value);
};

#endif // DUEQUALIZER_H
