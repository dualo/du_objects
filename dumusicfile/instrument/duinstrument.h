#ifndef DUINSTRUMENT_H
#define DUINSTRUMENT_H

#include "duinstrumentinfo.h"
#include "dupreset.h"
#include "effects/dumixer.h"
#include "effects/dudistortion.h"
#include "effects/duwah.h"
#include "effects/ducompressor.h"
#include "effects/duequalizer.h"
#include "effects/dudelay.h"
#include "effects/duchorus.h"
#include "effects/duvibrato.h"
#include "effects/duadsr.h"


DU_OBJECT(DuInstrument)

class DuInstrument : public DuContainer
{
public:
    explicit DuInstrument();
    ~DuInstrument();

    virtual DuObjectPtr clone() const;

    static DuInstrumentPtr fromDuMusicBinary(const music_instr &du_instr);
    static DuInstrumentPtr fromJson(const QJsonObject &jsonInstrument);

    QByteArray toDuMusicBinary() const;

    int size() const;

    DU_KEY_ACCESSORS_OBJECT(InstrumentInfo, DuInstrumentInfo)
    DU_KEY_ACCESSORS_OBJECT(Preset,         DuPreset)
    DU_KEY_ACCESSORS_OBJECT(Mixer,          DuMixer)
    DU_KEY_ACCESSORS_OBJECT(Distortion,     DuDistortion)
    DU_KEY_ACCESSORS_OBJECT(Wah,            DuWah)
    DU_KEY_ACCESSORS_OBJECT(Compressor,     DuCompressor)
    DU_KEY_ACCESSORS_OBJECT(Equalizer,      DuEqualizer)
    DU_KEY_ACCESSORS_OBJECT(Delay,          DuDelay)
    DU_KEY_ACCESSORS_OBJECT(Chorus,         DuChorus)
    DU_KEY_ACCESSORS_OBJECT(Vibrato,        DuVibrato)
    DU_KEY_ACCESSORS_OBJECT(Adsr,           DuAdsr)
};

#endif // DUINSTRUMENT_H
