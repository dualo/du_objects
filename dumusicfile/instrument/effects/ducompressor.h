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


DU_OBJECT(DuCompressor)

class DuCompressor : public DuEffectSettings
{
public:
    explicit DuCompressor();
    ~DuCompressor();

    virtual DuObjectPtr clone() const;

    static DuCompressorPtr fromDuMusicFile(const FX_compressor &du_compressor);
    static DuCompressorPtr fromJson(const QJsonObject &jsonCompressor);

    QByteArray toDuMusicFile() const;

    int size() const;

    int getOnOff() const;
    bool setOnOff(int value);

    int getAttackTime() const;
    bool setAttackTime(int value);

    int getReleaseTime() const;
    bool setReleaseTime(int value);

    int getThreshold() const;
    bool setThreshold(int value);

    int getRatio() const;
    bool setRatio(int value);

    int getBoost() const;
    bool setBoost(int value);

    int getKneeType() const;
    bool setKneeType(int value);

    QString getEffectName() const;
    bool setEffectName(const QString &value);
};

#endif // DUCOMPRESSOR_H
