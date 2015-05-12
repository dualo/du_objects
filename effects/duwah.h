#ifndef DUWAH_H
#define DUWAH_H

#include "dueffectsettings.h"


#define KEY_WAH_FILTERTYPE              "FilterType"
#define KEY_WAH_FILTERFREQUENCY         "FilterFrequency"
#define KEY_WAH_FILTERRESONANCE         "FilterResonance"
#define KEY_WAH_AUTOWAHSENSITIVITY      "AutoWahSensitivity"
#define KEY_WAH_EFFECTNAME              "EffectName"


class DuWah : public DuEffectSettings
{
public:
    explicit DuWah();
    ~DuWah();

    static DuWah *fromDuMusicFile(const FX_wah &du_wah);

    static DuWah *fromJson(const QJsonObject &jsonWah);

    int getFilterType() const;
    void setFilterType(int value);

    int getFilterFrequency() const;
    void setFilterFrequency(int value);

    int getFilterResonance() const;
    void setFilterResonance(int value);

    int getAutoWahSensitivity() const;
    void setAutoWahSensitivity(int value);

    QString getEffectName() const;
    void setEffectName(QString &value);
};

#endif // DUWAH_H
