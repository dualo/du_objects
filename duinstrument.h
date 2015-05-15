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


#define KEY_INSTR_INSTRUMENTINFO        "InstrumentInfo"
#define KEY_INSTR_PRESET                "Preset"
#define KEY_INSTR_MIXERSETTINGS         "MixerSettings"
#define KEY_INSTR_DISTORTIONSETTINGS    "DistortionSettings"
#define KEY_INSTR_WAHSETTINGS           "WahSettings"
#define KEY_INSTR_COMPRESSORSETTINGS    "CompressorSettings"
#define KEY_INSTR_EQUALIZERSETTINGS     "EqualizerSettings"
#define KEY_INSTR_DELAYSETTINGS         "DelaySettings"
#define KEY_INSTR_CHORUSSETTINGS        "ChorusSettings"
#define KEY_INSTR_VIBRATOSETTINGS       "VibratoSettings"
#define KEY_INSTR_ADSRSETTINGS          "AttackDecaySustainReleaseSettings"


class DuInstrument : public DuContainer
{
public:
    explicit DuInstrument();
    ~DuInstrument();

    static DuInstrument *fromDuMusicFile(const music_instr &du_instr);
    static DuInstrument *fromJson(const QJsonObject &jsonInstrument);

    int size() const;

    DuInstrumentInfo *getInstrumentInfo();
    void setInstrumentInfo(DuInstrumentInfo *instrumentInfo);

    DuPreset *getPreset();
    void setPreset(DuPreset *preset);

    DuMixer *getMixer();
    void setMixer(DuMixer *mixer);

    DuDistortion *getDistortion();
    void setDistortion(DuDistortion *distortion);

    DuWah *getWah();
    void setWah(DuWah *wah);

    DuCompressor *getCompressor();
    void setCompressor(DuCompressor *compressor);

    DuEqualizer *getEqualizer();
    void setEqualizer(DuEqualizer *equalizer);

    DuDelay *getDelay();
    void setDelay(DuDelay *delay);

    DuChorus *getChorus();
    void setChorus(DuChorus *chorus);

    DuVibrato *getVibrato();
    void setVibrato(DuVibrato *vibrato);

    DuAdsr *getAdsr();
    void setAdsr(DuAdsr *adsr);
};

#endif // DUINSTRUMENT_H
