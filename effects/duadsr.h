#ifndef DUADSR_H
#define DUADSR_H

#include "dueffectsettings.h"


#define KEY_ADSR_ENVELOPEATTACKTIME                 "EnvelopeAttackTime"
#define KEY_ADSR_ENVELOPEDECAYTIME                  "EnvelopeDecayTime"
#define KEY_ADSR_TIMEVARIANTFILTERCUTOFFRESONANCE   "TimeVariantFilterCutoffResonance"
#define KEY_ADSR_TIMEVARIANTFILTERCUTOFFFREQUENCY   "TimeVariantFilterCutoffFrequency"
#define KEY_ADSR_ENVELOPERELEASETIME                "EnvelopeReleaseTime"
#define KEY_ADSR_EFFECTNAME                         "EffectName"


class DuAdsr : public DuEffectSettings
{
public:
    explicit DuAdsr();
    ~DuAdsr();

    static DuAdsr *fromDuMusicFile(const FX_adsr &du_adsr);
    static DuAdsr *fromJson(const QJsonObject &jsonAdsr);

    QByteArray toDuMusicFile();

    int getEnvelopeAttackTime() const;
    void setEnvelopeAttackTime(int value);

    int getEnvelopeDecayTime() const;
    void setEnvelopeDecayTime(int value);

    int getTimeVariantFilterCutoffResonance() const;
    void setTimeVariantFilterCutoffResonance(int value);

    int getTimeVariantFilterCutoffFrequency() const;
    void setTimeVariantFilterCutoffFrequency(int value);

    int getEnvelopeReleaseTime() const;
    void setEnvelopeReleaseTime(int value);

    QString getEffectName() const;
    void setEffectName(const QString &value);
};

#endif // DUADSR_H
