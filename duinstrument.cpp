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
    DuInstrument *instrument = new DuInstrument();

    instrument->setInstrumentInfo(
                DuInstrumentInfo::fromDuMusicFile(du_instr.i_instrument));

    instrument->setPreset(DuPreset::fromDuMusicFile(du_instr.i_preset));

    instrument->setMixer(DuMixer::fromDuMusicFile(du_instr.i_mix));

    instrument->setDistortion(DuDistortion::fromDuMusicFile(du_instr.i_distortion));

    instrument->setWah(DuWah::fromDuMusicFile(du_instr.i_wah));

    instrument->setCompressor(DuCompressor::fromDuMusicFile(du_instr.i_compressor));

    instrument->setEqualizer(DuEqualizer::fromDuMusicFile(du_instr.i_equalizer));

    instrument->setDelay(DuDelay::fromDuMusicFile(du_instr.i_delay));

    instrument->setChorus(DuChorus::fromDuMusicFile(du_instr.i_chorus));

    instrument->setVibrato(DuVibrato::fromDuMusicFile(du_instr.i_vibrato));

    instrument->setAdsr(DuAdsr::fromDuMusicFile(du_instr.i_adsr));

    return instrument;
}


DuInstrument *DuInstrument::fromJson(const QJsonObject &jsonInstrument)
{
    DuInstrument *instrument = new DuInstrument();
    QStringList &keyList = instrument->keys();

    bool test = true;
    for (int i = 0; i < keyList.count(); i++)
    {
        test = test && jsonInstrument.contains(keyList[i]);
    }

    if (!test)
        return NULL;

    instrument->setInstrumentInfo(
                DuInstrumentInfo::fromJson(
                    jsonInstrument[KEY_INSTR_INSTRUMENTINFO].toObject()));
    instrument->setPreset(
                DuPreset::fromJson(
                    jsonInstrument[KEY_INSTR_PRESET].toObject()));
    instrument->setMixer(
                DuMixer::fromJson(
                    jsonInstrument[KEY_INSTR_MIXERSETTINGS].toObject()));
    instrument->setDistortion(
                DuDistortion::fromJson(
                    jsonInstrument[KEY_INSTR_DISTORTIONSETTINGS].toObject()));
    instrument->setWah(
                DuWah::fromJson(
                    jsonInstrument[KEY_INSTR_WAHSETTINGS].toObject()));
    instrument->setCompressor(
                DuCompressor::fromJson(
                    jsonInstrument[KEY_INSTR_COMPRESSORSETTINGS].toObject()));
    instrument->setEqualizer(
                DuEqualizer::fromJson(
                    jsonInstrument[KEY_INSTR_EQUALIZERSETTINGS].toObject()));
    instrument->setDelay(
                DuDelay::fromJson(
                    jsonInstrument[KEY_INSTR_DELAYSETTINGS].toObject()));
    instrument->setChorus(
                DuChorus::fromJson(
                    jsonInstrument[KEY_INSTR_CHORUSSETTINGS].toObject()));
    instrument->setVibrato(
                DuVibrato::fromJson(
                    jsonInstrument[KEY_INSTR_VIBRATOSETTINGS].toObject()));
    instrument->setAdsr(
                DuAdsr::fromJson(
                    jsonInstrument[KEY_INSTR_ADSRSETTINGS].toObject()));

    return instrument;
}


DuInstrumentInfo *DuInstrument::getInstrumentInfo()
{
    return dynamic_cast<DuInstrumentInfo *>(getChild(KEY_INSTR_INSTRUMENTINFO));
}

void DuInstrument::setInstrumentInfo(DuInstrumentInfo *instrumentInfo)
{
    if (getChild(KEY_INSTR_INSTRUMENTINFO) != NULL)
        delete getChild(KEY_INSTR_INSTRUMENTINFO);

    addChild(KEY_INSTR_INSTRUMENTINFO, instrumentInfo);
}


DuPreset *DuInstrument::getPreset()
{
    return dynamic_cast<DuPreset *>(getChild(KEY_INSTR_PRESET));
}

void DuInstrument::setPreset(DuPreset *preset)
{
    if (getChild(KEY_INSTR_PRESET) != NULL)
        delete getChild(KEY_INSTR_PRESET);

    addChild(KEY_INSTR_PRESET, preset);
}


DuMixer *DuInstrument::getMixer()
{
    return dynamic_cast<DuMixer *>(getChild(KEY_INSTR_MIXERSETTINGS));
}

void DuInstrument::setMixer(DuMixer *mixer)
{
    if (getChild(KEY_INSTR_MIXERSETTINGS) != NULL)
        delete getChild(KEY_INSTR_MIXERSETTINGS);

    addChild(KEY_INSTR_MIXERSETTINGS, mixer);
}

DuDistortion *DuInstrument::getDistortion()
{
    return dynamic_cast<DuDistortion *>(getChild(KEY_INSTR_DISTORTIONSETTINGS));
}

void DuInstrument::setDistortion(DuDistortion *distortion)
{
    if (getChild(KEY_INSTR_DISTORTIONSETTINGS) != NULL)
        delete getChild(KEY_INSTR_DISTORTIONSETTINGS);

    addChild(KEY_INSTR_DISTORTIONSETTINGS, distortion);
}

DuWah *DuInstrument::getWah()
{
    return dynamic_cast<DuWah *>(getChild(KEY_INSTR_WAHSETTINGS));
}

void DuInstrument::setWah(DuWah *wah)
{
    if (getChild(KEY_INSTR_WAHSETTINGS) != NULL)
        delete getChild(KEY_INSTR_WAHSETTINGS);

    addChild(KEY_INSTR_WAHSETTINGS, wah);
}

DuCompressor *DuInstrument::getCompressor()
{
    return dynamic_cast<DuCompressor *>(getChild(KEY_INSTR_COMPRESSORSETTINGS));
}

void DuInstrument::setCompressor(DuCompressor *compressor)
{
    if (getChild(KEY_INSTR_COMPRESSORSETTINGS) != NULL)
        delete getChild(KEY_INSTR_COMPRESSORSETTINGS);

    addChild(KEY_INSTR_COMPRESSORSETTINGS, compressor);
}

DuEqualizer *DuInstrument::getEqualizer()
{
    return dynamic_cast<DuEqualizer *>(getChild(KEY_INSTR_EQUALIZERSETTINGS));
}

void DuInstrument::setEqualizer(DuEqualizer *equalizer)
{
    if (getChild(KEY_INSTR_EQUALIZERSETTINGS) != NULL)
        delete getChild(KEY_INSTR_EQUALIZERSETTINGS);

    addChild(KEY_INSTR_EQUALIZERSETTINGS, equalizer);
}

DuDelay *DuInstrument::getDelay()
{
    return dynamic_cast<DuDelay *>(getChild(KEY_INSTR_DELAYSETTINGS));
}

void DuInstrument::setDelay(DuDelay *delay)
{
    if (getChild(KEY_INSTR_DELAYSETTINGS) != NULL)
        delete getChild(KEY_INSTR_DELAYSETTINGS);

    addChild(KEY_INSTR_DELAYSETTINGS, delay);
}

DuChorus *DuInstrument::getChorus()
{
    return dynamic_cast<DuChorus *>(getChild(KEY_INSTR_CHORUSSETTINGS));
}

void DuInstrument::setChorus(DuChorus *chorus)
{
    if (getChild(KEY_INSTR_CHORUSSETTINGS) != NULL)
        delete getChild(KEY_INSTR_CHORUSSETTINGS);

    addChild(KEY_INSTR_CHORUSSETTINGS, chorus);
}

DuVibrato *DuInstrument::getVibrato()
{
    return dynamic_cast<DuVibrato *>(getChild(KEY_INSTR_VIBRATOSETTINGS));
}

void DuInstrument::setVibrato(DuVibrato *vibrato)
{
    if (getChild(KEY_INSTR_VIBRATOSETTINGS) != NULL)
        delete getChild(KEY_INSTR_VIBRATOSETTINGS);

    addChild(KEY_INSTR_VIBRATOSETTINGS, vibrato);
}

DuAdsr *DuInstrument::getAdsr()
{
    return dynamic_cast<DuAdsr *>(getChild(KEY_INSTR_ADSRSETTINGS));
}

void DuInstrument::setAdsr(DuAdsr *adsr)
{
    if (getChild(KEY_INSTR_ADSRSETTINGS) != NULL)
        delete getChild(KEY_INSTR_ADSRSETTINGS);

    addChild(KEY_INSTR_ADSRSETTINGS, adsr);
}
