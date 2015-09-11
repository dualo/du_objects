#ifndef DUWAH_H
#define DUWAH_H

#include "dueffectsettings.h"


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


    DU_KEY_ACCESSORS(FilterType,         int)
    DU_KEY_ACCESSORS(FilterFrequency,    int)
    DU_KEY_ACCESSORS(FilterResonance,    int)
    DU_KEY_ACCESSORS(AutoWahSensitivity, int)
    DU_KEY_ACCESSORS(EffectName,         QString)
};

#endif // DUWAH_H
