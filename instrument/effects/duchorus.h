#ifndef DUCHORUS_H
#define DUCHORUS_H

#include "dueffectsettings.h"


DU_OBJECT(DuChorus);

class DuChorus : public DuEffectSettings
{
public:
    explicit DuChorus();
    ~DuChorus();

    virtual DuObjectPtr clone() const;

    static DuChorusPtr fromDuMusicBinary(const FX_chorus &du_chorus);

    QByteArray toDuMusicBinary() const;

    int size() const;


    DU_KEY_ACCESSORS(Mode,                         int)
    DU_KEY_ACCESSORS(EffectLevel,                  int)
    DU_KEY_ACCESSORS(DelayTime,                    int)
    DU_KEY_ACCESSORS(Feedback,                     int)
    DU_KEY_ACCESSORS(InputHighPassFilterFrequency, int)
    DU_KEY_ACCESSORS(HDAmp,                        int)
    DU_KEY_ACCESSORS(ModulationDepth,              int)
    DU_KEY_ACCESSORS(ModulationRate,               int)
    DU_KEY_ACCESSORS(TremoloShape,                 int)
    DU_KEY_ACCESSORS(RotarySpeed,                  int)
};

#endif // DUCHORUS_H
