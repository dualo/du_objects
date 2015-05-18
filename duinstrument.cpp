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
    if (instrInfo != NULL)
        instrument->setInstrumentInfo(instrInfo);
    else
    {
        delete instrument;
        return NULL;
    }

    DuPreset *preset = DuPreset::fromDuMusicFile(du_instr.i_preset);
    if (preset != NULL)
        instrument->setPreset(preset);
    else
    {
        delete instrument;
        return NULL;
    }

    DuMixer *mixer = DuMixer::fromDuMusicFile(du_instr.i_mix);
    if (mixer != NULL)
        instrument->setMixer(mixer);
    else
    {
        delete instrument;
        return NULL;
    }

    DuDistortion *distortion = DuDistortion::fromDuMusicFile(du_instr.i_distortion);
    if (distortion != NULL)
        instrument->setDistortion(distortion);
    else
    {
        delete instrument;
        return NULL;
    }

    DuWah *wah = DuWah::fromDuMusicFile(du_instr.i_wah);
    if (wah != NULL)
        instrument->setWah(wah);
    else
    {
        delete instrument;
        return NULL;
    }

    DuCompressor *compressor = DuCompressor::fromDuMusicFile(du_instr.i_compressor);
    if (compressor != NULL)
        instrument->setCompressor(compressor);
    else
    {
        delete instrument;
        return NULL;
    }

    DuEqualizer *equalizer = DuEqualizer::fromDuMusicFile(du_instr.i_equalizer);
    if (equalizer != NULL)
        instrument->setEqualizer(equalizer);
    else
    {
        delete instrument;
        return NULL;
    }

    DuDelay *delay = DuDelay::fromDuMusicFile(du_instr.i_delay);
    if (delay != NULL)
        instrument->setDelay(delay);
    else
    {
        delete instrument;
        return NULL;
    }

    DuChorus *chorus = DuChorus::fromDuMusicFile(du_instr.i_chorus);
    if (chorus != NULL)
        instrument->setChorus(chorus);
    else
    {
        delete instrument;
        return NULL;
    }

    DuVibrato *vibrato = DuVibrato::fromDuMusicFile(du_instr.i_vibrato);
    if (vibrato != NULL)
        instrument->setVibrato(vibrato);
    else
    {
        delete instrument;
        return NULL;
    }

    DuAdsr *adsr = DuAdsr::fromDuMusicFile(du_instr.i_adsr);
    if (adsr != NULL)
        instrument->setAdsr(adsr);
    else
    {
        delete instrument;
        return NULL;
    }

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


    DuInstrumentInfo *instrInfo = getInstrumentInfo();
    if (instrInfo == NULL)
        return QByteArray();

#ifdef Q_OS_WIN
    memcpy_s((char *)&(du_instrument.i_instrument), instrInfo->size(),
             instrInfo->toDuMusicFile().data(), instrInfo->size());
#else
    memcpy((char *)&(du_instrument.i_instrument),
           instrInfo->toDuMusicFile().data(), instrInfo->size());
#endif

    DuPreset *preset = getPreset();
    if (preset == NULL)
        return QByteArray();

#ifdef Q_OS_WIN
    memcpy_s((char *)&(du_instrument.i_preset), preset->size(),
             preset->toDuMusicFile().data(), preset->size());
#else
    memcpy((char *)&(du_instrument.i_preset),
           preset->toDuMusicFile().data(), preset->size());
#endif

    DuMixer *mixer = getMixer();
    if (mixer == NULL)
        return QByteArray();

#ifdef Q_OS_WIN
    memcpy_s((char *)&(du_instrument.i_mix), mixer->size(),
             mixer->toDuMusicFile().data(), mixer->size());
#else
    memcpy((char *)&(du_instrument.i_mix),
           mixer->toDuMusicFile().data(), mixer->size());
#endif

    DuDistortion *distortion = getDistortion();
    if (distortion == NULL)
        return QByteArray();

#ifdef Q_OS_WIN
    memcpy_s((char *)&(du_instrument.i_distortion), distortion->size(),
             distortion->toDuMusicFile().data(), distortion->size());
#else
    memcpy((char *)&(du_instrument.i_distortion),
           distortion->toDuMusicFile().data(), distortion->size());
#endif

    DuWah *wah = getWah();
    if (wah == NULL)
        return QByteArray();

#ifdef Q_OS_WIN
    memcpy_s((char *)&(du_instrument.i_wah), wah->size(),
             wah->toDuMusicFile().data(), wah->size());
#else
    memcpy((char *)&(du_instrument.i_wah),
           wah->toDuMusicFile().data(), wah->size());
#endif

    DuCompressor *compressor = getCompressor();
    if (compressor == NULL)
        return QByteArray();

#ifdef Q_OS_WIN
    memcpy_s((char *)&(du_instrument.i_compressor), compressor->size(),
             compressor->toDuMusicFile().data(), compressor->size());
#else
    memcpy((char *)&(du_instrument.i_compressor),
           compressor->toDuMusicFile().data(), compressor->size());
#endif

    DuEqualizer *equalizer = getEqualizer();
    if (equalizer == NULL)
        return QByteArray();

#ifdef Q_OS_WIN
    memcpy_s((char *)&(du_instrument.i_equalizer), equalizer->size(),
             equalizer->toDuMusicFile().data(), equalizer->size());
#else
    memcpy((char *)&(du_instrument.i_equalizer),
           equalizer->toDuMusicFile().data(), equalizer->size());
#endif

    DuDelay *delay = getDelay();
    if (delay == NULL)
        return QByteArray();

#ifdef Q_OS_WIN
    memcpy_s((char *)&(du_instrument.i_delay), delay->size(),
             delay->toDuMusicFile().data(), delay->size());
#else
    memcpy((char *)&(du_instrument.i_delay),
           delay->toDuMusicFile().data(), delay->size());
#endif

    DuChorus *chorus = getChorus();
    if (chorus == NULL)
        return QByteArray();

#ifdef Q_OS_WIN
    memcpy_s((char *)&(du_instrument.i_chorus), chorus->size(),
             chorus->toDuMusicFile().data(), chorus->size());
#else
    memcpy((char *)&(du_instrument.i_chorus),
           chorus->toDuMusicFile().data(), chorus->size());
#endif

    DuVibrato *vibrato = getVibrato();
    if (vibrato == NULL)
        return QByteArray();

#ifdef Q_OS_WIN
    memcpy_s((char *)&(du_instrument.i_vibrato), vibrato->size(),
             vibrato->toDuMusicFile().data(), vibrato->size());
#else
    memcpy((char *)&(du_instrument.i_vibrato),
           vibrato->toDuMusicFile().data(), vibrato->size());
#endif

    DuAdsr *adsr = getAdsr();
    if (adsr == NULL)
        return QByteArray();

#ifdef Q_OS_WIN
    memcpy_s((char *)&(du_instrument.i_adsr), adsr->size(),
             adsr->toDuMusicFile().data(), adsr->size());
#else
    memcpy((char *)&(du_instrument.i_adsr),
           adsr->toDuMusicFile().data(), adsr->size());
#endif

    return QByteArray((char *)&(du_instrument), size());
}


int DuInstrument::size() const
{
    return MUSIC_INSTRU_SIZE;
}


DuInstrumentInfo *DuInstrument::getInstrumentInfo() const
{
    return dynamic_cast<DuInstrumentInfo *>(getChild(KEY_INSTR_INSTRUMENTINFO));
}

void DuInstrument::setInstrumentInfo(DuInstrumentInfo *instrumentInfo)
{
    if (getChild(KEY_INSTR_INSTRUMENTINFO) != NULL)
        delete getChild(KEY_INSTR_INSTRUMENTINFO);

    addChild(KEY_INSTR_INSTRUMENTINFO, instrumentInfo);
}


DuPreset *DuInstrument::getPreset() const
{
    return dynamic_cast<DuPreset *>(getChild(KEY_INSTR_PRESET));
}

void DuInstrument::setPreset(DuPreset *preset)
{
    if (getChild(KEY_INSTR_PRESET) != NULL)
        delete getChild(KEY_INSTR_PRESET);

    addChild(KEY_INSTR_PRESET, preset);
}


DuMixer *DuInstrument::getMixer() const
{
    return dynamic_cast<DuMixer *>(getChild(KEY_INSTR_MIXERSETTINGS));
}

void DuInstrument::setMixer(DuMixer *mixer)
{
    if (getChild(KEY_INSTR_MIXERSETTINGS) != NULL)
        delete getChild(KEY_INSTR_MIXERSETTINGS);

    addChild(KEY_INSTR_MIXERSETTINGS, mixer);
}

DuDistortion *DuInstrument::getDistortion() const
{
    return dynamic_cast<DuDistortion *>(getChild(KEY_INSTR_DISTORTIONSETTINGS));
}

void DuInstrument::setDistortion(DuDistortion *distortion)
{
    if (getChild(KEY_INSTR_DISTORTIONSETTINGS) != NULL)
        delete getChild(KEY_INSTR_DISTORTIONSETTINGS);

    addChild(KEY_INSTR_DISTORTIONSETTINGS, distortion);
}

DuWah *DuInstrument::getWah() const
{
    return dynamic_cast<DuWah *>(getChild(KEY_INSTR_WAHSETTINGS));
}

void DuInstrument::setWah(DuWah *wah)
{
    if (getChild(KEY_INSTR_WAHSETTINGS) != NULL)
        delete getChild(KEY_INSTR_WAHSETTINGS);

    addChild(KEY_INSTR_WAHSETTINGS, wah);
}

DuCompressor *DuInstrument::getCompressor() const
{
    return dynamic_cast<DuCompressor *>(getChild(KEY_INSTR_COMPRESSORSETTINGS));
}

void DuInstrument::setCompressor(DuCompressor *compressor)
{
    if (getChild(KEY_INSTR_COMPRESSORSETTINGS) != NULL)
        delete getChild(KEY_INSTR_COMPRESSORSETTINGS);

    addChild(KEY_INSTR_COMPRESSORSETTINGS, compressor);
}

DuEqualizer *DuInstrument::getEqualizer() const
{
    return dynamic_cast<DuEqualizer *>(getChild(KEY_INSTR_EQUALIZERSETTINGS));
}

void DuInstrument::setEqualizer(DuEqualizer *equalizer)
{
    if (getChild(KEY_INSTR_EQUALIZERSETTINGS) != NULL)
        delete getChild(KEY_INSTR_EQUALIZERSETTINGS);

    addChild(KEY_INSTR_EQUALIZERSETTINGS, equalizer);
}

DuDelay *DuInstrument::getDelay() const
{
    return dynamic_cast<DuDelay *>(getChild(KEY_INSTR_DELAYSETTINGS));
}

void DuInstrument::setDelay(DuDelay *delay)
{
    if (getChild(KEY_INSTR_DELAYSETTINGS) != NULL)
        delete getChild(KEY_INSTR_DELAYSETTINGS);

    addChild(KEY_INSTR_DELAYSETTINGS, delay);
}

DuChorus *DuInstrument::getChorus() const
{
    return dynamic_cast<DuChorus *>(getChild(KEY_INSTR_CHORUSSETTINGS));
}

void DuInstrument::setChorus(DuChorus *chorus)
{
    if (getChild(KEY_INSTR_CHORUSSETTINGS) != NULL)
        delete getChild(KEY_INSTR_CHORUSSETTINGS);

    addChild(KEY_INSTR_CHORUSSETTINGS, chorus);
}

DuVibrato *DuInstrument::getVibrato() const
{
    return dynamic_cast<DuVibrato *>(getChild(KEY_INSTR_VIBRATOSETTINGS));
}

void DuInstrument::setVibrato(DuVibrato *vibrato)
{
    if (getChild(KEY_INSTR_VIBRATOSETTINGS) != NULL)
        delete getChild(KEY_INSTR_VIBRATOSETTINGS);

    addChild(KEY_INSTR_VIBRATOSETTINGS, vibrato);
}

DuAdsr *DuInstrument::getAdsr() const
{
    return dynamic_cast<DuAdsr *>(getChild(KEY_INSTR_ADSRSETTINGS));
}

void DuInstrument::setAdsr(DuAdsr *adsr)
{
    if (getChild(KEY_INSTR_ADSRSETTINGS) != NULL)
        delete getChild(KEY_INSTR_ADSRSETTINGS);

    addChild(KEY_INSTR_ADSRSETTINGS, adsr);
}
