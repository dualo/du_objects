#ifndef DUVIBRATO_H
#define DUVIBRATO_H

#include "dueffectsettings.h"


#define KEY_VIB_DEPTH               "Depth"
#define KEY_VIB_DELAY               "Delay"
#define KEY_VIB_RATE                "Rate"
#define KEY_VIB_EFFECTNAME          "EffectName"


DU_OBJECT(DuVibrato)

class DuVibrato : public DuEffectSettings
{
public:
    explicit DuVibrato();
    ~DuVibrato();

    virtual DuObjectPtr clone() const;

    static DuVibratoPtr fromDuMusicFile(const FX_vibrato &du_vibrato);
    static DuVibratoPtr fromJson(const QJsonObject &jsonVibrato);

    QByteArray toDuMusicFile() const;

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
