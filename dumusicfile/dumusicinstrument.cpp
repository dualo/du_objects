#include "dumusicinstrument.h"

#include <cstring>

#include <QJsonObject>

#include "../instrument/duinstrumentinfo.h"
#include "../instrument/dupreset.h"

DU_OBJECT_IMPL(DuMusicInstrument)

DuMusicInstrument::DuMusicInstrument() :
    DuContainer()
{
    addChild(KeyInstrumentInfo, new DuInstrumentInfo);
    addChild(KeyPreset,         new DuPreset);
}

DuMusicInstrument::~DuMusicInstrument()
{
}

DuObjectPtr DuMusicInstrument::clone() const
{
    return DuMusicInstrumentPtr(new DuMusicInstrument(*this));
}


DuMusicInstrumentPtr DuMusicInstrument::fromDuMusicBinary(const music_instr &du_instr)
{
    DuMusicInstrumentPtr instrument(new DuMusicInstrument);

    const DuInstrumentInfoPtr &instrInfo =
            DuInstrumentInfo::fromDuMusicBinary(du_instr.i_instrument);
    if (instrInfo == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMusicInstrument::fromDuMusicBinary():\n"
                    << "failed to generate DuMusicInstrument\n"
                    << "the DuInstrumentInfo was not properly generated";

        return DuMusicInstrumentPtr();
    }
    instrument->setInstrumentInfo(instrInfo);

    const DuPresetPtr &preset =
            DuPreset::fromDuMusicBinary(du_instr.i_preset);
    if (preset == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMusicInstrument::fromDuMusicBinary():\n"
                    << "failed to generate DuMusicInstrument\n"
                    << "the DuPreset was not properly generated";

        return DuMusicInstrumentPtr();
    }
    instrument->setPreset(preset);

    return instrument;
}


DuMusicInstrumentPtr DuMusicInstrument::fromJson(const QJsonObject &jsonInstrument)
{
    QJsonValue jsonInstrInfo    = jsonInstrument[KeyInstrumentInfo];
    QJsonValue jsonPreset       = jsonInstrument[KeyPreset];

    if (!jsonInstrInfo.isObject() || !jsonPreset.isObject())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMusicInstrument::fromJson():\n"
                    << "failed to generate DuMusicInstrument\n"
                    << "a json key did not contain the proper type";

        return DuMusicInstrumentPtr();
    }


    DuMusicInstrumentPtr instrument(new DuMusicInstrument);

    const DuInstrumentInfoPtr &instrInfo =
            DuInstrumentInfo::fromJson(jsonInstrInfo.toObject());
    if (instrInfo != NULL)
        instrument->setInstrumentInfo(instrInfo);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMusicInstrument::fromJson():\n"
                    << "failed to generate DuMusicInstrument\n"
                    << "the DuInstrumentInfo was not properly generated";

        return DuMusicInstrumentPtr();
    }

    const DuPresetPtr &preset =
            DuPreset::fromJson(jsonPreset.toObject());
    if (preset != NULL)
        instrument->setPreset(preset);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMusicInstrument::fromJson():\n"
                    << "failed to generate DuMusicInstrument\n"
                    << "the DuPreset was not properly generated";

        return DuMusicInstrumentPtr();
    }

    return instrument;
}

QByteArray DuMusicInstrument::toDuMusicBinary() const
{
    music_instr du_instrument;
    std::memset((char*)&du_instrument, 0, size());


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

    return QByteArray((char *)&(du_instrument), size());
}


int DuMusicInstrument::size() const
{
    return MUSIC_INSTRU_SIZE;
}


DU_KEY_ACCESSORS_OBJECT_IMPL(DuMusicInstrument, InstrumentInfo, DuInstrumentInfo)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuMusicInstrument, Preset,         DuPreset)
