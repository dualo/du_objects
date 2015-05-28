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


DU_OBJECT(DuDistortion)

class DuDistortion : public DuEffectSettings
{
public:
    explicit DuDistortion();
    ~DuDistortion();

    virtual DuObjectPtr clone() const;

    static DuDistortionPtr fromDuMusicBinary(const FX_distortion &du_distortion);
    static DuDistortionPtr fromJson(const QJsonObject &jsonDistortion);

    QByteArray toDuMusicBinary() const;

    int size() const;

    int getOnOff() const;
    bool setOnOff(int value);

    int getPreGain() const;
    bool setPreGain(int value);

    int getEffectType() const;
    bool setEffectType(int value);

    int getLowPassFilterFrequency() const;
    bool setLowPassFilterFrequency(int value);

    int getLowPassFilterResonance() const;
    bool setLowPassFilterResonance(int value);

    int getPostGain() const;
    bool setPostGain(int value);

    int getDrive() const;
    bool setDrive(int value);

    QString getEffectName() const;
    bool setEffectName(const QString &value);
};

#endif // DUDISTORTION_H
