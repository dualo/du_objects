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

DuObjectPtr DuMusicInstrument::clone() const
{
    return DuMusicInstrumentPtr(new DuMusicInstrument(*this));
}


DuMusicInstrumentPtr DuMusicInstrument::fromDuMusicBinary(const music_instr &du_instr)
{
    DuMusicInstrumentPtr instrument(new DuMusicInstrument);

    const DuInstrumentInfoPtr &instrInfo =
            DuInstrumentInfo::fromDuMusicBinary(du_instr.i_instrument);
    if (instrInfo == Q_NULLPTR)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMusicInstrument::fromDuMusicBinary():\n"
                    << "failed to generate DuMusicInstrument\n"
                    << "the DuInstrumentInfo was not properly generated";

        return DuMusicInstrumentPtr();
    }
    instrument->setInstrumentInfo(instrInfo);

    const DuPresetPtr &preset =
            DuPreset::fromDuMusicBinary(du_instr.i_preset);
    if (preset == Q_NULLPTR)
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
    std::memset(&du_instrument, 0, static_cast<size_t>(size()));


    const DuInstrumentInfoConstPtr& instrInfo = getInstrumentInfo();
    if (instrInfo == Q_NULLPTR)
        return QByteArray();

    std::memcpy(&(du_instrument.i_instrument),
                instrInfo->toDuMusicBinary().constData(),
                static_cast<size_t>(instrInfo->size()));

    const DuPresetConstPtr& preset = getPreset();
    if (preset == Q_NULLPTR)
        return QByteArray();

    std::memcpy(&(du_instrument.i_preset),
                preset->toDuMusicBinary().constData(),
                static_cast<size_t>(preset->size()));

    return QByteArray(reinterpret_cast<char*>(&du_instrument), size());
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
        if (info == Q_NULLPTR)
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
        if (info == Q_NULLPTR)
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
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuMusicInstrument, UserID,        DuInstrumentInfo, InstrumentInfo, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuMusicInstrument, ID,            DuInstrumentInfo, InstrumentInfo, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuMusicInstrument, InstrVersion,  DuInstrumentInfo, InstrumentInfo, int, -1)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuMusicInstrument, InstrumentInfo, DuInstrumentInfo)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuMusicInstrument, Preset,         DuPreset)
