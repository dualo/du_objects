#include "dusoundinfo.h"

#include "../instrument/duinstrumentinfo.h"
#include "../instrument/dupreset.h"

#include "../general/duarray.h"
#include "../general/dunumeric.h"
#include "../general/dustring.h"

#include <cstring>

DuSoundInfo::DuSoundInfo()
{
    addChild(KeyInstrumentInfo, new DuInstrumentInfo);

    addChild(KeyPresetNum,      new DuNumeric(0, NUMERIC_DEFAULT_SIZE, FX_NUM_PRESET_INTR, 0));

    addChild(KeyName,           new DuString(SOUND_NAME_SIZE));

    DuArrayPtr<DuPreset> presetArray(new DuArray<DuPreset>(FX_NUM_PRESET_INTR));
    for (int i = 0; i < FX_NUM_PRESET_INTR; ++i)
        presetArray->append(new DuPreset);
    addChild(KeyPresetArray, presetArray);
}

DuObjectPtr DuSoundInfo::clone() const
{
    return DuSoundInfoPtr(new DuSoundInfo(*this));
}

int DuSoundInfo::size() const
{
    return INSTRU_STRUCT_SIZE;
}

DuSoundInfoPtr DuSoundInfo::fromBinary(const sound_instr &data)
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

    verif = soundInfo->setPresetNum(data.s_presetnum)   ? verif : false;
    verif = soundInfo->setName(QString::fromUtf8((char*) data.s_complete_name, SOUND_NAME_SIZE)) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "An attribute was not properly set";
    }

    DuArrayPtr<DuPreset> presetArray(new DuArray<DuPreset>);
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

    return soundInfo;
}

QByteArray DuSoundInfo::toBinary(uint8_t nbLayer, int nbSamples, uint32_t sampleSize) const
{
    int tmpInt;

    sound_instr soundStruct;
    std::memset((char*)&soundStruct, 0, size());

    const DuInstrumentInfoConstPtr &m3infos = getInstrumentInfo();
    if (m3infos == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct.s_instrument), m3infos->toBinary(nbLayer, nbSamples, sampleSize).constData(), m3infos->size());

    tmpInt = getPresetNum();
    if (tmpInt == -1)
        return QByteArray();
    soundStruct.s_presetnum = tmpInt;

    QByteArray tmpName(SOUND_NAME_SIZE, (char)0x00);
    QString tmpStr = getName();
    if (tmpStr.isNull())
        return QByteArray();
    tmpName.prepend(tmpStr.toUtf8());
    std::memcpy(soundStruct.s_complete_name, tmpName.constData(), SOUND_NAME_SIZE);

    const DuArrayConstPtr<DuPreset> &presetArray = getPresetArray();
    if (presetArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct.s_preset), presetArray->toDuMusicBinary().constData(), presetArray->size());

    return QByteArray((char*)&soundStruct, size());
}

DuObjectPtr DuSoundInfo::getChild(const QString &key)
{
    if (    key == KeyNameForDevice      ||
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
    if (    key == KeyNameForDevice      ||
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

DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, NameForDevice,      DuInstrumentInfo, InstrumentInfo, QString, QString())
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, KeyMapping,         DuInstrumentInfo, InstrumentInfo, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, Octave,             DuInstrumentInfo, InstrumentInfo, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, UserID,             DuInstrumentInfo, InstrumentInfo, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, ID,                 DuInstrumentInfo, InstrumentInfo, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, ActiveNoteOff,      DuInstrumentInfo, InstrumentInfo, bool, false)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, Category,           DuInstrumentInfo, InstrumentInfo, QString, QString())
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, RelativeVolume,     DuInstrumentInfo, InstrumentInfo, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, InstrType,          DuInstrumentInfo, InstrumentInfo, INSTRUMENT_TYPE, NUM_INSTR_TYPE)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, InstrVersion,       DuInstrumentInfo, InstrumentInfo, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, HardInstrVersion,   DuInstrumentInfo, InstrumentInfo, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSoundInfo, SoftInstrVersion,   DuInstrumentInfo, InstrumentInfo, int, -1)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSoundInfo, InstrumentInfo,  DuInstrumentInfo)

DU_KEY_ACCESSORS_IMPL(DuSoundInfo, PresetNum,  Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuSoundInfo, Name,       String,  QString, QString())

DU_KEY_ACCESSORS_OBJECT_TEMPLATE_IMPL(DuSoundInfo, PresetArray, DuArray, DuPreset)
