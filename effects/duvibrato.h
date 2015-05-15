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

    int size() const;

    int getDepth() const;
    bool setDepth(int value);

    int getDelay() const;
    bool setDelay(int value);

    int getRate() const;
    bool setRate(int value);

    QString getEffectName() const;
    bool setEffectName(const QString &value);
};

#endif // DUVIBRATO_H
