#include "dusoundinfo.h"

#include "../dumusicfile/instrument/duinstrumentinfo.h"
#include "../dumusicfile/instrument/dupreset.h"

#include "../dumusicfile/instrument/effects/duadsr.h"
#include "../dumusicfile/instrument/effects/duchorus.h"
#include "../dumusicfile/instrument/effects/ducompressor.h"
#include "../dumusicfile/instrument/effects/dudelay.h"
#include "../dumusicfile/instrument/effects/dudistortion.h"
#include "../dumusicfile/instrument/effects/duequalizer.h"
#include "../dumusicfile/instrument/effects/dumixer.h"
#include "../dumusicfile/instrument/effects/duvibrato.h"
#include "../dumusicfile/instrument/effects/duwah.h"

#include "../general/duarray.h"
#include "../general/dunumeric.h"

#include <cstring>

DuSoundInfo::DuSoundInfo()
{
    addChild(KeyInstrumentInfo,  new DuInstrumentInfo);

    addChild(KeyPresetNum,       new DuNumeric(0, NUMERIC_DEFAULT_SIZE, FX_NUM_PRESET_INTR, 0));
    addChild(KeyDisplayLed,      new DuNumeric(0xFF, NUMERIC_DEFAULT_SIZE, 0xFF, 0));

    addChild(KeyPresetArray,     new DuArray(FX_NUM_PRESET_INTR));

    addChild(KeyMixer,           new DuMixer);
    addChild(KeyDistortionArray, new DuArray(FX_NUM_FX_INTR));
    addChild(KeyWahArray,        new DuArray(FX_NUM_FX_INTR));
    addChild(KeyCompressorArray, new DuArray(FX_NUM_FX_INTR));
    addChild(KeyEqualizerArray,  new DuArray(FX_NUM_FX_INTR));
    addChild(KeyDelayArray,      new DuArray(FX_NUM_FX_INTR));
    addChild(KeyChorusArray,     new DuArray(FX_NUM_FX_INTR));
    addChild(KeyVibratoArray,    new DuArray(FX_NUM_FX_INTR));
    addChild(KeyAdsrArray,       new DuArray(FX_NUM_FX_INTR));

    addChild(KeyLedArray,        new DuArray(NUM_LED_VALUE));
}

DuObjectPtr DuSoundInfo::clone() const
{
    return DuSoundInfoPtr(new DuSoundInfo(*this));
}

int DuSoundInfo::size() const
{
    return INSTRU_STRUCT_SIZE;
}

DuSoundInfoPtr DuSoundInfo::fromBinary(const struct_instr &data)
{
    DuSoundInfoPtr soundInfo(new DuSoundInfo);

    DuInstrumentInfoPtr m3Infos = DuInstrumentInfo::fromDuMusicBinary(data.s_instrument);
    if (m3Infos != NULL)
    {
        soundInfo->setInstrumentInfo(m3Infos);
    }
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound info:\n"
                                      << "M3 infos were not properly generated";

        return DuSoundInfoPtr();
    }

    bool verif = true;

    verif = soundInfo->setPresetNum(data.s_presetnum)       ? verif : false;
    verif = soundInfo->setDisplayLed(data.s_displayled)     ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "An attribute was not properly set";
    }

    DuArrayPtr presetArray(new DuArray);
    for (int i = 0; i < FX_NUM_PRESET_INTR; ++i)
    {
        DuPresetPtr preset = DuPreset::fromDuMusicBinary(data.s_preset[i]);
        if (preset != NULL)
        {
            presetArray->append(preset);
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound info:\n"
                                          << "Preset" << i << "was not properly generated";

            return DuSoundInfoPtr();
        }
    }
    soundInfo->setPresetArray(presetArray);

    DuMixerPtr mixer = DuMixer::fromDuMusicBinary(data.s_mix);
    if (mixer != NULL)
    {
        soundInfo->setMixer(mixer);
    }
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound info:\n"
                                      << "Mixer was not properly generated";

        return DuSoundInfoPtr();
    }

    DuArrayPtr distoArray(new DuArray);
    for (int i = 0; i < FX_NUM_FX_INTR; ++i)
    {
        DuDistortionPtr disto = DuDistortion::fromDuMusicBinary(data.s_distortion[i]);
        if (disto != NULL)
        {
            distoArray->append(disto);
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound info:\n"
                                          << "Distortion" << i << "was not properly generated";

            return DuSoundInfoPtr();
        }
    }
    soundInfo->setDistortionArray(distoArray);

    DuArrayPtr wahArray(new DuArray);
    for (int i = 0; i < FX_NUM_FX_INTR; ++i)
    {
        DuWahPtr wah = DuWah::fromDuMusicBinary(data.s_wah[i]);
        if (wah != NULL)
        {
            wahArray->append(wah);
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound info:\n"
                                          << "Wah" << i << "was not properly generated";

            return DuSoundInfoPtr();
        }
    }
    soundInfo->setWahArray(wahArray);

    DuArrayPtr compressorArray(new DuArray);
    for (int i = 0; i < FX_NUM_FX_INTR; ++i)
    {
        DuCompressorPtr compressor = DuCompressor::fromDuMusicBinary(data.s_compressor[i]);
        if (compressor != NULL)
        {
            compressorArray->append(compressor);
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound info:\n"
                                          << "Compressor" << i << "was not properly generated";

            return DuSoundInfoPtr();
        }
    }
    soundInfo->setCompressorArray(compressorArray);

    DuArrayPtr equalizerArray(new DuArray);
    for (int i = 0; i < FX_NUM_FX_INTR; ++i)
    {
        DuEqualizerPtr equalizer = DuEqualizer::fromDuMusicBinary(data.s_equalizer[i]);
        if (equalizer != NULL)
        {
            equalizerArray->append(equalizer);
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound info:\n"
                                          << "Equalizer" << i << "was not properly generated";

            return DuSoundInfoPtr();
        }
    }
    soundInfo->setEqualizerArray(equalizerArray);

    DuArrayPtr delayArray(new DuArray);
    for (int i = 0; i < FX_NUM_FX_INTR; ++i)
    {
        DuDelayPtr delay = DuDelay::fromDuMusicBinary(data.s_delay[i]);
        if (delay != NULL)
        {
            delayArray->append(delay);
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound info:\n"
                                          << "Delay" << i << "was not properly generated";

            return DuSoundInfoPtr();
        }
    }
    soundInfo->setDelayArray(delayArray);

    DuArrayPtr chorusArray(new DuArray);
    for (int i = 0; i < FX_NUM_FX_INTR; ++i)
    {
        DuChorusPtr chorus = DuChorus::fromDuMusicBinary(data.s_chorus[i]);
        if (chorus != NULL)
        {
            chorusArray->append(chorus);
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound info:\n"
                                          << "Chorus" << i << "was not properly generated";

            return DuSoundInfoPtr();
        }
    }
    soundInfo->setChorusArray(chorusArray);

    DuArrayPtr vibratoArray(new DuArray);
    for (int i = 0; i < FX_NUM_FX_INTR; ++i)
    {
        DuVibratoPtr vibrato = DuVibrato::fromDuMusicBinary(data.s_vibrato[i]);
        if (vibrato != NULL)
        {
            vibratoArray->append(vibrato);
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound info:\n"
                                          << "Vibrato" << i << "was not properly generated";

            return DuSoundInfoPtr();
        }
    }
    soundInfo->setVibratoArray(vibratoArray);

    DuArrayPtr adsrArray(new DuArray);
    for (int i = 0; i < FX_NUM_FX_INTR; ++i)
    {
        DuAdsrPtr adsr = DuAdsr::fromDuMusicBinary(data.s_adsr[i]);
        if (adsr != NULL)
        {
            adsrArray->append(adsr);
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound info:\n"
                                          << "ADSR" << i << "was not properly generated";

            return DuSoundInfoPtr();
        }
    }
    soundInfo->setAdsrArray(adsrArray);

    DuArrayPtr ledsArray(new DuArray);
    for (int i = 0; i < NUM_LED_VALUE; ++i)
    {
        DuNumericPtr leds(new DuNumeric(data.s_leds[i], NUMERIC_DEFAULT_SIZE, 0xFF, 0x000, 0));
        if (leds != NULL)
        {
            ledsArray->append(leds);
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound info:\n"
                                          << "Leds" << i << "was not properly generated";

            return DuSoundInfoPtr();
        }
    }
    soundInfo->setLedArray(ledsArray);

    return soundInfo;
}

QByteArray DuSoundInfo::toBinary() const
{
    int tmpInt;

    struct_instr soundStruct;
    std::memcpy((char*)&soundStruct, QByteArray(size(), 0), size());

    const DuInstrumentInfoConstPtr &m3infos = getInstrumentInfo();
    if (m3infos == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct.s_instrument), m3infos->toDuMusicBinary().data(), m3infos->size());

    tmpInt = getPresetNum();
    if (tmpInt == -1)
        return QByteArray();
    soundStruct.s_presetnum = tmpInt;

    tmpInt = getDisplayLed();
    if (tmpInt == -1)
        return QByteArray();
    soundStruct.s_displayled = tmpInt;

    const DuArrayConstPtr &presetArray = getPresetArray();
    if (presetArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct.s_preset), presetArray->toDuMusicBinary().data(), presetArray->size());

    const DuMixerConstPtr &mixer = getMixer();
    if (mixer == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct.s_mix), mixer->toDuMusicBinary().data(), mixer->size());

    const DuArrayConstPtr &distortionArray = getDistortionArray();
    if (distortionArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct.s_distortion), distortionArray->toDuMusicBinary().data(), distortionArray->size());

    const DuArrayConstPtr &wahArray = getWahArray();
    if (wahArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct.s_wah), wahArray->toDuMusicBinary().data(), wahArray->size());

    const DuArrayConstPtr &compressorArray = getCompressorArray();
    if (compressorArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct.s_compressor), compressorArray->toDuMusicBinary().data(), compressorArray->size());

    const DuArrayConstPtr &equalizerArray = getEqualizerArray();
    if (equalizerArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct.s_equalizer), equalizerArray->toDuMusicBinary().data(), equalizerArray->size());

    const DuArrayConstPtr &delayArray = getDelayArray();
    if (delayArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct.s_delay), delayArray->toDuMusicBinary().data(), delayArray->size());

    const DuArrayConstPtr &chorusArray = getChorusArray();
    if (chorusArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct.s_chorus), chorusArray->toDuMusicBinary().data(), chorusArray->size());

    const DuArrayConstPtr &vibratoArray = getVibratoArray();
    if (vibratoArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct.s_vibrato), vibratoArray->toDuMusicBinary().data(), vibratoArray->size());

    const DuArrayConstPtr &adsrArray = getAdsrArray();
    if (adsrArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct.s_adsr), adsrArray->toDuMusicBinary().data(), adsrArray->size());

    const DuArrayConstPtr &ledsArray = getLedArray();
    if (ledsArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct.s_leds), ledsArray->toDuMusicBinary().data(), ledsArray->size());

    return QByteArray((char*)&soundStruct, size());
}

QString DuSoundInfo::name() const
{
    const DuInstrumentInfoConstPtr& instrInfo = getInstrumentInfo();

    if (instrInfo == NULL)
    {
        return QString();
    }

    return instrInfo->getName();
}

bool DuSoundInfo::setName(const QString &name)
{
    const DuInstrumentInfoPtr &instrInfo = getChildAs<DuInstrumentInfo>(KeyInstrumentInfo);

    if (instrInfo == NULL)
    {
        return false;
    }

    return instrInfo->setName(name);
}

int DuSoundInfo::dreamProgramChange() const
{
    const DuInstrumentInfoConstPtr& instrInfo = getInstrumentInfo();

    if (instrInfo == NULL)
    {
        return -1;
    }

    return instrInfo->getDreamProgramChange();
}

bool DuSoundInfo::setDreamProgramChange(int value)
{
    const DuInstrumentInfoPtr &instrInfo = getChildAs<DuInstrumentInfo>(KeyInstrumentInfo);

    if (instrInfo == NULL)
    {
        return false;
    }

    return instrInfo->setDreamProgramChange(value);
}

int DuSoundInfo::octave() const
{
    const DuInstrumentInfoConstPtr& instrInfo = getInstrumentInfo();

    if (instrInfo == NULL)
    {
        return -1;
    }

    return instrInfo->getOctave();
}

bool DuSoundInfo::setOctave(int value)
{
    const DuInstrumentInfoPtr &instrInfo = getChildAs<DuInstrumentInfo>(KeyInstrumentInfo);

    if (instrInfo == NULL)
    {
        return false;
    }

    return instrInfo->setOctave(value);
}

int DuSoundInfo::activeNoteOff() const
{
    const DuInstrumentInfoConstPtr& instrInfo = getInstrumentInfo();

    if (instrInfo == NULL)
    {
        return -1;
    }

    return instrInfo->getActiveNoteOff();
}

bool DuSoundInfo::setActiveNoteOff(int value)
{
    const DuInstrumentInfoPtr &instrInfo = getChildAs<DuInstrumentInfo>(KeyInstrumentInfo);

    if (instrInfo == NULL)
    {
        return false;
    }

    return instrInfo->setActiveNoteOff(value);
}

QString DuSoundInfo::category() const
{
    const DuInstrumentInfoConstPtr& instrInfo = getInstrumentInfo();

    if (instrInfo == NULL)
    {
        return QString();
    }

    return instrInfo->getCategory();
}

bool DuSoundInfo::setCategory(const QString &value)
{
    const DuInstrumentInfoPtr &instrInfo = getChildAs<DuInstrumentInfo>(KeyInstrumentInfo);

    if (instrInfo == NULL)
    {
        return false;
    }

    return instrInfo->setCategory(value);
}

int DuSoundInfo::relativeVolume() const
{
    const DuInstrumentInfoConstPtr& instrInfo = getInstrumentInfo();

    if (instrInfo == NULL)
    {
        return -1;
    }

    return instrInfo->getRelativeVolume();
}

bool DuSoundInfo::setRelativeVolume(int value)
{
    const DuInstrumentInfoPtr &instrInfo = getChildAs<DuInstrumentInfo>(KeyInstrumentInfo);

    if (instrInfo == NULL)
    {
        return false;
    }

    return instrInfo->setRelativeVolume(value);
}

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSoundInfo, InstrumentInfo,  DuInstrumentInfo)

DU_KEY_ACCESSORS_IMPL(DuSoundInfo, PresetNum,  Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSoundInfo, DisplayLed, Numeric, int, -1)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSoundInfo, PresetArray,     DuArray)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSoundInfo, Mixer,           DuMixer)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuSoundInfo, DistortionArray, DuArray)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuSoundInfo, WahArray,        DuArray)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuSoundInfo, CompressorArray, DuArray)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuSoundInfo, EqualizerArray,  DuArray)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuSoundInfo, DelayArray,      DuArray)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuSoundInfo, ChorusArray,     DuArray)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuSoundInfo, VibratoArray,    DuArray)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuSoundInfo, AdsrArray,       DuArray)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSoundInfo, LedArray,        DuArray)
