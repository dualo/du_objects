#include "duinstrumentinfo.h"

#include "../general/duboolean.h"
#include "../general/DuEnum.h"
#include "../general/dustring.h"
#include "../general/dunumeric.h"

#include <cstring>

#include <QJsonObject>

QList<DuInstrumentInfo::Category> DuInstrumentInfo::categoryMap = QList<DuInstrumentInfo::Category>()
    << DuInstrumentInfo::Category({   0, QLatin1String("system"),       -1             })
    << DuInstrumentInfo::Category({   1, QLatin1String("percussions"),  INSTR_PERCU    })
    << DuInstrumentInfo::Category({   2, QLatin1String("mallet"),       INSTR_HARMONIC })
    << DuInstrumentInfo::Category({   3, QLatin1String("hammer"),       INSTR_HARMONIC })
    << DuInstrumentInfo::Category({   4, QLatin1String("el. piano"),    INSTR_HARMONIC })
    << DuInstrumentInfo::Category({   5, QLatin1String("bass"),         INSTR_HARMONIC })
    << DuInstrumentInfo::Category({   6, QLatin1String("ac. guitar"),   INSTR_HARMONIC })
    << DuInstrumentInfo::Category({   7, QLatin1String("el. guitar"),   INSTR_HARMONIC })
    << DuInstrumentInfo::Category({   8, QLatin1String("dist. guitar"), INSTR_HARMONIC })
    << DuInstrumentInfo::Category({   9, QLatin1String("plucked"),      INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  10, QLatin1String("strings"),      INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  11, QLatin1String("reeds"),        INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  12, QLatin1String("pipe organ"),   INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  13, QLatin1String("el. organ"),    INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  14, QLatin1String("wind"),         INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  15, QLatin1String("flute"),        INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  16, QLatin1String("brass"),        INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  17, QLatin1String("el. percu"),    INSTR_PERCU    })
    << DuInstrumentInfo::Category({  18, QLatin1String("perc synth"),   INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  19, QLatin1String("mod bass"),     INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  20, QLatin1String("club bass"),    INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  21, QLatin1String("synth bass"),   INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  22, QLatin1String("soft pad"),     INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  23, QLatin1String("bright pad"),   INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  24, QLatin1String("soft lead"),    INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  25, QLatin1String("hard lead"),    INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  26, QLatin1String("pure sound"),   INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  27, QLatin1String("choir"),        INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  28, QLatin1String("samples"),      INSTR_SAMPLE   })
    << DuInstrumentInfo::Category({  29, QLatin1String("synth brass"),  INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  30, QLatin1String("other"),        INSTR_HARMONIC })
    << DuInstrumentInfo::Category({0xFF, QLatin1String("unknown"),      -1             });
    // WARNING : names must have a size inferior to NAME_CARACT

QString DuInstrumentInfo::getCategoryNameFromProgramChange(int programChange)
{
    for (const Category& category : categoryMap)
    {
        if (category.programChange == programChange)
        {
            return category.name;
        }
    }

    return QStringLiteral("unknown");
}

int DuInstrumentInfo::getProgramChangeFromCategoryName(const QString name)
{
    for (const Category& category : categoryMap)
    {
        if (category.name == name)
        {
            return category.programChange;
        }
    }

    // 0xFF : unknown category or migrated du-music from v2
    return 0xFF;
}

QStringList DuInstrumentInfo::getCategoriesFromType(int type)
{
    QStringList list;

    for (const Category& category : categoryMap)
    {
        if (category.type == type)
        {
            list << category.name;
        }
    }

    return list;
}

DU_OBJECT_IMPL(DuInstrumentInfo)

DuInstrumentInfo::DuInstrumentInfo() :
    DuContainer()
{
    addChild(KeyNameForDevice, new DuString(NAME_CARACT));

    addChild(KeyOctave,
             new DuNumeric(MAIN_OCTAVE_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_OCTAVE_MAX, MAIN_OCTAVE_MIN));

    addChild(KeyUserID, new DuNumeric(0));

    addChild(KeyID, new DuNumeric(0));

    addChild(KeyActiveNoteOff, new DuBoolean(true));

    addChild(KeyCategory, new DuString(NAME_CARACT));

    addChild(KeyRelativeVolume,
             new DuNumeric(0x7F, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KeyInstrType,
             new DuEnum<INSTRUMENT_TYPE>(INSTR_HARMONIC, NUMERIC_DEFAULT_SIZE,
                                         NUM_INSTR_TYPE, INSTR_HARMONIC));

    addChild(KeyInstrVersion, new DuNumeric(0));

    addChild(KeyHardInstrVersion,
             new DuNumeric(0x0000, 2, 0xFFFF, 0x0000));

    addChild(KeySoftInstrVersion,
             new DuNumeric(0x0000, 2, 0xFFFF, 0x0000));
}

DuObjectPtr DuInstrumentInfo::clone() const
{
    return DuInstrumentInfoPtr(new DuInstrumentInfo(*this));
}


DuInstrumentInfoPtr DuInstrumentInfo::fromDuMusicBinary(const info_instr &du_instrInfo)
{
    DuInstrumentInfoPtr instrInfo(new DuInstrumentInfo);
    bool verif = true;

    verif = instrInfo->setNameForDevice(DuString::fromStruct(du_instrInfo.instr_name, NAME_CARACT)) ? verif : false;

    verif = instrInfo->setOctave(du_instrInfo.instr_octave) ? verif : false;

    verif = instrInfo->setUserID(static_cast<int>(du_instrInfo.instr_user_id)) ? verif : false;
    verif = instrInfo->setID(static_cast<int>(du_instrInfo.instr_id)) ? verif : false;

    verif = instrInfo->setActiveNoteOff(du_instrInfo.instr_noteoff == 0) ? verif : false;

    verif = instrInfo->setCategory(getCategoryNameFromProgramChange(du_instrInfo.instr_midi_pc)) ? verif : false;

    verif = instrInfo->setRelativeVolume(du_instrInfo.instr_relvolume) ? verif : false;

    verif = instrInfo->setInstrType(static_cast<INSTRUMENT_TYPE>(du_instrInfo.instr_type)) ? verif : false;
    verif = instrInfo->setInstrVersion(static_cast<int>(du_instrInfo.instr_version)) ? verif : false;
    verif = instrInfo->setHardInstrVersion(du_instrInfo.HW_instr_version) ? verif : false;
    verif = instrInfo->setSoftInstrVersion(du_instrInfo.SW_instr_version) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "an attribute was not properly set";
    }

    return instrInfo;
}

bool DuInstrumentInfo::toStruct(info_instr& outStruct, bool forDuTouchSOrL) const
{
    QString tmpStr;
    int tmpNum = 0;

    std::memset(&outStruct, 0, static_cast<size_t>(size()));

    QByteArray tmpName(NAME_CARACT, 0x00);
    tmpStr = getNameForDevice();
    if (tmpStr.isNull())
        return false;
    tmpName.prepend(tmpStr.toLatin1());

    std::memcpy(outStruct.instr_name, tmpName.data(), NAME_CARACT);

    tmpNum = getProgramChangeFromCategoryName(getCategory());
    if (tmpNum == -1)
        return false;
    outStruct.instr_midi_pc = static_cast<quint8>(tmpNum);

    outStruct.instr_key_map = 0;

    tmpNum = getOctave();
    if (tmpNum == -1)
        return false;
    outStruct.instr_octave = static_cast<quint8>(tmpNum);

    tmpNum = getUserID();
    if (tmpNum == -1)
        return false;
    outStruct.instr_user_id = static_cast<quint32>(tmpNum);

    tmpNum = getID();
    if (tmpNum == -1)
        return false;
    outStruct.instr_id = static_cast<quint32>(tmpNum);

    tmpNum = getActiveNoteOff() ? 0 : 1;
    if (tmpNum == -1)
        return false;
    outStruct.instr_noteoff = static_cast<quint8>(tmpNum);

    QByteArray tmpCategory(NAME_CARACT, 0x00);
    tmpStr = getCategory();
    if (tmpStr.isNull())
        return false;
    tmpCategory.prepend(tmpStr.toLatin1());

    std::memcpy(outStruct.instr_cat, tmpCategory.data(), NAME_CARACT);

    tmpNum = getRelativeVolume();
    if (tmpNum == -1)
        return false;
    outStruct.instr_relvolume = static_cast<quint8>(tmpNum);

    outStruct.format_id = static_cast<quint8>(forDuTouchSOrL ? SDK_5000 : SDK_3000);

    INSTRUMENT_TYPE tmpType = getInstrType();
    if (tmpType == static_cast<INSTRUMENT_TYPE>(-1))
        return false;
    outStruct.instr_type = static_cast<quint8>(tmpType);

    tmpNum = getInstrVersion();
    if (tmpNum == -1)
        return false;
    outStruct.instr_version = static_cast<quint32>(tmpNum);

    tmpNum = getHardInstrVersion();
    if (tmpNum == -1)
        return false;
    outStruct.HW_instr_version = static_cast<quint16>(tmpNum);

    tmpNum = getSoftInstrVersion();
    if (tmpNum == -1)
        return false;
    outStruct.SW_instr_version = static_cast<quint16>(tmpNum);

    return true;
}

QByteArray DuInstrumentInfo::toDuMusicBinary() const
{
    info_instr du_instrumentinfo;
    if (!toStruct(du_instrumentinfo, false))
    {
        return QByteArray();
    }

    return QByteArray(reinterpret_cast<char*>(&du_instrumentinfo), size());
}


QByteArray DuInstrumentInfo::toBinary(uint8_t nbLayer, int nbSamples, uint32_t sampleSize, bool forDuTouchSOrL) const
{
    info_instr du_instrumentinfo;
    if (!toStruct(du_instrumentinfo, forDuTouchSOrL))
    {
        return QByteArray();
    }

    du_instrumentinfo.nb_layer = nbLayer;

    du_instrumentinfo.ip_size = static_cast<quint16>(nbLayer) * 2 + (forDuTouchSOrL ? 2 : 0) + static_cast<quint16>(nbSamples) * INSTR_DREAM_IP_SIZE;

    du_instrumentinfo.sp_size = static_cast<quint16>(nbSamples) * INSTR_DREAM_SP_SIZE;

    du_instrumentinfo.sample_size = sampleSize;

    return QByteArray(reinterpret_cast<char*>(&du_instrumentinfo), size());
}

int DuInstrumentInfo::size() const
{
    return INSTR_INFO_SIZE;
}


DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, NameForDevice,      String, QString, QString())

DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, Octave,             Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, UserID,             Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, ID,                 Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, ActiveNoteOff,      Boolean, bool, false)

DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, Category,           String, QString, QString())

DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, RelativeVolume,     Numeric, int, -1)

DU_KEY_ACCESSORS_TEMPLATE_IMPL(DuInstrumentInfo, InstrType, Enum, INSTRUMENT_TYPE, INSTRUMENT_TYPE, static_cast<INSTRUMENT_TYPE>(-1))
DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, InstrVersion,       Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, HardInstrVersion,   Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, SoftInstrVersion,   Numeric, int, -1)
