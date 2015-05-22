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

    QByteArray toDuMusicFile() const;

    int size() const;

    QSharedPointer<DuInstrumentInfo> getInstrumentInfo() const;
    void setInstrumentInfo(DuInstrumentInfo *instrumentInfo);

    QSharedPointer<DuPreset> getPreset() const;
    void setPreset(DuPreset *preset);

    QSharedPointer<DuMixer> getMixer() const;
    void setMixer(DuMixer *mixer);

    QSharedPointer<DuDistortion> getDistortion() const;
    void setDistortion(DuDistortion *distortion);

    QSharedPointer<DuWah> getWah() const;
    void setWah(DuWah *wah);

    QSharedPointer<DuCompressor> getCompressor() const;
    void setCompressor(DuCompressor *compressor);

    QSharedPointer<DuEqualizer> getEqualizer() const;
    void setEqualizer(DuEqualizer *equalizer);

    QSharedPointer<DuDelay> getDelay() const;
    void setDelay(DuDelay *delay);

    QSharedPointer<DuChorus> getChorus() const;
    void setChorus(DuChorus *chorus);

    QSharedPointer<DuVibrato> getVibrato() const;
    void setVibrato(DuVibrato *vibrato);

    QSharedPointer<DuAdsr> getAdsr() const;
    void setAdsr(DuAdsr *adsr);
};

#endif // DUINSTRUMENT_H
