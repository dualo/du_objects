#ifndef DUINSTRUMENT_H
#define DUINSTRUMENT_H

#include "../../general/ducontainer.h"


DU_OBJECT(DuInstrumentInfo);
DU_OBJECT(DuPreset);
DU_OBJECT(DuMixer);
DU_OBJECT(DuDistortion);
DU_OBJECT(DuWah);
DU_OBJECT(DuCompressor);
DU_OBJECT(DuEqualizer);
DU_OBJECT(DuDelay);
DU_OBJECT(DuChorus);
DU_OBJECT(DuVibrato);
DU_OBJECT(DuAdsr);

DU_OBJECT(DuInstrument);

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
