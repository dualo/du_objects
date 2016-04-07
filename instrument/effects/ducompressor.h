#ifndef DUCOMPRESSOR_H
#define DUCOMPRESSOR_H

#include "dueffectsettings.h"


DU_OBJECT(DuCompressor);

class DuCompressor : public DuEffectSettings
{
public:
    explicit DuCompressor();
    ~DuCompressor();

    virtual DuObjectPtr clone() const;

    static DuCompressorPtr fromDuMusicBinary(const FX_compressor &du_compressor);

    QByteArray toDuMusicBinary() const;

    int size() const;


    DU_KEY_ACCESSORS(OnOff,       int)
    DU_KEY_ACCESSORS(AttackTime,  int)
    DU_KEY_ACCESSORS(ReleaseTime, int)
    DU_KEY_ACCESSORS(Threshold,   int)
    DU_KEY_ACCESSORS(Ratio,       int)
    DU_KEY_ACCESSORS(Boost,       int)
    DU_KEY_ACCESSORS(KneeType,    int)
};

#endif // DUCOMPRESSOR_H
