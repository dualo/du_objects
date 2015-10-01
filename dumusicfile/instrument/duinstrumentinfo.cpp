#include "duinstrumentinfo.h"

#include "../../general/dustring.h"
#include "../../general/dunumeric.h"

#include <cstring>

#include <QJsonObject>

DU_OBJECT_IMPL(DuInstrumentInfo)

DuInstrumentInfo::DuInstrumentInfo() :
    DuContainer()
{
    addChild(KeyName, new DuString(NAME_CARACT));

    addChild(KeyDreamProgramChange,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KeyMidiControlChange0,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KeyKeyMapping,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x04, 0x00));

    addChild(KeyOctave,
             new DuNumeric(MAIN_OCTAVE_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_OCTAVE_MAX, MAIN_OCTAVE_MIN));

    addChild(KeyUserID, new DuNumeric(0));

    addChild(KeyID, new DuNumeric(0));

    addChild(KeyActiveNoteOff,
             new DuNumeric(0x01, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KeyCategory, new DuString(NAME_CARACT));

    addChild(KeyRelativeVolume,
             new DuNumeric(0x40, NUMERIC_DEFAULT_SIZE,
                           0xFF, 0x00));

    addChild(KeyDreamFormatId,
             new DuNumeric(NO_FORMAT, NUMERIC_DEFAULT_SIZE,
                           SDK_5000, NO_FORMAT));

    addChild(KeyInstrType,
             new DuNumeric(INSTR_HARMONIC, NUMERIC_DEFAULT_SIZE,
                           NUM_INSTR_TYPE, INSTR_HARMONIC));

    addChild(KeyInstrVersion,
             new DuNumeric(0, NUMERIC_DEFAULT_SIZE,
                           0xFF, 0x00));
}

DuInstrumentInfo::~DuInstrumentInfo()
{
}

DuObjectPtr DuInstrumentInfo::clone() const
{
    return DuInstrumentInfoPtr(new DuInstrumentInfo(*this));
}


DuInstrumentInfoPtr DuInstrumentInfo::fromDuMusicBinary(const s_instr &du_instrInfo)
{
    DuInstrumentInfoPtr instrInfo(new DuInstrumentInfo);
    bool verif = true;

    verif = instrInfo->setName(QString(QByteArray((char *)du_instrInfo.instr_name, NAME_CARACT))) ? verif : false;

    verif = instrInfo->setDreamProgramChange(du_instrInfo.instr_midi_pc) ? verif : false;
    verif = instrInfo->setMidiControlChange0(du_instrInfo.instr_midi_C0) ? verif : false;

    verif = instrInfo->setKeyMapping(du_instrInfo.instr_key_map) ? verif : false;
    verif = instrInfo->setOctave(du_instrInfo.instr_octave) ? verif : false;

    verif = instrInfo->setUserID(du_instrInfo.instr_user_id) ? verif : false;
    verif = instrInfo->setID(du_instrInfo.instr_id) ? verif : false;

    verif = instrInfo->setActiveNoteOff(du_instrInfo.instr_noteoff) ? verif : false;

    verif = instrInfo->setCategory(QString(QByteArray((char *)du_instrInfo.instr_cat, NAME_CARACT))) ? verif : false;

    verif = instrInfo->setRelativeVolume(du_instrInfo.instr_relvolume) ? verif : false;

    verif = instrInfo->setDreamFormatId((DreamFormat)du_instrInfo.format_id) ? verif : false;

    verif = instrInfo->setInstrType((INSTRUMENT_TYPE)du_instrInfo.instr_type) ? verif : false;
    verif = instrInfo->setInstrVersion(du_instrInfo.instr_version) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "an attribute was not properly set";
    }

    return instrInfo;
}


DuInstrumentInfoPtr DuInstrumentInfo::fromJson(const QJsonObject &jsonInstrInfo)
{
    QJsonValue jsonName         = jsonInstrInfo[KeyName];
    QJsonValue jsonProgChange   = jsonInstrInfo[KeyDreamProgramChange];
    QJsonValue jsonCtrlChange   = jsonInstrInfo[KeyMidiControlChange0];
    QJsonValue jsonKeyMap       = jsonInstrInfo[KeyKeyMapping];
    QJsonValue jsonOctave       = jsonInstrInfo[KeyOctave];
    QJsonValue jsonId           = jsonInstrInfo[KeyID];
    QJsonValue jsonNoteOff      = jsonInstrInfo[KeyActiveNoteOff];
    QJsonValue jsonCategory     = jsonInstrInfo[KeyCategory];
    QJsonValue jsonRelVolume    = jsonInstrInfo[KeyRelativeVolume];
    QJsonValue jsonType         = jsonInstrInfo[KeyInstrType];
    QJsonValue jsonUserId       = jsonInstrInfo[KeyUserID];

    if (        !jsonName.isString()        ||  !jsonProgChange.isDouble()
            ||  !jsonCtrlChange.isDouble()  ||  !jsonKeyMap.isDouble()
            ||  !jsonOctave.isDouble()      ||  !jsonId.isDouble()
            ||  !jsonNoteOff.isDouble()     ||  !jsonCategory.isString()
            ||  !jsonRelVolume.isDouble()   ||  !jsonType.isDouble()
            ||  !jsonUserId.isString())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuInstrumentInfo::fromJson():\n"
                    << "failed to generate DuInstrumentInfo\n"
                    << "a json key did not contain the proper type";

        return DuInstrumentInfoPtr();
    }


    DuInstrumentInfoPtr instrInfo(new DuInstrumentInfo);
    bool verif = true;

    verif = instrInfo->setName(jsonName.toString()) ? verif : false;

    verif = instrInfo->setDreamProgramChange(jsonProgChange.toInt()) ? verif : false;
    verif = instrInfo->setMidiControlChange0(jsonCtrlChange.toInt()) ? verif : false;

    verif = instrInfo->setKeyMapping(jsonKeyMap.toInt()) ? verif : false;
    verif = instrInfo->setOctave(jsonOctave.toInt()) ? verif : false;
    verif = instrInfo->setID(jsonId.toInt()) ? verif : false;

    verif = instrInfo->setActiveNoteOff(jsonNoteOff.toInt()) ? verif : false;
    verif = instrInfo->setCategory(jsonCategory.toString()) ? verif : false;

    verif = instrInfo->setRelativeVolume(jsonRelVolume.toInt()) ? verif : false;

    verif = instrInfo->setInstrType((INSTRUMENT_TYPE)jsonType.toInt()) ? verif : false;

    verif = instrInfo->setUserID(jsonUserId.toInt()) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuInstrumentInfo::fromJson():\n"
                   << "an attribute was not properly set";
    }

    return instrInfo;
}

bool DuInstrumentInfo::toStruct(s_instr& outStruct) const
{
    QString tmpStr;
    int tmpNum = 0;

    std::memcpy((char *)&(outStruct), QByteArray(size(), 0x00), size());

    QByteArray tmpName(NAME_CARACT, (char)0x00);
    tmpStr = getName();
    if (tmpStr.isNull())
        return false;
    tmpName.prepend(tmpStr.toUtf8());

    std::memcpy(outStruct.instr_name, tmpName.data(), NAME_CARACT);

    tmpNum = getDreamProgramChange();
    if (tmpNum == -1)
        return false;
    outStruct.instr_midi_pc = tmpNum;

    tmpNum = getMidiControlChange0();
    if (tmpNum == -1)
        return false;
    outStruct.instr_midi_C0 = tmpNum;

    tmpNum = getKeyMapping();
    if (tmpNum == -1)
        return false;
    outStruct.instr_key_map = tmpNum;

    tmpNum = getOctave();
    if (tmpNum == -1)
        return false;
    outStruct.instr_octave = tmpNum;

    tmpNum = getUserID();
    if (tmpNum == -1)
        return false;
    outStruct.instr_user_id = tmpNum;

    tmpNum = getID();
    if (tmpNum == -1)
        return false;
    outStruct.instr_id = tmpNum;

    tmpNum = getActiveNoteOff();
    if (tmpNum == -1)
        return false;
    outStruct.instr_noteoff = tmpNum;

    QByteArray tmpCategory(NAME_CARACT, (char)0x00);
    tmpStr = getCategory();
    if (tmpStr.isNull())
        return false;
    tmpCategory.prepend(tmpStr.toUtf8());

    std::memcpy(outStruct.instr_cat, tmpCategory.data(), NAME_CARACT);

    tmpNum = getRelativeVolume();
    if (tmpNum == -1)
        return false;
    outStruct.instr_relvolume = tmpNum;

    DreamFormat tmpFormat = getDreamFormatId();
    if (tmpFormat == NO_FORMAT)
        return false;
    outStruct.format_id = (uint8_t)tmpFormat;

    INSTRUMENT_TYPE tmpType = getInstrType();
    if (tmpType == NUM_INSTR_TYPE)
        return false;
    outStruct.instr_type = (uint8_t)tmpType;

    tmpNum = getInstrVersion();
    if (tmpNum == -1)
        return false;
    outStruct.instr_version = tmpNum;

    return true;
}

QByteArray DuInstrumentInfo::toDuMusicBinary() const
{
    s_instr du_instrumentinfo;
    if (!toStruct(du_instrumentinfo))
    {
        return QByteArray();
    }

    return QByteArray((char *)&(du_instrumentinfo), size());
}


QByteArray DuInstrumentInfo::toBinary(uint32_t sampleAddress, uint8_t nbLayer, int nbSamples, uint32_t sampleSize) const
{
    s_instr du_instrumentinfo;
    if (!toStruct(du_instrumentinfo))
    {
        return QByteArray();
    }

    du_instrumentinfo.sample_address = sampleAddressReadableToDream(sampleAddress);

    du_instrumentinfo.nb_layer = nbLayer;

    du_instrumentinfo.ip_size = nbLayer * 2 + nbSamples * INSTR_DREAM_IP_SIZE;

    du_instrumentinfo.sp_size = nbSamples * INSTR_DREAM_SP_SIZE;

    du_instrumentinfo.sample_size = sampleSize;

    return QByteArray((char *)&(du_instrumentinfo), size());
}

uint32_t DuInstrumentInfo::sampleAddressDreamToReadable(uint32_t dreamValue)
{
    return (dreamValue - SOUNDBANK_STARTADRESS) * 2;
}

uint32_t DuInstrumentInfo::sampleAddressReadableToDream(uint32_t readableValue)
{
    return (readableValue / 2) + SOUNDBANK_STARTADRESS;
}


int DuInstrumentInfo::size() const
{
    return INSTR_INFO_SIZE;
}


DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, Name,               String, QString, QString())

DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, DreamProgramChange, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, MidiControlChange0, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, KeyMapping,         Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, Octave,             Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, UserID,             Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, ID,                 Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, ActiveNoteOff,      Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, Category,           String, QString, QString())

DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, RelativeVolume,     Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, DreamFormatId,      Numeric, DuInstrumentInfo::DreamFormat, NO_FORMAT)

DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, InstrType,          Numeric, INSTRUMENT_TYPE, NUM_INSTR_TYPE)
DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, InstrVersion,       Numeric, int, -1)
