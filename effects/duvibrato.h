#ifndef DUVIBRATO_H
#define DUVIBRATO_H

#include "dueffectsettings.h"


#define KEY_VIB_DEPTH               "Depth"
#define KEY_VIB_DELAY               "Delay"
#define KEY_VIB_RATE                "Rate"
#define KEY_VIB_EFFECTNAME          "EffectName"


class DuVibrato : public DuEffectSettings
{
public:
    explicit DuVibrato();
    ~DuVibrato();

    static DuVibrato *fromDuMusicFile(const FX_vibrato &du_vibrato);

    static DuVibrato *fromJson(const QJsonObject &jsonVibrato);

    int getDepth() const;
    void setDepth(int value);

    int getDelay() const;
    void setDelay(int value);

    int getRate() const;
    void setRate(int value);

    QString getEffectName() const;
    void setEffectName(QString &value);
};

#endif // DUVIBRATO_H
