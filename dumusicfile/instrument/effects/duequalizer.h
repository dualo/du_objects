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


DU_OBJECT(DuEqualizer)

class DuEqualizer : public DuEffectSettings
{
public:
    explicit DuEqualizer();
    ~DuEqualizer();

    virtual DuObjectPtr clone() const;

    static DuEqualizerPtr fromDuMusicBinary(const FX_equalizer du_equalizer);
    static DuEqualizerPtr fromJson(const QJsonObject &jsonEqualizer);

    QByteArray toDuMusicBinary() const;

    int size() const;

    int getOnOff() const;
    bool setOnOff(int value);

    int getLowBandGain() const;
    bool setLowBandGain(int value);

    int getLowMidBandGain() const;
    bool setLowMidBandGain(int value);

    int getHighMidBandGain() const;
    bool setHighMidBandGain(int value);

    int getHighBandGain() const;
    bool setHighBandGain(int value);

    int getLowBandFrequency() const;
    bool setLowBandFrequency(int value);

    int getLowMidBandFrequency() const;
    bool setLowMidBandFrequency(int value);

    int getHighMidBandFrequency() const;
    bool setHighMidBandFrequency(int value);

    int getHighBandFrequency() const;
    bool setHighBandFrequency(int value);

    int getLowMidBandQualityFactor() const;
    bool setLowMidBandQualityFactor(int value);

    int getHighMidBandQualityFactor() const;
    bool setHighMidBandQualityFactor(int value);

    QString getEffectName() const;
    bool setEffectName(const QString &value);
};

#endif // DUEQUALIZER_H
