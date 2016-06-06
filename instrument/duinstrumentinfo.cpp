#include "duinstrumentinfo.h"

#include "../general/dustring.h"
#include "../general/dunumeric.h"

#include <cstring>

#include <QJsonObject>

QList<DuInstrumentInfo::Category> DuInstrumentInfo::categoryMap = QList<DuInstrumentInfo::Category>()
    << DuInstrumentInfo::Category({  0, "system",       -1             })
    << DuInstrumentInfo::Category({  1, "percussions",  INSTR_PERCU    })
    << DuInstrumentInfo::Category({  2, "mallet",       INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  3, "hammer",       INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  4, "el. piano",    INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  5, "bass",         INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  6, "ac. guitar",   INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  7, "el. guitar",   INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  8, "dist. guitar", INSTR_HARMONIC })
    << DuInstrumentInfo::Category({  9, "plucked",      INSTR_HARMONIC })
    << DuInstrumentInfo::Category({ 10, "strings",      INSTR_HARMONIC })
    << DuInstrumentInfo::Category({ 11, "reeds",        INSTR_HARMONIC })
    << DuInstrumentInfo::Category({ 12, "pipe organ",   INSTR_HARMONIC })
    << DuInstrumentInfo::Category({ 13, "el. organ",    INSTR_HARMONIC })
    << DuInstrumentInfo::Category({ 14, "wind",         INSTR_HARMONIC })
    << DuInstrumentInfo::Category({ 15, "flute",        INSTR_HARMONIC })
    << DuInstrumentInfo::Category({ 16, "brass",        INSTR_HARMONIC })
    << DuInstrumentInfo::Category({ 17, "el. percu",    INSTR_PERCU    })
    << DuInstrumentInfo::Category({ 18, "perc synth",   INSTR_HARMONIC })
    << DuInstrumentInfo::Category({ 19, "mod bass",     INSTR_HARMONIC })
    << DuInstrumentInfo::Category({ 20, "club bass",    INSTR_HARMONIC })
    << DuInstrumentInfo::Category({ 21, "synth bass",   INSTR_HARMONIC })
    << DuInstrumentInfo::Category({ 22, "soft pad",     INSTR_HARMONIC })
    << DuInstrumentInfo::Category({ 23, "bright pad",   INSTR_HARMONIC })
    << DuInstrumentInfo::Category({ 24, "soft lead",    INSTR_HARMONIC })
    << DuInstrumentInfo::Category({ 25, "hard lead",    INSTR_HARMONIC })
    << DuInstrumentInfo::Category({ 26, "pure sound",   INSTR_HARMONIC })
    << DuInstrumentInfo::Category({ 27, "choir",        INSTR_HARMONIC })
    << DuInstrumentInfo::Category({ 28, "samples",      INSTR_SAMPLE   })
    << DuInstrumentInfo::Category({ 29, "synth brass",  INSTR_HARMONIC })
    << DuInstrumentInfo::Category({ 30, "other",        INSTR_HARMONIC });
    // WARNING : names must have a size inferior to NAME_CARACT

QString DuInstrumentInfo::getCategoryNameFromProgramChange(int programChange)
{
    foreach (const Category& category, categoryMap)
    {
        if (category.programChange == programChange)
        {
            return category.name;
        }
    }

    return QString();
}

int DuInstrumentInfo::getProgramChangeFromCategoryName(const QString name)
{
    foreach (const Category& category, categoryMap)
    {
        if (category.name == name)
        {
            return category.programChange;
        }
    }

    return -1;
}

QStringList DuInstrumentInfo::getCategoriesFromType(int type)
{
    QStringList list;

    foreach (const Category& category, categoryMap)
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

    addChild(KeyKeyMapping,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           INSTRUMENT_MAPPING_MAX, 0x00));

    addChild(KeyOctave,
             new DuNumeric(MAIN_OCTAVE_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_OCTAVE_MAX, MAIN_OCTAVE_MIN));

    addChild(KeyUserID, new DuNumeric(0)); //TODO: get user id

    addChild(KeyID, new DuNumeric(0)); //TODO: generate new id

    addChild(KeyActiveNoteOff,
             new DuNumeric(0x01, NUMERIC_DEFAULT_SIZE,
                           0xFF, 0x00));

    addChild(KeyCategory, new DuString(NAME_CARACT));

    addChild(KeyRelativeVolume,
             new DuNumeric(0x7F, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KeyDreamFormatId,
             new DuNumeric(NO_FORMAT, NUMERIC_DEFAULT_SIZE,
                           SDK_5000, FORMAT_ERROR)); //TODO: delete that

    addChild(KeyInstrType,
             new DuNumeric(INSTR_HARMONIC, NUMERIC_DEFAULT_SIZE,
                           NUM_INSTR_TYPE - 1, INSTR_HARMONIC));

    addChild(KeyInstrVersion, new DuNumeric(0));

    addChild(KeyHardInstrVersion,
             new DuNumeric(0x0000, 2, 0xFFFF, 0x0000));

    addChild(KeySoftInstrVersion,
             new DuNumeric(0x0000, 2, 0xFFFF, 0x0000));
}

DuInstrumentInfo::~DuInstrumentInfo()
{
}

DuObjectPtr DuInstrumentInfo::clone() const
{
    return DuInstrumentInfoPtr(new DuInstrumentInfo(*this));
}


DuInstrumentInfoPtr DuInstrumentInfo::fromDuMusicBinary(const info_instr &du_instrInfo)
{
    DuInstrumentInfoPtr instrInfo(new DuInstrumentInfo);
    bool verif = true;

    verif = instrInfo->setNameForDevice(QString(QByteArray((char *)du_instrInfo.instr_name, NAME_CARACT))) ? verif : false;

    verif = instrInfo->setKeyMapping(du_instrInfo.instr_key_map) ? verif : false;
    verif = instrInfo->setOctave(du_instrInfo.instr_octave) ? verif : false;

    verif = instrInfo->setUserID((int) du_instrInfo.instr_user_id) ? verif : false;
    verif = instrInfo->setID((int) du_instrInfo.instr_id) ? verif : false;

    verif = instrInfo->setActiveNoteOff(du_instrInfo.instr_noteoff) ? verif : false;

    verif = instrInfo->setCategory(getCategoryNameFromProgramChange(du_instrInfo.instr_midi_pc)) ? verif : false;

    verif = instrInfo->setRelativeVolume(du_instrInfo.instr_relvolume) ? verif : false;

    verif = instrInfo->setDreamFormatId((DreamFormat)du_instrInfo.format_id) ? verif : false;

    verif = instrInfo->setInstrType((INSTRUMENT_TYPE)du_instrInfo.instr_type) ? verif : false;
    verif = instrInfo->setInstrVersion((int) du_instrInfo.instr_version) ? verif : false;
    verif = instrInfo->setHardInstrVersion(du_instrInfo.HW_instr_version) ? verif : false;
    verif = instrInfo->setSoftInstrVersion(du_instrInfo.SW_instr_version) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "an attribute was not properly set";
    }

    return instrInfo;
}

bool DuInstrumentInfo::toStruct(info_instr& outStruct) const
{
    QString tmpStr;
    int tmpNum = 0;

    std::memset((char*)&outStruct, 0, (size_t) size());

    QByteArray tmpName(NAME_CARACT, (char)0x00);
    tmpStr = getNameForDevice();
    if (tmpStr.isNull())
        return false;
    tmpName.prepend(tmpStr.toUtf8());

    std::memcpy(outStruct.instr_name, tmpName.data(), NAME_CARACT);

    tmpNum = getProgramChangeFromCategoryName(getCategory());
    if (tmpNum == -1)
        return false;
    outStruct.instr_midi_pc = (uint8_t) tmpNum;

    tmpNum = getKeyMapping();
    if (tmpNum == -1)
        return false;
    outStruct.instr_key_map = (uint8_t) tmpNum;

    tmpNum = getOctave();
    if (tmpNum == -1)
        return false;
    outStruct.instr_octave = (uint8_t) tmpNum;

    tmpNum = getUserID();
    if (tmpNum == -1)
        return false;
    outStruct.instr_user_id = (uint32_t) tmpNum;

    tmpNum = getID();
    if (tmpNum == -1)
        return false;
    outStruct.instr_id = (uint32_t) tmpNum;

    tmpNum = getActiveNoteOff();
    if (tmpNum == -1)
        return false;
    outStruct.instr_noteoff = (uint8_t) tmpNum;

    QByteArray tmpCategory(NAME_CARACT, (char)0x00);
    tmpStr = getCategory();
    if (tmpStr.isNull())
        return false;
    tmpCategory.prepend(tmpStr.toUtf8());

    std::memcpy(outStruct.instr_cat, tmpCategory.data(), NAME_CARACT);

    tmpNum = getRelativeVolume();
    if (tmpNum == -1)
        return false;
    outStruct.instr_relvolume = (uint8_t) tmpNum;

    DreamFormat tmpFormat = getDreamFormatId();
    if (tmpFormat == FORMAT_ERROR)
        return false;
    outStruct.format_id = (uint8_t)tmpFormat;

    INSTRUMENT_TYPE tmpType = getInstrType();
    if (tmpType == NUM_INSTR_TYPE)
        return false;
    outStruct.instr_type = (uint8_t)tmpType;

    tmpNum = getInstrVersion();
    if (tmpNum == -1)
        return false;
    outStruct.instr_version = (uint32_t) tmpNum;

    tmpNum = getHardInstrVersion();
    if (tmpNum == -1)
        return false;
    outStruct.HW_instr_version = (uint16_t) tmpNum;

    tmpNum = getSoftInstrVersion();
    if (tmpNum == -1)
        return false;
    outStruct.SW_instr_version = (uint16_t) tmpNum;

    return true;
}

QByteArray DuInstrumentInfo::toDuMusicBinary() const
{
    info_instr du_instrumentinfo;
    if (!toStruct(du_instrumentinfo))
    {
        return QByteArray();
    }

    return QByteArray((char *)&(du_instrumentinfo), size());
}


QByteArray DuInstrumentInfo::toBinary(uint8_t nbLayer, int nbSamples, uint32_t sampleSize) const
{
    info_instr du_instrumentinfo;
    if (!toStruct(du_instrumentinfo))
    {
        return QByteArray();
    }

    du_instrumentinfo.nb_layer = nbLayer;

    du_instrumentinfo.ip_size = (uint16_t) nbLayer * 2 + (uint16_t) nbSamples * INSTR_DREAM_IP_SIZE;

    du_instrumentinfo.sp_size = (uint16_t) nbSamples * INSTR_DREAM_SP_SIZE;

    du_instrumentinfo.sample_size = sampleSize;

    return QByteArray((char *)&(du_instrumentinfo), size());
}

int DuInstrumentInfo::size() const
{
    return INSTR_INFO_SIZE;
}


DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, NameForDevice,      String, QString, QString())

DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, KeyMapping,         Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, Octave,             Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, UserID,             Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, ID,                 Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, ActiveNoteOff,      Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, Category,           String, QString, QString())

DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, RelativeVolume,     Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, DreamFormatId,      Numeric, DuInstrumentInfo::DreamFormat, FORMAT_ERROR)

DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, InstrType,          Numeric, INSTRUMENT_TYPE, NUM_INSTR_TYPE)
DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, InstrVersion,       Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, HardInstrVersion,   Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, SoftInstrVersion,   Numeric, int, -1)
