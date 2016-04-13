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

DuObjectPtr DuMusicInstrument::getChild(const QString &key)
{
    if (key == KeyNameForDevice)
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

DuObjectConstPtr DuMusicInstrument::getChild(const QString &key) const
{
    if (key == KeyNameForDevice)
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

DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuMusicInstrument, NameForDevice, DuInstrumentInfo, InstrumentInfo, QString, QString())

DU_KEY_ACCESSORS_OBJECT_IMPL(DuMusicInstrument, InstrumentInfo, DuInstrumentInfo)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuMusicInstrument, Preset,         DuPreset)
