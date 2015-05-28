#include "duinstrument.h"

#include <QJsonObject>
#include <QDebug>

DU_OBJECT_IMPL(DuInstrument)

DuInstrument::DuInstrument() :
    DuContainer()
{
    addChild(KEY_INSTR_INSTRUMENTINFO, new DuInstrumentInfo());
    addChild(KEY_INSTR_PRESET, new DuPreset());

    addChild(KEY_INSTR_MIXERSETTINGS, new DuMixer());
    addChild(KEY_INSTR_DISTORTIONSETTINGS, new DuDistortion());
    addChild(KEY_INSTR_WAHSETTINGS, new DuWah());
    addChild(KEY_INSTR_COMPRESSORSETTINGS, new DuCompressor());
    addChild(KEY_INSTR_EQUALIZERSETTINGS, new DuEqualizer());
    addChild(KEY_INSTR_DELAYSETTINGS, new DuDelay());
    addChild(KEY_INSTR_CHORUSSETTINGS, new DuChorus());
    addChild(KEY_INSTR_VIBRATOSETTINGS, new DuVibrato());
    addChild(KEY_INSTR_ADSRSETTINGS, new DuAdsr());
}

DuInstrument::~DuInstrument()
{
}

DuObjectPtr DuInstrument::clone() const
{
    return DuInstrumentPtr(new DuInstrument(*this));
}


DuInstrumentPtr DuInstrument::fromDuMusicFile(const music_instr &du_instr)
{
    DuInstrumentPtr instrument(new DuInstrument);

    const DuInstrumentInfoPtr& instrInfo =
            DuInstrumentInfo::fromDuMusicFile(du_instr.i_instrument);
    if (instrInfo == NULL)
    {
        return DuInstrumentPtr();
    }
    instrument->setInstrumentInfo(instrInfo);

    const DuPresetPtr& preset = DuPreset::fromDuMusicFile(du_instr.i_preset);
    if (preset == NULL)
    {
        return DuInstrumentPtr();
    }
    instrument->setPreset(preset);

    const DuMixerPtr& mixer = DuMixer::fromDuMusicFile(du_instr.i_mix);
    if (mixer == NULL)
    {
        return DuInstrumentPtr();
    }
    instrument->setMixer(mixer);

    const DuDistortionPtr& distortion = DuDistortion::fromDuMusicFile(du_instr.i_distortion);
    if (distortion == NULL)
    {
        return DuInstrumentPtr();
    }
    instrument->setDistortion(distortion);

    const DuWahPtr& wah = DuWah::fromDuMusicFile(du_instr.i_wah);
    if (wah == NULL)
    {
        return DuInstrumentPtr();
    }
    instrument->setWah(wah);

    const DuCompressorPtr& compressor = DuCompressor::fromDuMusicFile(du_instr.i_compressor);
    if (compressor == NULL)
    {
        return DuInstrumentPtr();
    }
    instrument->setCompressor(compressor);

    const DuEqualizerPtr& equalizer = DuEqualizer::fromDuMusicFile(du_instr.i_equalizer);
    if (equalizer == NULL)
    {
        return DuInstrumentPtr();
    }
    instrument->setEqualizer(equalizer);

    const DuDelayPtr& delay = DuDelay::fromDuMusicFile(du_instr.i_delay);
    if (delay == NULL)
    {
        return DuInstrumentPtr();
    }
    instrument->setDelay(delay);

    const DuChorusPtr& chorus = DuChorus::fromDuMusicFile(du_instr.i_chorus);
    if (chorus == NULL)
    {
        return DuInstrumentPtr();
    }
    instrument->setChorus(chorus);

    const DuVibratoPtr& vibrato = DuVibrato::fromDuMusicFile(du_instr.i_vibrato);
    if (vibrato == NULL)
    {
        return DuInstrumentPtr();
    }
    instrument->setVibrato(vibrato);

    const DuAdsrPtr& adsr = DuAdsr::fromDuMusicFile(du_instr.i_adsr);
    if (adsr == NULL)
    {
        return DuInstrumentPtr();
    }
    instrument->setAdsr(adsr);

    return instrument;
}


DuInstrumentPtr DuInstrument::fromJson(const QJsonObject &jsonInstrument)
{
    QJsonValue jsonInstrInfo    = jsonInstrument[KEY_INSTR_INSTRUMENTINFO];
    QJsonValue jsonPreset       = jsonInstrument[KEY_INSTR_PRESET];
    QJsonValue jsonMixer        = jsonInstrument[KEY_INSTR_MIXERSETTINGS];
    QJsonValue jsonDistortion   = jsonInstrument[KEY_INSTR_DISTORTIONSETTINGS];
    QJsonValue jsonWah          = jsonInstrument[KEY_INSTR_WAHSETTINGS];
    QJsonValue jsonCompressor   = jsonInstrument[KEY_INSTR_COMPRESSORSETTINGS];
    QJsonValue jsonEqualizer    = jsonInstrument[KEY_INSTR_EQUALIZERSETTINGS];
    QJsonValue jsonDelay        = jsonInstrument[KEY_INSTR_DELAYSETTINGS];
    QJsonValue jsonChorus       = jsonInstrument[KEY_INSTR_CHORUSSETTINGS];
    QJsonValue jsonVibrato      = jsonInstrument[KEY_INSTR_VIBRATOSETTINGS];
    QJsonValue jsonAdsr         = jsonInstrument[KEY_INSTR_ADSRSETTINGS];

    if (        !jsonInstrInfo.isObject()   ||  !jsonPreset.isObject()
            ||  !jsonMixer.isObject()       ||  !jsonDistortion.isObject()
            ||  !jsonWah.isObject()         ||  !jsonCompressor.isObject()
            ||  !jsonEqualizer.isObject()   ||  !jsonDelay.isObject()
            ||  !jsonChorus.isObject()      ||  !jsonVibrato.isObject()
            ||  !jsonAdsr.isObject())

        return DuInstrumentPtr();


    DuInstrumentPtr instrument(new DuInstrument);

    const DuInstrumentInfoPtr& instrInfo =
            DuInstrumentInfo::fromJson(jsonInstrInfo.toObject());
    if (instrInfo != NULL)
        instrument->setInstrumentInfo(instrInfo);
    else
    {
        return DuInstrumentPtr();
    }

    const DuPresetPtr& preset = DuPreset::fromJson(jsonPreset.toObject());
    if (preset != NULL)
        instrument->setPreset(preset);
    else
    {
        return DuInstrumentPtr();
    }

    const DuMixerPtr& mixer = DuMixer::fromJson(jsonMixer.toObject());
    if (mixer != NULL)
        instrument->setMixer(mixer);
    else
    {
        return DuInstrumentPtr();
    }

    const DuDistortionPtr& distortion = DuDistortion::fromJson(jsonDistortion.toObject());
    if (distortion != NULL)
        instrument->setDistortion(distortion);
    else
    {
        return DuInstrumentPtr();
    }

    const DuWahPtr& wah = DuWah::fromJson(jsonWah.toObject());
    if (wah != NULL)
        instrument->setWah(wah);
    else
    {
        return DuInstrumentPtr();
    }

    const DuCompressorPtr& compressor = DuCompressor::fromJson(jsonCompressor.toObject());
    if (compressor != NULL)
        instrument->setCompressor(compressor);
    else
    {
        return DuInstrumentPtr();
    }

    const DuEqualizerPtr& equalizer = DuEqualizer::fromJson(jsonEqualizer.toObject());
    if (equalizer != NULL)
        instrument->setEqualizer(equalizer);
    else
    {
        return DuInstrumentPtr();
    }

    const DuDelayPtr& delay = DuDelay::fromJson(jsonDelay.toObject());
    if (delay != NULL)
        instrument->setDelay(delay);
    else
    {
        return DuInstrumentPtr();
    }

    const DuChorusPtr& chorus = DuChorus::fromJson(jsonChorus.toObject());
    if (chorus != NULL)
        instrument->setChorus(chorus);
    else
    {
        return DuInstrumentPtr();
    }

    const DuVibratoPtr& vibrato = DuVibrato::fromJson(jsonVibrato.toObject());
    if (vibrato != NULL)
        instrument->setVibrato(vibrato);
    else
    {
        return DuInstrumentPtr();
    }

    const DuAdsrPtr& adsr = DuAdsr::fromJson(jsonAdsr.toObject());
    if (adsr != NULL)
        instrument->setAdsr(adsr);
    else
    {
        return DuInstrumentPtr();
    }


    return instrument;
}

QByteArray DuInstrument::toDuMusicFile() const
{
    music_instr du_instrument;


    const DuInstrumentInfoConstPtr& instrInfo = getInstrumentInfo();
    if (instrInfo == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_instrument),
                instrInfo->toDuMusicFile().data(), instrInfo->size());

    const DuPresetConstPtr& preset = getPreset();
    if (preset == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_preset),
                preset->toDuMusicFile().data(), preset->size());

    const DuMixerConstPtr& mixer = getMixer();
    if (mixer == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_mix),
                mixer->toDuMusicFile().data(), mixer->size());

    const DuDistortionConstPtr& distortion = getDistortion();
    if (distortion == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_distortion),
                distortion->toDuMusicFile().data(), distortion->size());

    const DuWahConstPtr& wah = getWah();
    if (wah == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_wah),
                wah->toDuMusicFile().data(), wah->size());

    const DuCompressorConstPtr& compressor = getCompressor();
    if (compressor == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_compressor),
                compressor->toDuMusicFile().data(), compressor->size());

    const DuEqualizerConstPtr& equalizer = getEqualizer();
    if (equalizer == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_equalizer),
                equalizer->toDuMusicFile().data(), equalizer->size());

    const DuDelayConstPtr& delay = getDelay();
    if (delay == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_delay),
                delay->toDuMusicFile().data(), delay->size());

    const DuChorusConstPtr& chorus = getChorus();
    if (chorus == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_chorus),
                chorus->toDuMusicFile().data(), chorus->size());

    const DuVibratoConstPtr& vibrato = getVibrato();
    if (vibrato == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_vibrato),
                vibrato->toDuMusicFile().data(), vibrato->size());

    const DuAdsrConstPtr& adsr = getAdsr();
    if (adsr == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_adsr),
                adsr->toDuMusicFile().data(), adsr->size());

    return QByteArray((char *)&(du_instrument), size());
}


int DuInstrument::size() const
{
    return MUSIC_INSTRU_SIZE;
}


DuInstrumentInfoConstPtr DuInstrument::getInstrumentInfo() const
{
    return getChildAs<DuInstrumentInfo>(KEY_INSTR_INSTRUMENTINFO);
}

void DuInstrument::setInstrumentInfo(const DuInstrumentInfoPtr& instrumentInfo)
{
    addChild(KEY_INSTR_INSTRUMENTINFO, instrumentInfo);
}


DuPresetConstPtr DuInstrument::getPreset() const
{
    return getChildAs<DuPreset>(KEY_INSTR_PRESET);
}

void DuInstrument::setPreset(const DuPresetPtr& preset)
{
    addChild(KEY_INSTR_PRESET, preset);
}


DuMixerConstPtr DuInstrument::getMixer() const
{
    return getChildAs<DuMixer>(KEY_INSTR_MIXERSETTINGS);
}

void DuInstrument::setMixer(const DuMixerPtr &mixer)
{
    addChild(KEY_INSTR_MIXERSETTINGS, mixer);
}

DuDistortionConstPtr DuInstrument::getDistortion() const
{
    return getChildAs<DuDistortion>(KEY_INSTR_DISTORTIONSETTINGS);
}

void DuInstrument::setDistortion(const DuDistortionPtr &distortion)
{
    addChild(KEY_INSTR_DISTORTIONSETTINGS, distortion);
}

DuWahConstPtr DuInstrument::getWah() const
{
    return getChildAs<DuWah>(KEY_INSTR_WAHSETTINGS);
}

void DuInstrument::setWah(const DuWahPtr &wah)
{
    addChild(KEY_INSTR_WAHSETTINGS, wah);
}

DuCompressorConstPtr DuInstrument::getCompressor() const
{
    return getChildAs<DuCompressor>(KEY_INSTR_COMPRESSORSETTINGS);
}

void DuInstrument::setCompressor(const DuCompressorPtr &compressor)
{
    addChild(KEY_INSTR_COMPRESSORSETTINGS, compressor);
}

DuEqualizerConstPtr DuInstrument::getEqualizer() const
{
    return getChildAs<DuEqualizer>(KEY_INSTR_EQUALIZERSETTINGS);
}

void DuInstrument::setEqualizer(const DuEqualizerPtr &equalizer)
{
    addChild(KEY_INSTR_EQUALIZERSETTINGS, equalizer);
}

DuDelayConstPtr DuInstrument::getDelay() const
{
    return getChildAs<DuDelay>(KEY_INSTR_DELAYSETTINGS);
}

void DuInstrument::setDelay(const DuDelayPtr &delay)
{
    addChild(KEY_INSTR_DELAYSETTINGS, delay);
}

DuChorusConstPtr DuInstrument::getChorus() const
{
    return getChildAs<DuChorus>(KEY_INSTR_CHORUSSETTINGS);
}

void DuInstrument::setChorus(const DuChorusPtr &chorus)
{
    addChild(KEY_INSTR_CHORUSSETTINGS, chorus);
}

DuVibratoConstPtr DuInstrument::getVibrato() const
{
    return getChildAs<DuVibrato>(KEY_INSTR_VIBRATOSETTINGS);
}

void DuInstrument::setVibrato(const DuVibratoPtr &vibrato)
{
    addChild(KEY_INSTR_VIBRATOSETTINGS, vibrato);
}

DuAdsrConstPtr DuInstrument::getAdsr() const
{
    return getChildAs<DuAdsr>(KEY_INSTR_ADSRSETTINGS);
}

void DuInstrument::setAdsr(const DuAdsrPtr &adsr)
{
    addChild(KEY_INSTR_ADSRSETTINGS, adsr);
}
