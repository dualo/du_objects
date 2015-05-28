#ifndef DUWAH_H
#define DUWAH_H

#include "dueffectsettings.h"


#define KEY_WAH_FILTERTYPE              "FilterType"
#define KEY_WAH_FILTERFREQUENCY         "FilterFrequency"
#define KEY_WAH_FILTERRESONANCE         "FilterResonance"
#define KEY_WAH_AUTOWAHSENSITIVITY      "AutoWahSensitivity"
#define KEY_WAH_EFFECTNAME              "EffectName"


DU_OBJECT(DuWah)

class DuWah : public DuEffectSettings
{
public:
    explicit DuWah();
    ~DuWah();

    virtual DuObjectPtr clone() const;

    static DuWahPtr fromDuMusicBinary(const FX_wah &du_wah);
    static DuWahPtr fromJson(const QJsonObject &jsonWah);

    QByteArray toDuMusicBinary() const;

    int size() const;

    int getFilterType() const;
    bool setFilterType(int value);

    int getFilterFrequency() const;
    bool setFilterFrequency(int value);

    int getFilterResonance() const;
    bool setFilterResonance(int value);

    int getAutoWahSensitivity() const;
    bool setAutoWahSensitivity(int value);

    QString getEffectName() const;
    bool setEffectName(const QString &value);
};

#endif // DUWAH_H
