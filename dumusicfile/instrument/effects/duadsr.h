#ifndef DUADSR_H
#define DUADSR_H

#include "dueffectsettings.h"


#define KEY_ADSR_ENVELOPEATTACKTIME                 "EnvelopeAttackTime"
#define KEY_ADSR_ENVELOPEDECAYTIME                  "EnvelopeDecayTime"
#define KEY_ADSR_TIMEVARIANTFILTERCUTOFFRESONANCE   "TimeVariantFilterCutoffResonance"
#define KEY_ADSR_TIMEVARIANTFILTERCUTOFFFREQUENCY   "TimeVariantFilterCutoffFrequency"
#define KEY_ADSR_ENVELOPERELEASETIME                "EnvelopeReleaseTime"
#define KEY_ADSR_EFFECTNAME                         "EffectName"


DU_OBJECT(DuAdsr)

class DuAdsr : public DuEffectSettings
{
public:
    explicit DuAdsr();
    ~DuAdsr();

    virtual DuObjectPtr clone() const;

    static DuAdsrPtr fromDuMusicFile(const FX_adsr &du_adsr);
    static DuAdsrPtr fromJson(const QJsonObject &jsonAdsr);

    QByteArray toDuMusicFile() const;

    int size() const;

    int getEnvelopeAttackTime() const;
    bool setEnvelopeAttackTime(int value);

    int getEnvelopeDecayTime() const;
    bool setEnvelopeDecayTime(int value);

    int getTimeVariantFilterCutoffResonance() const;
    bool setTimeVariantFilterCutoffResonance(int value);

    int getTimeVariantFilterCutoffFrequency() const;
    bool setTimeVariantFilterCutoffFrequency(int value);

    int getEnvelopeReleaseTime() const;
    bool setEnvelopeReleaseTime(int value);

    QString getEffectName() const;
    bool setEffectName(const QString &value);
};

#endif // DUADSR_H
