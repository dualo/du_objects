#include "duinstrumentinfo.h"

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
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KeyID, new DuNumeric(0));

    addChild(KeyActiveNoteOff,
             new DuNumeric(0x01, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KeyCategory, new DuString(NAME_CARACT));

    addChild(KeyRelativeVolume,
             new DuNumeric(0x40, NUMERIC_DEFAULT_SIZE,
                           0xFF, 0x00));

    addChild(KeyType,
             new DuNumeric(INSTR_HARMONIC, NUMERIC_DEFAULT_SIZE,
                           NUM_INSTR_TYPE, INSTR_HARMONIC));

    addChild(KeyUserID, new DuString(16));
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
    verif = instrInfo->setID(du_instrInfo.instr_id) ? verif : false;

    verif = instrInfo->setActiveNoteOff(du_instrInfo.instr_noteoff) ? verif : false;
    verif = instrInfo->setCategory(QString(QByteArray((char *)du_instrInfo.instr_cat, NAME_CARACT))) ? verif : false;

    verif = instrInfo->setRelativeVolume(du_instrInfo.instr_relvolume) ? verif : false;

    verif = instrInfo->setType(du_instrInfo.instr_type) ? verif : false;

    //verif = instrInfo->setUserID(instrInfo.instr_userid) ? verif : false;
    //TODO: add userID when possible.

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuInstrumentInfo::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
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
    QJsonValue jsonType         = jsonInstrInfo[KeyType];
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

    verif = instrInfo->setType(jsonType.toInt()) ? verif : false;

    verif = instrInfo->setUserID(jsonUserId.toString()) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuInstrumentInfo::fromJson():\n"
                   << "an attribute was not properly set";
    }

    return instrInfo;
}


QByteArray DuInstrumentInfo::toDuMusicBinary() const
{
    s_instr du_instrumentinfo;

    QString tmpStr;
    int tmpNum = 0;

    QByteArray tmpClear(size(), (char)0x00);
    std::memcpy((char *)&(du_instrumentinfo), tmpClear.data(), size());

    tmpNum = getDreamProgramChange();
    if (tmpNum == -1)
        return QByteArray();
    du_instrumentinfo.instr_midi_pc = tmpNum;

    tmpNum = getMidiControlChange0();
    if (tmpNum == -1)
        return QByteArray();
    du_instrumentinfo.instr_midi_C0 = tmpNum;

    tmpNum = getKeyMapping();
    if (tmpNum == -1)
        return QByteArray();
    du_instrumentinfo.instr_key_map = tmpNum;

    tmpNum = getOctave();
    if (tmpNum == -1)
        return QByteArray();
    du_instrumentinfo.instr_octave = tmpNum;

    tmpNum = getID();
    if (tmpNum == -1)
        return QByteArray();
    du_instrumentinfo.instr_id = tmpNum;

    tmpNum = getActiveNoteOff();
    if (tmpNum == -1)
        return QByteArray();
    du_instrumentinfo.instr_noteoff = tmpNum;

    tmpNum = getRelativeVolume();
    if (tmpNum == -1)
        return QByteArray();
    du_instrumentinfo.instr_relvolume = tmpNum;

    tmpNum = getType();
    if (tmpNum == -1)
        return QByteArray();
    du_instrumentinfo.instr_type = tmpNum;


    QByteArray tmpName(NAME_CARACT, (char)0x00);
    tmpStr = getName();
    if (tmpStr.isNull())
        return QByteArray();
    tmpName.prepend(tmpStr.toUtf8());

    std::memcpy(du_instrumentinfo.instr_name, tmpName.data(), NAME_CARACT);

    QByteArray tmpCategory(NAME_CARACT, (char)0x00);
    tmpStr = getCategory();
    if (tmpStr.isNull())
        return QByteArray();
    tmpCategory.prepend(tmpStr.toUtf8());

    std::memcpy(du_instrumentinfo.instr_cat, tmpCategory.data(), NAME_CARACT);

/*
    QByteArray tmpUserID(NAME_CARACT, (char)0x00);
    tmpString = getUserID();
    if (tmpString.isNull())
        return QByteArray();
    tmpUserID.prepend(tmpString.toUtf8());

    std::memcpy(du_instrumentinfo.instr_userid, tmpUserID.data(), NAME_CARACT);
*/
    //TODO: add userID when possible.

    return QByteArray((char *)&(du_instrumentinfo), size());
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

DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, UserID,             String, QString, QString())
DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, ID,                 Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, ActiveNoteOff,      Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, Category,           String, QString, QString())

DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, RelativeVolume,     Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuInstrumentInfo, Type,               Numeric, int, -1)
