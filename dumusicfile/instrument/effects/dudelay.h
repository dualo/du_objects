#ifndef DUDELAY_H
#define DUDELAY_H

#include "dueffectsettings.h"


DU_OBJECT(DuDelay)

class DuDelay : public DuEffectSettings
{
public:
    explicit DuDelay();
    ~DuDelay();

    virtual DuObjectPtr clone() const;

    static DuDelayPtr fromDuMusicBinary(const FX_delay &du_delay);
    static DuDelayPtr fromJson(const QJsonObject &jsonDelay);

    QByteArray toDuMusicBinary() const;

    int size() const;


    DU_KEY_ACCESSORS(OnOff,            int)
    DU_KEY_ACCESSORS(Mode,             int)
    DU_KEY_ACCESSORS(PreLowPassFilter, int)
    DU_KEY_ACCESSORS(EffectLevel,      int)
    DU_KEY_ACCESSORS(EffectTime,       int)
    DU_KEY_ACCESSORS(Feedback,         int)
    DU_KEY_ACCESSORS(HDAmp,            int)
    DU_KEY_ACCESSORS(EffectName,       QString)
};

#endif // DUDELAY_H
