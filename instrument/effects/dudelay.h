#ifndef DUDELAY_H
#define DUDELAY_H

#include "dueffectsettings.h"


DU_OBJECT(DuDelay);

class DuDelay : public DuEffectSettings
{
public:
    DuDelay();
    virtual ~DuDelay() = default;

    virtual DuObjectPtr clone() const;

    static DuDelayPtr fromDuMusicBinary(const FX_delay &du_delay);

    QByteArray toDuMusicBinary() const;

    int size() const;


    DU_KEY_ACCESSORS(OnOff,            int)
    DU_KEY_ACCESSORS(Mode,             int)
    DU_KEY_ACCESSORS(PreLowPassFilter, int)
    DU_KEY_ACCESSORS(EffectLevel,      int)
    DU_KEY_ACCESSORS(EffectTime,       int)
    DU_KEY_ACCESSORS(Feedback,         int)
    DU_KEY_ACCESSORS(HDAmp,            int)
};

#endif // DUDELAY_H
