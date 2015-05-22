#include "duinstrument.h"

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


DuInstrument *DuInstrument::fromDuMusicFile(const music_instr &du_instr)
{
    DuInstrument *instrument = new DuInstrument;

    DuInstrumentInfo *instrInfo =
            DuInstrumentInfo::fromDuMusicFile(du_instr.i_instrument);
    if (instrInfo == NULL)
    {
        delete instrument;
        return NULL;
    }
    instrument->setInstrumentInfo(instrInfo);

    DuPreset *preset = DuPreset::fromDuMusicFile(du_instr.i_preset);
    if (preset == NULL)
    {
        delete instrument;
        return NULL;
    }
    instrument->setPreset(preset);

    DuMixer *mixer = DuMixer::fromDuMusicFile(du_instr.i_mix);
    if (mixer == NULL)
    {
        delete instrument;
        return NULL;
    }
    instrument->setMixer(mixer);

    DuDistortion *distortion = DuDistortion::fromDuMusicFile(du_instr.i_distortion);
    if (distortion == NULL)
    {
        delete instrument;
        return NULL;
    }
    instrument->setDistortion(distortion);

    DuWah *wah = DuWah::fromDuMusicFile(du_instr.i_wah);
    if (wah == NULL)
    {
        delete instrument;
        return NULL;
    }
    instrument->setWah(wah);

    DuCompressor *compressor = DuCompressor::fromDuMusicFile(du_instr.i_compressor);
    if (compressor == NULL)
    {
        delete instrument;
        return NULL;
    }
    instrument->setCompressor(compressor);

    DuEqualizer *equalizer = DuEqualizer::fromDuMusicFile(du_instr.i_equalizer);
    if (equalizer == NULL)
    {
        delete instrument;
        return NULL;
    }
    instrument->setEqualizer(equalizer);

    DuDelay *delay = DuDelay::fromDuMusicFile(du_instr.i_delay);
    if (delay == NULL)
    {
        delete instrument;
        return NULL;
    }
    instrument->setDelay(delay);

    DuChorus *chorus = DuChorus::fromDuMusicFile(du_instr.i_chorus);
    if (chorus == NULL)
    {
        delete instrument;
        return NULL;
    }
    instrument->setChorus(chorus);

    DuVibrato *vibrato = DuVibrato::fromDuMusicFile(du_instr.i_vibrato);
    if (vibrato == NULL)
    {
        delete instrument;
        return NULL;
    }
    instrument->setVibrato(vibrato);

    DuAdsr *adsr = DuAdsr::fromDuMusicFile(du_instr.i_adsr);
    if (adsr == NULL)
    {
        delete instrument;
        return NULL;
    }
    instrument->setAdsr(adsr);

    return instrument;
}


DuInstrument *DuInstrument::fromJson(const QJsonObject &jsonInstrument)
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

        return NULL;


    DuInstrument *instrument = new DuInstrument;

    DuInstrumentInfo *instrInfo =
            DuInstrumentInfo::fromJson(jsonInstrInfo.toObject());
    if (instrInfo != NULL)
        instrument->setInstrumentInfo(instrInfo);
    else
    {
        delete instrument;
        return NULL;
    }

    DuPreset *preset = DuPreset::fromJson(jsonPreset.toObject());
    if (preset != NULL)
        instrument->setPreset(preset);
    else
    {
        delete instrument;
        return NULL;
    }

    DuMixer *mixer = DuMixer::fromJson(jsonMixer.toObject());
    if (mixer != NULL)
        instrument->setMixer(mixer);
    else
    {
        delete instrument;
        return NULL;
    }

    DuDistortion *distortion = DuDistortion::fromJson(jsonDistortion.toObject());
    if (distortion != NULL)
        instrument->setDistortion(distortion);
    else
    {
        delete instrument;
        return NULL;
    }

    DuWah *wah = DuWah::fromJson(jsonWah.toObject());
    if (wah != NULL)
        instrument->setWah(wah);
    else
    {
        delete instrument;
        return NULL;
    }

    DuCompressor *compressor = DuCompressor::fromJson(jsonCompressor.toObject());
    if (compressor != NULL)
        instrument->setCompressor(compressor);
    else
    {
        delete instrument;
        return NULL;
    }

    DuEqualizer *equalizer = DuEqualizer::fromJson(jsonEqualizer.toObject());
    if (equalizer != NULL)
        instrument->setEqualizer(equalizer);
    else
    {
        delete instrument;
        return NULL;
    }

    DuDelay *delay = DuDelay::fromJson(jsonDelay.toObject());
    if (delay != NULL)
        instrument->setDelay(delay);
    else
    {
        delete instrument;
        return NULL;
    }

    DuChorus *chorus = DuChorus::fromJson(jsonChorus.toObject());
    if (chorus != NULL)
        instrument->setChorus(chorus);
    else
    {
        delete instrument;
        return NULL;
    }

    DuVibrato *vibrato = DuVibrato::fromJson(jsonVibrato.toObject());
    if (vibrato != NULL)
        instrument->setVibrato(vibrato);
    else
    {
        delete instrument;
        return NULL;
    }

    DuAdsr *adsr = DuAdsr::fromJson(jsonAdsr.toObject());
    if (adsr != NULL)
        instrument->setAdsr(adsr);
    else
    {
        delete instrument;
        return NULL;
    }


    return instrument;
}


QByteArray DuInstrument::toDuMusicFile() const
{
    music_instr du_instrument;


    QSharedPointer<DuInstrumentInfo> instrInfo = getInstrumentInfo();
    if (instrInfo == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_instrument),
                instrInfo->toDuMusicFile().data(), instrInfo->size());

    QSharedPointer<DuPreset> preset = getPreset();
    if (preset == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_preset),
                preset->toDuMusicFile().data(), preset->size());

    QSharedPointer<DuMixer> mixer = getMixer();
    if (mixer == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_mix),
                mixer->toDuMusicFile().data(), mixer->size());

    QSharedPointer<DuDistortion> distortion = getDistortion();
    if (distortion == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_distortion),
                distortion->toDuMusicFile().data(), distortion->size());

    QSharedPointer<DuWah> wah = getWah();
    if (wah == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_wah),
                wah->toDuMusicFile().data(), wah->size());

    QSharedPointer<DuCompressor> compressor = getCompressor();
    if (compressor == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_compressor),
                compressor->toDuMusicFile().data(), compressor->size());

    QSharedPointer<DuEqualizer> equalizer = getEqualizer();
    if (equalizer == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_equalizer),
                equalizer->toDuMusicFile().data(), equalizer->size());

    QSharedPointer<DuDelay> delay = getDelay();
    if (delay == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_delay),
                delay->toDuMusicFile().data(), delay->size());

    QSharedPointer<DuChorus> chorus = getChorus();
    if (chorus == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_chorus),
                chorus->toDuMusicFile().data(), chorus->size());

    QSharedPointer<DuVibrato> vibrato = getVibrato();
    if (vibrato == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_vibrato),
                vibrato->toDuMusicFile().data(), vibrato->size());

    QSharedPointer<DuAdsr> adsr = getAdsr();
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


QSharedPointer<DuInstrumentInfo> DuInstrument::getInstrumentInfo() const
{
    return getChildAs<DuInstrumentInfo>(KEY_INSTR_INSTRUMENTINFO);
}

void DuInstrument::setInstrumentInfo(DuInstrumentInfo *instrumentInfo)
{
    addChild(KEY_INSTR_INSTRUMENTINFO, instrumentInfo);
}


QSharedPointer<DuPreset> DuInstrument::getPreset() const
{
    return getChildAs<DuPreset>(KEY_INSTR_PRESET);
}

void DuInstrument::setPreset(DuPreset *preset)
{
    addChild(KEY_INSTR_PRESET, preset);
}


QSharedPointer<DuMixer> DuInstrument::getMixer() const
{
    return getChildAs<DuMixer>(KEY_INSTR_MIXERSETTINGS);
}

void DuInstrument::setMixer(DuMixer *mixer)
{
    addChild(KEY_INSTR_MIXERSETTINGS, mixer);
}

QSharedPointer<DuDistortion> DuInstrument::getDistortion() const
{
    return getChildAs<DuDistortion>(KEY_INSTR_DISTORTIONSETTINGS);
}

void DuInstrument::setDistortion(DuDistortion *distortion)
{
    addChild(KEY_INSTR_DISTORTIONSETTINGS, distortion);
}

QSharedPointer<DuWah> DuInstrument::getWah() const
{
    return getChildAs<DuWah>(KEY_INSTR_WAHSETTINGS);
}

void DuInstrument::setWah(DuWah *wah)
{
    addChild(KEY_INSTR_WAHSETTINGS, wah);
}

QSharedPointer<DuCompressor> DuInstrument::getCompressor() const
{
    return getChildAs<DuCompressor>(KEY_INSTR_COMPRESSORSETTINGS);
}

void DuInstrument::setCompressor(DuCompressor *compressor)
{
    addChild(KEY_INSTR_COMPRESSORSETTINGS, compressor);
}

QSharedPointer<DuEqualizer> DuInstrument::getEqualizer() const
{
    return getChildAs<DuEqualizer>(KEY_INSTR_EQUALIZERSETTINGS);
}

void DuInstrument::setEqualizer(DuEqualizer *equalizer)
{
    addChild(KEY_INSTR_EQUALIZERSETTINGS, equalizer);
}

QSharedPointer<DuDelay> DuInstrument::getDelay() const
{
    return getChildAs<DuDelay>(KEY_INSTR_DELAYSETTINGS);
}

void DuInstrument::setDelay(DuDelay *delay)
{
    addChild(KEY_INSTR_DELAYSETTINGS, delay);
}

QSharedPointer<DuChorus> DuInstrument::getChorus() const
{
    return getChildAs<DuChorus>(KEY_INSTR_CHORUSSETTINGS);
}

void DuInstrument::setChorus(DuChorus *chorus)
{
    addChild(KEY_INSTR_CHORUSSETTINGS, chorus);
}

QSharedPointer<DuVibrato> DuInstrument::getVibrato() const
{
    return getChildAs<DuVibrato>(KEY_INSTR_VIBRATOSETTINGS);
}

void DuInstrument::setVibrato(DuVibrato *vibrato)
{
    addChild(KEY_INSTR_VIBRATOSETTINGS, vibrato);
}

QSharedPointer<DuAdsr> DuInstrument::getAdsr() const
{
    return getChildAs<DuAdsr>(KEY_INSTR_ADSRSETTINGS);
}

void DuInstrument::setAdsr(DuAdsr *adsr)
{
    addChild(KEY_INSTR_ADSRSETTINGS, adsr);
}
