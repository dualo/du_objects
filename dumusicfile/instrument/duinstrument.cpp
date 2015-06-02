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


DuInstrumentPtr DuInstrument::fromDuMusicBinary(const music_instr &du_instr)
{
    DuInstrumentPtr instrument(new DuInstrument);

    const DuInstrumentInfoPtr &instrInfo =
            DuInstrumentInfo::fromDuMusicBinary(du_instr.i_instrument);
    if (instrInfo == NULL)
    {
        qCritical() << "DuInstrument::fromDuMusicBinary():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuInstrumentInfo was not generated properly";

        return DuInstrumentPtr();
    }
    instrument->setInstrumentInfo(instrInfo);

    const DuPresetPtr &preset =
            DuPreset::fromDuMusicBinary(du_instr.i_preset);
    if (preset == NULL)
    {
        qCritical() << "DuInstrument::fromDuMusicBinary():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuPreset was not generated properly";

        return DuInstrumentPtr();
    }
    instrument->setPreset(preset);

    const DuMixerPtr &mixer =
            DuMixer::fromDuMusicBinary(du_instr.i_mix);
    if (mixer == NULL)
    {
        qCritical() << "DuInstrument::fromDuMusicBinary():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuMixer was not generated properly";

        return DuInstrumentPtr();
    }
    instrument->setMixer(mixer);

    const DuDistortionPtr &distortion =
            DuDistortion::fromDuMusicBinary(du_instr.i_distortion);
    if (distortion == NULL)
    {
        qCritical() << "DuInstrument::fromDuMusicBinary():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuDistortion was not generated properly";

        return DuInstrumentPtr();
    }
    instrument->setDistortion(distortion);

    const DuWahPtr &wah =
            DuWah::fromDuMusicBinary(du_instr.i_wah);
    if (wah == NULL)
    {
        qCritical() << "DuInstrument::fromDuMusicBinary():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuWah was not generated properly";

        return DuInstrumentPtr();
    }
    instrument->setWah(wah);

    const DuCompressorPtr &compressor =
            DuCompressor::fromDuMusicBinary(du_instr.i_compressor);
    if (compressor == NULL)
    {
        qCritical() << "DuInstrument::fromDuMusicBinary():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuCompressor was not generated properly";

        return DuInstrumentPtr();
    }
    instrument->setCompressor(compressor);

    const DuEqualizerPtr &equalizer =
            DuEqualizer::fromDuMusicBinary(du_instr.i_equalizer);
    if (equalizer == NULL)
    {
        qCritical() << "DuInstrument::fromDuMusicBinary():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuEqualizer was not generated properly";

        return DuInstrumentPtr();
    }
    instrument->setEqualizer(equalizer);

    const DuDelayPtr &delay =
            DuDelay::fromDuMusicBinary(du_instr.i_delay);
    if (delay == NULL)
    {
        qCritical() << "DuInstrument::fromDuMusicBinary():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuDelay was not generated properly";

        return DuInstrumentPtr();
    }
    instrument->setDelay(delay);

    const DuChorusPtr &chorus =
            DuChorus::fromDuMusicBinary(du_instr.i_chorus);
    if (chorus == NULL)
    {
        qCritical() << "DuInstrument::fromDuMusicBinary():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuChorus was not generated properly";

        return DuInstrumentPtr();
    }
    instrument->setChorus(chorus);

    const DuVibratoPtr &vibrato =
            DuVibrato::fromDuMusicBinary(du_instr.i_vibrato);
    if (vibrato == NULL)
    {
        qCritical() << "DuInstrument::fromDuMusicBinary():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuVibrato was not generated properly";

        return DuInstrumentPtr();
    }
    instrument->setVibrato(vibrato);

    const DuAdsrPtr &adsr =
            DuAdsr::fromDuMusicBinary(du_instr.i_adsr);
    if (adsr == NULL)
    {
        qCritical() << "DuInstrument::fromDuMusicBinary():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuAdsr was not generated properly";

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
    {
        qCritical() << "DuInstrument::fromJson():\n"
                    << "failed to generate DuInstrument\n"
                    << "a json key did not contain the proper type";

        return DuInstrumentPtr();
    }


    DuInstrumentPtr instrument(new DuInstrument);

    const DuInstrumentInfoPtr &instrInfo =
            DuInstrumentInfo::fromJson(jsonInstrInfo.toObject());
    if (instrInfo != NULL)
        instrument->setInstrumentInfo(instrInfo);
    else
    {
        qCritical() << "DuInstrument::fromJson():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuInstrumentInfo was not generated properly";

        return DuInstrumentPtr();
    }

    const DuPresetPtr &preset =
            DuPreset::fromJson(jsonPreset.toObject());
    if (preset != NULL)
        instrument->setPreset(preset);
    else
    {
        qCritical() << "DuInstrument::fromJson():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuPreset was not generated properly";

        return DuInstrumentPtr();
    }

    const DuMixerPtr &mixer =
            DuMixer::fromJson(jsonMixer.toObject());
    if (mixer != NULL)
        instrument->setMixer(mixer);
    else
    {
        qCritical() << "DuInstrument::fromJson():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuMixer was not generated properly";

        return DuInstrumentPtr();
    }

    const DuDistortionPtr &distortion =
            DuDistortion::fromJson(jsonDistortion.toObject());
    if (distortion != NULL)
        instrument->setDistortion(distortion);
    else
    {
        qCritical() << "DuInstrument::fromJson():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuDistortion was not generated properly";

        return DuInstrumentPtr();
    }

    const DuWahPtr &wah =
            DuWah::fromJson(jsonWah.toObject());
    if (wah != NULL)
        instrument->setWah(wah);
    else
    {
        qCritical() << "DuInstrument::fromJson():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuWah was not generated properly";

        return DuInstrumentPtr();
    }

    const DuCompressorPtr &compressor =
            DuCompressor::fromJson(jsonCompressor.toObject());
    if (compressor != NULL)
        instrument->setCompressor(compressor);
    else
    {
        qCritical() << "DuInstrument::fromJson():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuCompressor was not generated properly";

        return DuInstrumentPtr();
    }

    const DuEqualizerPtr &equalizer =
            DuEqualizer::fromJson(jsonEqualizer.toObject());
    if (equalizer != NULL)
        instrument->setEqualizer(equalizer);
    else
    {
        qCritical() << "DuInstrument::fromJson():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuEqualizer was not generated properly";

        return DuInstrumentPtr();
    }

    const DuDelayPtr &delay =
            DuDelay::fromJson(jsonDelay.toObject());
    if (delay != NULL)
        instrument->setDelay(delay);
    else
    {
        qCritical() << "DuInstrument::fromJson():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuDelay was not generated properly";

        return DuInstrumentPtr();
    }

    const DuChorusPtr &chorus =
            DuChorus::fromJson(jsonChorus.toObject());
    if (chorus != NULL)
        instrument->setChorus(chorus);
    else
    {
        qCritical() << "DuInstrument::fromJson():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuChorus was not generated properly";

        return DuInstrumentPtr();
    }

    const DuVibratoPtr &vibrato =
            DuVibrato::fromJson(jsonVibrato.toObject());
    if (vibrato != NULL)
        instrument->setVibrato(vibrato);
    else
    {
        qCritical() << "DuInstrument::fromJson():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuVibrato was not generated properly";

        return DuInstrumentPtr();
    }

    const DuAdsrPtr &adsr =
            DuAdsr::fromJson(jsonAdsr.toObject());
    if (adsr != NULL)
        instrument->setAdsr(adsr);
    else
    {
        qCritical() << "DuInstrument::fromJson():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuAdsr was not generated properly";

        return DuInstrumentPtr();
    }


    return instrument;
}

QByteArray DuInstrument::toDuMusicBinary() const
{
    music_instr du_instrument;


    const DuInstrumentInfoConstPtr& instrInfo = getInstrumentInfo();
    if (instrInfo == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_instrument),
                instrInfo->toDuMusicBinary().data(), instrInfo->size());

    const DuPresetConstPtr& preset = getPreset();
    if (preset == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_preset),
                preset->toDuMusicBinary().data(), preset->size());

    const DuMixerConstPtr& mixer = getMixer();
    if (mixer == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_mix),
                mixer->toDuMusicBinary().data(), mixer->size());

    const DuDistortionConstPtr& distortion = getDistortion();
    if (distortion == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_distortion),
                distortion->toDuMusicBinary().data(), distortion->size());

    const DuWahConstPtr& wah = getWah();
    if (wah == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_wah),
                wah->toDuMusicBinary().data(), wah->size());

    const DuCompressorConstPtr& compressor = getCompressor();
    if (compressor == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_compressor),
                compressor->toDuMusicBinary().data(), compressor->size());

    const DuEqualizerConstPtr& equalizer = getEqualizer();
    if (equalizer == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_equalizer),
                equalizer->toDuMusicBinary().data(), equalizer->size());

    const DuDelayConstPtr& delay = getDelay();
    if (delay == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_delay),
                delay->toDuMusicBinary().data(), delay->size());

    const DuChorusConstPtr& chorus = getChorus();
    if (chorus == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_chorus),
                chorus->toDuMusicBinary().data(), chorus->size());

    const DuVibratoConstPtr& vibrato = getVibrato();
    if (vibrato == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_vibrato),
                vibrato->toDuMusicBinary().data(), vibrato->size());

    const DuAdsrConstPtr& adsr = getAdsr();
    if (adsr == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_adsr),
                adsr->toDuMusicBinary().data(), adsr->size());

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
