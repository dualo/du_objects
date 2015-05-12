#ifndef DUDISTORTION_H
#define DUDISTORTION_H

#include "dueffectsettings.h"


#define KEY_DIST_ONOFF                      "OnOff"
#define KEY_DIST_PREGAIN                    "PreGain"
#define KEY_DIST_TYPE                       "Type"
#define KEY_DIST_LOWPASSFILTERFREQUENCY     "LowPassFilterFrequency"
#define KEY_DIST_LOWPASSFILTERRESONANCE     "LowPassFilterResonance"
#define KEY_DIST_POSTGAIN                   "PostGain"
#define KEY_DIST_DRIVE                      "Drive"
#define KEY_DIST_EFFECTNAME                 "EffectName"


class DuDistortion : public DuEffectSettings
{
public:
    explicit DuDistortion();
    ~DuDistortion();

    static DuDistortion *fromDuMusicFile(const FX_distortion &du_distortion);

    static DuDistortion *fromJson(const QJsonObject &jsonDistortion);

    int getOnOff() const;
    void setOnOff(int value);

    int getPreGain() const;
    void setPreGain(int value);

    int getEffectType() const;
    void setEffectType(int value);

    int getLowPassFilterFrequency() const;
    void setLowPassFilterFrequency(int value);

    int getLowPassFilterResonance() const;
    void setLowPassFilterResonance(int value);

    int getPostGain() const;
    void setPostGain(int value);

    int getDrive() const;
    void setDrive(int value);

    QString getEffectName() const;
    void setEffectName(const QString &value);
};

#endif // DUDISTORTION_H
