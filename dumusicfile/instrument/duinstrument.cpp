#include "duinstrument.h"

#include "duinstrumentinfo.h"
#include "dupreset.h"

#include <cstring>

#include <QJsonObject>

#include "effects/duchorus.h"
#include "effects/ducompressor.h"
#include "effects/dudelay.h"
#include "effects/dudistortion.h"
#include "effects/duequalizer.h"
#include "effects/dumixer.h"

DU_OBJECT_IMPL(DuInstrument)

DuInstrument::DuInstrument() :
    DuContainer()
{
    addChild(KeyInstrumentInfo, new DuInstrumentInfo);
    addChild(KeyPreset,         new DuPreset);

    addChild(KeyMixer,          new DuMixer);
    addChild(KeyDistortion,     new DuDistortion);
    addChild(KeyCompressor,     new DuCompressor);
    addChild(KeyEqualizer,      new DuEqualizer);
    addChild(KeyDelay,          new DuDelay);
    addChild(KeyChorus,         new DuChorus);
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
        qCCritical(LOG_CAT_DU_OBJECT) << "DuInstrument::fromDuMusicBinary():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuInstrumentInfo was not properly generated";

        return DuInstrumentPtr();
    }
    instrument->setInstrumentInfo(instrInfo);

    const DuPresetPtr &preset =
            DuPreset::fromDuMusicBinary(du_instr.i_preset);
    if (preset == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuInstrument::fromDuMusicBinary():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuPreset was not properly generated";

        return DuInstrumentPtr();
    }
    instrument->setPreset(preset);

    const DuMixerPtr &mixer =
            DuMixer::fromDuMusicBinary(du_instr.i_mix);
    if (mixer == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuInstrument::fromDuMusicBinary():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuMixer was not properly generated";

        return DuInstrumentPtr();
    }
    instrument->setMixer(mixer);

    const DuDistortionPtr &distortion =
            DuDistortion::fromDuMusicBinary(du_instr.i_distortion);
    if (distortion == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuInstrument::fromDuMusicBinary():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuDistortion was not properly generated";

        return DuInstrumentPtr();
    }
    instrument->setDistortion(distortion);

    const DuCompressorPtr &compressor =
            DuCompressor::fromDuMusicBinary(du_instr.i_compressor);
    if (compressor == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuInstrument::fromDuMusicBinary():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuCompressor was not properly generated";

        return DuInstrumentPtr();
    }
    instrument->setCompressor(compressor);

    const DuEqualizerPtr &equalizer =
            DuEqualizer::fromDuMusicBinary(du_instr.i_equalizer);
    if (equalizer == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuInstrument::fromDuMusicBinary():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuEqualizer was not properly generated";

        return DuInstrumentPtr();
    }
    instrument->setEqualizer(equalizer);

    const DuDelayPtr &delay =
            DuDelay::fromDuMusicBinary(du_instr.i_delay);
    if (delay == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuInstrument::fromDuMusicBinary():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuDelay was not properly generated";

        return DuInstrumentPtr();
    }
    instrument->setDelay(delay);

    const DuChorusPtr &chorus =
            DuChorus::fromDuMusicBinary(du_instr.i_chorus);
    if (chorus == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuInstrument::fromDuMusicBinary():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuChorus was not properly generated";

        return DuInstrumentPtr();
    }
    instrument->setChorus(chorus);

    return instrument;
}


DuInstrumentPtr DuInstrument::fromJson(const QJsonObject &jsonInstrument)
{
    QJsonValue jsonInstrInfo    = jsonInstrument[KeyInstrumentInfo];
    QJsonValue jsonPreset       = jsonInstrument[KeyPreset];
    QJsonValue jsonMixer        = jsonInstrument[KeyMixer];
    QJsonValue jsonDistortion   = jsonInstrument[KeyDistortion];
    QJsonValue jsonCompressor   = jsonInstrument[KeyCompressor];
    QJsonValue jsonEqualizer    = jsonInstrument[KeyEqualizer];
    QJsonValue jsonDelay        = jsonInstrument[KeyDelay];
    QJsonValue jsonChorus       = jsonInstrument[KeyChorus];

    if (        !jsonInstrInfo.isObject()   ||  !jsonPreset.isObject()
            ||  !jsonMixer.isObject()       ||  !jsonDistortion.isObject()
            ||  !jsonCompressor.isObject()
            ||  !jsonEqualizer.isObject()   ||  !jsonDelay.isObject()
            ||  !jsonChorus.isObject())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuInstrument::fromJson():\n"
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
        qCCritical(LOG_CAT_DU_OBJECT) << "DuInstrument::fromJson():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuInstrumentInfo was not properly generated";

        return DuInstrumentPtr();
    }

    const DuPresetPtr &preset =
            DuPreset::fromJson(jsonPreset.toObject());
    if (preset != NULL)
        instrument->setPreset(preset);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuInstrument::fromJson():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuPreset was not properly generated";

        return DuInstrumentPtr();
    }

    const DuMixerPtr &mixer =
            DuMixer::fromJson(jsonMixer.toObject());
    if (mixer != NULL)
        instrument->setMixer(mixer);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuInstrument::fromJson():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuMixer was not properly generated";

        return DuInstrumentPtr();
    }

    const DuDistortionPtr &distortion =
            DuDistortion::fromJson(jsonDistortion.toObject());
    if (distortion != NULL)
        instrument->setDistortion(distortion);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuInstrument::fromJson():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuDistortion was not properly generated";

        return DuInstrumentPtr();
    }

    const DuCompressorPtr &compressor =
            DuCompressor::fromJson(jsonCompressor.toObject());
    if (compressor != NULL)
        instrument->setCompressor(compressor);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuInstrument::fromJson():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuCompressor was not properly generated";

        return DuInstrumentPtr();
    }

    const DuEqualizerPtr &equalizer =
            DuEqualizer::fromJson(jsonEqualizer.toObject());
    if (equalizer != NULL)
        instrument->setEqualizer(equalizer);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuInstrument::fromJson():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuEqualizer was not properly generated";

        return DuInstrumentPtr();
    }

    const DuDelayPtr &delay =
            DuDelay::fromJson(jsonDelay.toObject());
    if (delay != NULL)
        instrument->setDelay(delay);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuInstrument::fromJson():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuDelay was not properly generated";

        return DuInstrumentPtr();
    }

    const DuChorusPtr &chorus =
            DuChorus::fromJson(jsonChorus.toObject());
    if (chorus != NULL)
        instrument->setChorus(chorus);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuInstrument::fromJson():\n"
                    << "failed to generate DuInstrument\n"
                    << "the DuChorus was not properly generated";

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
                instrInfo->toDuMusicBinary().constData(), instrInfo->size());

    const DuPresetConstPtr& preset = getPreset();
    if (preset == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_preset),
                preset->toDuMusicBinary().constData(), preset->size());

    const DuMixerConstPtr& mixer = getMixer();
    if (mixer == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_mix),
                mixer->toDuMusicBinary().constData(), mixer->size());

    const DuDistortionConstPtr& distortion = getDistortion();
    if (distortion == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_distortion),
                distortion->toDuMusicBinary().constData(), distortion->size());

    const DuCompressorConstPtr& compressor = getCompressor();
    if (compressor == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_compressor),
                compressor->toDuMusicBinary().constData(), compressor->size());

    const DuEqualizerConstPtr& equalizer = getEqualizer();
    if (equalizer == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_equalizer),
                equalizer->toDuMusicBinary().constData(), equalizer->size());

    const DuDelayConstPtr& delay = getDelay();
    if (delay == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_delay),
                delay->toDuMusicBinary().constData(), delay->size());

    const DuChorusConstPtr& chorus = getChorus();
    if (chorus == NULL)
        return QByteArray();

    std::memcpy((char *)&(du_instrument.i_chorus),
                chorus->toDuMusicBinary().constData(), chorus->size());

    return QByteArray((char *)&(du_instrument), size());
}


int DuInstrument::size() const
{
    return MUSIC_INSTRU_SIZE;
}


DU_KEY_ACCESSORS_OBJECT_IMPL(DuInstrument, InstrumentInfo, DuInstrumentInfo)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuInstrument, Preset,         DuPreset)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuInstrument, Mixer,          DuMixer)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuInstrument, Distortion,     DuDistortion)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuInstrument, Compressor,     DuCompressor)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuInstrument, Equalizer,      DuEqualizer)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuInstrument, Delay,          DuDelay)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuInstrument, Chorus,         DuChorus)
