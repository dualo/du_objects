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

    DuInstrumentInfoConstPtr getInstrumentInfo() const;
    void setInstrumentInfo(const DuInstrumentInfoPtr &instrumentInfo);

    DuPresetConstPtr getPreset() const;
    void setPreset(const DuPresetPtr& preset);

    DuMixerConstPtr getMixer() const;
    void setMixer(const DuMixerPtr& mixer);

    DuDistortionConstPtr getDistortion() const;
    void setDistortion(const DuDistortionPtr& distortion);

    DuWahConstPtr getWah() const;
    void setWah(const DuWahPtr& wah);

    DuCompressorConstPtr getCompressor() const;
    void setCompressor(const DuCompressorPtr& compressor);

    DuEqualizerConstPtr getEqualizer() const;
    void setEqualizer(const DuEqualizerPtr& equalizer);

    DuDelayConstPtr getDelay() const;
    void setDelay(const DuDelayPtr& delay);

    DuChorusConstPtr getChorus() const;
    void setChorus(const DuChorusPtr& chorus);

    DuVibratoConstPtr getVibrato() const;
    void setVibrato(const DuVibratoPtr& vibrato);

    DuAdsrConstPtr getAdsr() const;
    void setAdsr(const DuAdsrPtr& adsr);
};

#endif // DUINSTRUMENT_H
