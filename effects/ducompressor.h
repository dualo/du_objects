#ifndef DUCOMPRESSOR_H
#define DUCOMPRESSOR_H

#include "dueffectsettings.h"


#define KEY_COMP_ONOFF              "OnOff"
#define KEY_COMP_ATTACKTIME         "AttackTime"
#define KEY_COMP_RELEASETIME        "ReleaseTime"
#define KEY_COMP_THRESHOLD          "Threshold"
#define KEY_COMP_RATIO              "Ratio"
#define KEY_COMP_BOOST              "Boost"
#define KEY_COMP_KNEETYPE           "KneeType"
#define KEY_COMP_EFFECTNAME         "EffectName"


class DuCompressor : public DuEffectSettings
{
public:
    explicit DuCompressor();
    ~DuCompressor();

    static DuCompressor *fromDuMusicFile(const FX_compressor &du_compressor);

    static DuCompressor *fromJson(const QJsonObject &jsonCompressor);

    int getOnOff() const;
    void setOnOff(int value);

    int getAttackTime() const;
    void setAttackTime(int value);

    int getReleaseTime() const;
    void setReleaseTime(int value);

    int getThreshold() const;
    void setThreshold(int value);

    int getRatio() const;
    void setRatio(int value);

    int getBoost() const;
    void setBoost(int value);

    int getKneeType() const;
    void setKneeType(int value);

    QString getEffectName() const;
    void setEffectName(QString &value);
};

#endif // DUCOMPRESSOR_H
