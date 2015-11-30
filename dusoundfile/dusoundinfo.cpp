#include "dusoundinfo.h"

#include "../dumusicfile/instrument/duinstrumentinfo.h"
#include "../dumusicfile/instrument/dupreset.h"

#include "../dumusicfile/instrument/effects/duchorus.h"
#include "../dumusicfile/instrument/effects/ducompressor.h"
#include "../dumusicfile/instrument/effects/dudelay.h"
#include "../dumusicfile/instrument/effects/dudistortion.h"
#include "../dumusicfile/instrument/effects/duequalizer.h"
#include "../dumusicfile/instrument/effects/dumixer.h"

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
    addChild(KeyCompressorArray, new DuArray(FX_NUM_FX_INTR));
    addChild(KeyEqualizerArray,  new DuArray(FX_NUM_FX_INTR));
    addChild(KeyDelayArray,      new DuArray(FX_NUM_FX_INTR));
    addChild(KeyChorusArray,     new DuArray(FX_NUM_FX_INTR));

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

QByteArray DuSoundInfo::toBinary(uint32_t sampleAddress, uint8_t nbLayer, int nbSamples, uint32_t sampleSize) const
{
    int tmpInt;

    struct_instr soundStruct;
    std::memcpy((char*)&soundStruct, QByteArray(size(), 0), size());

    const DuInstrumentInfoConstPtr &m3infos = getInstrumentInfo();
    if (m3infos == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct.s_instrument), m3infos->toBinary(sampleAddress, nbLayer, nbSamples, sampleSize).constData(), m3infos->size());

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
    std::memcpy((char*)&(soundStruct.s_preset), presetArray->toDuMusicBinary().constData(), presetArray->size());

    const DuMixerConstPtr &mixer = getMixer();
    if (mixer == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct.s_mix), mixer->toDuMusicBinary().constData(), mixer->size());

    const DuArrayConstPtr &distortionArray = getDistortionArray();
    if (distortionArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct.s_distortion), distortionArray->toDuMusicBinary().constData(), distortionArray->size());

    const DuArrayConstPtr &compressorArray = getCompressorArray();
    if (compressorArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct.s_compressor), compressorArray->toDuMusicBinary().constData(), compressorArray->size());

    const DuArrayConstPtr &equalizerArray = getEqualizerArray();
    if (equalizerArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct.s_equalizer), equalizerArray->toDuMusicBinary().constData(), equalizerArray->size());

    const DuArrayConstPtr &delayArray = getDelayArray();
    if (delayArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct.s_delay), delayArray->toDuMusicBinary().constData(), delayArray->size());

    const DuArrayConstPtr &chorusArray = getChorusArray();
    if (chorusArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct.s_chorus), chorusArray->toDuMusicBinary().constData(), chorusArray->size());

    const DuArrayConstPtr &ledsArray = getLedArray();
    if (ledsArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct.s_leds), ledsArray->toDuMusicBinary().constData(), ledsArray->size());

    return QByteArray((char*)&soundStruct, size());
}

DuObjectPtr DuSoundInfo::getChild(const QString &key)
{
    if (    key == KeyName               ||
            key == KeyDreamProgramChange ||
            key == KeyMidiControlChange0 ||
            key == KeyKeyMapping         ||
            key == KeyOctave             ||
            key == KeyUserID             ||
            key == KeyID                 ||
            key == KeyActiveNoteOff      ||
            key == KeyCategory           ||
            key == KeyRelativeVolume     ||
            key == KeyInstrType          ||
            key == KeyInstrVersion       ||
            key == KeyHardInstrVersion   ||
            key == KeySoftInstrVersion)
    {
        DuInstrumentInfoPtr info = getInstrumentInfo();
        if (info == NULL)
        {
            return DuObjectPtr();
        }

        return info->getChild(key);
    }
    else
    {
        return DuContainer::getChild(key);
    }
}

DuObjectConstPtr DuSoundInfo::getChild(const QString &key) const
{
    if (    key == KeyName               ||
            key == KeyDreamProgramChange ||
            key == KeyMidiControlChange0 ||
            key == KeyKeyMapping         ||
            key == KeyOctave             ||
            key == KeyUserID             ||
            key == KeyID                 ||
            key == KeyActiveNoteOff      ||
            key == KeyCategory           ||
            key == KeyRelativeVolume     ||
            key == KeyInstrType          ||
            key == KeyInstrVersion       ||
            key == KeyHardInstrVersion   ||
            key == KeySoftInstrVersion)
    {
        DuInstrumentInfoConstPtr info = getInstrumentInfo();
        if (info == NULL)
        {
            return DuObjectPtr();
        }

        return info->getChild(key);
    }
    else
    {
        return DuContainer::getChild(key);
    }
}

DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, Name,               DuInstrumentInfo, InstrumentInfo, QString, QString())
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, DreamProgramChange, DuInstrumentInfo, InstrumentInfo, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, MidiControlChange0, DuInstrumentInfo, InstrumentInfo, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, KeyMapping,         DuInstrumentInfo, InstrumentInfo, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, Octave,             DuInstrumentInfo, InstrumentInfo, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, UserID,             DuInstrumentInfo, InstrumentInfo, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, ID,                 DuInstrumentInfo, InstrumentInfo, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, ActiveNoteOff,      DuInstrumentInfo, InstrumentInfo, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, Category,           DuInstrumentInfo, InstrumentInfo, QString, QString())
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, RelativeVolume,     DuInstrumentInfo, InstrumentInfo, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, InstrType,          DuInstrumentInfo, InstrumentInfo, INSTRUMENT_TYPE, NUM_INSTR_TYPE)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, InstrVersion,       DuInstrumentInfo, InstrumentInfo, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, HardInstrVersion,   DuInstrumentInfo, InstrumentInfo, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, SoftInstrVersion,   DuInstrumentInfo, InstrumentInfo, int, -1)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSoundInfo, InstrumentInfo,  DuInstrumentInfo)

DU_KEY_ACCESSORS_IMPL(DuSoundInfo, PresetNum,  Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSoundInfo, DisplayLed, Numeric, int, -1)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSoundInfo, PresetArray,     DuArray)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSoundInfo, Mixer,           DuMixer)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuSoundInfo, DistortionArray, DuArray)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuSoundInfo, CompressorArray, DuArray)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuSoundInfo, EqualizerArray,  DuArray)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuSoundInfo, DelayArray,      DuArray)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuSoundInfo, ChorusArray,     DuArray)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSoundInfo, LedArray,        DuArray)
