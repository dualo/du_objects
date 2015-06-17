#include "duinstrumentinfo.h"

#include <cstring>

#include <QJsonObject>

DU_OBJECT_IMPL(DuInstrumentInfo)

DuInstrumentInfo::DuInstrumentInfo() :
    DuContainer()
{
    addChild(KEY_INSTRINFO_CATEGORY, new DuString(NAME_CARACT));

    addChild(KEY_INSTRINFO_NAME, new DuString(NAME_CARACT));

    addChild(KEY_INSTRINFO_ID, new DuNumeric(0));

    addChild(KEY_INSTRINFO_USERID, new DuString(16));

    addChild(KEY_INSTRINFO_MIDIPROGRAMCHANGE,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KEY_INSTRINFO_MIDICONTROLCHANGE0,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KEY_INSTRINFO_ACTIVENOTEOFF,
             new DuNumeric(0x01, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KEY_INSTRINFO_RELVOLUME,
             new DuNumeric(0x40, NUMERIC_DEFAULT_SIZE,
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

    verif = verif && instrInfo->setCategory(
            QString(QByteArray((char *)du_instrInfo.instr_cat, NAME_CARACT)));
    verif = verif && instrInfo->setName(
            QString(QByteArray((char *)du_instrInfo.instr_name, NAME_CARACT)));
    verif = verif && instrInfo->setID(du_instrInfo.instr_id);
    //verif = verif && instrInfo->setUserID(instrInfo.instr_userid);
    //TODO: add userID when possible.

    verif = verif && instrInfo->setMidiProgramChange(du_instrInfo.instr_midi_pc);
    verif = verif && instrInfo->setMidiControlChange0(du_instrInfo.instr_midi_C0);

    verif = verif && instrInfo->setActiveNoteOff(du_instrInfo.instr_noteoff);
    verif = verif && instrInfo->setRelativeVolume(du_instrInfo.instr_relvolume);

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuInstrumentInfo::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    return instrInfo;
}


DuInstrumentInfoPtr DuInstrumentInfo::fromJson(const QJsonObject &jsonInstrInfo)
{
    QJsonValue jsonCategory     = jsonInstrInfo[KEY_INSTRINFO_CATEGORY];
    QJsonValue jsonName         = jsonInstrInfo[KEY_INSTRINFO_NAME];
    QJsonValue jsonId           = jsonInstrInfo[KEY_INSTRINFO_ID];
    QJsonValue jsonUserId       = jsonInstrInfo[KEY_INSTRINFO_USERID];
    QJsonValue jsonProgChange   = jsonInstrInfo[KEY_INSTRINFO_MIDIPROGRAMCHANGE];
    QJsonValue jsonCtrlChange   = jsonInstrInfo[KEY_INSTRINFO_MIDICONTROLCHANGE0];
    QJsonValue jsonNoteOff      = jsonInstrInfo[KEY_INSTRINFO_ACTIVENOTEOFF];
    QJsonValue jsonRelVolume    = jsonInstrInfo[KEY_INSTRINFO_RELVOLUME];

    if (        !jsonCategory.isString()    ||  !jsonName.isString()
            ||  !jsonId.isDouble()          ||  !jsonUserId.isString()
            ||  !jsonProgChange.isDouble()  ||  !jsonCtrlChange.isDouble()
            ||  !jsonNoteOff.isDouble()     ||  !jsonRelVolume.isDouble())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuInstrumentInfo::fromJson():\n"
                    << "failed to generate DuInstrumentInfo\n"
                    << "a json key did not contain the proper type";

        return DuInstrumentInfoPtr();
    }


    DuInstrumentInfoPtr instrInfo(new DuInstrumentInfo);
    bool verif = true;

    verif = verif && instrInfo->setCategory(jsonCategory.toString());
    verif = verif && instrInfo->setName(jsonName.toString());
    verif = verif && instrInfo->setID(jsonId.toInt());
    verif = verif && instrInfo->setUserID(jsonUserId.toString());

    verif = verif && instrInfo->setMidiProgramChange(jsonProgChange.toInt());
    verif = verif && instrInfo->setMidiControlChange0(jsonCtrlChange.toInt());
    verif = verif && instrInfo->setActiveNoteOff(jsonNoteOff.toInt());
    verif = verif && instrInfo->setRelativeVolume(jsonRelVolume.toInt());

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

    tmpNum = getMidiProgramChange();
    if (tmpNum == -1)
        return QByteArray();
    du_instrumentinfo.instr_midi_pc = tmpNum;

    tmpNum = getMidiControlChange0();
    if (tmpNum == -1)
        return QByteArray();
    du_instrumentinfo.instr_midi_C0 = tmpNum;

    tmpNum = getActiveNoteOff();
    if (tmpNum == -1)
        return QByteArray();
    du_instrumentinfo.instr_noteoff = tmpNum;

    tmpNum = getRelativeVolume();
    if (tmpNum == -1)
        return QByteArray();
    du_instrumentinfo.instr_relvolume = tmpNum;

    tmpNum = getID();
    if (tmpNum == -1)
        return QByteArray();
    du_instrumentinfo.instr_id = tmpNum;


    QByteArray tmpCategory(NAME_CARACT, (char)0x00);
    tmpStr = getCategory();
    if (tmpStr.isNull())
        return QByteArray();
    tmpCategory.prepend(tmpStr.toUtf8());

    std::memcpy(du_instrumentinfo.instr_cat, tmpCategory.data(), NAME_CARACT);

    QByteArray tmpName(NAME_CARACT, (char)0x00);
    tmpStr = getName();
    if (tmpStr.isNull())
        return QByteArray();
    tmpName.prepend(tmpStr.toUtf8());

    std::memcpy(du_instrumentinfo.instr_name, tmpName.data(), NAME_CARACT);

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


QString DuInstrumentInfo::getCategory() const
{
    const DuStringConstPtr &tmp =
            getChildAs<DuString>(KEY_INSTRINFO_CATEGORY);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuInstrumentInfo::setCategory(const QString &value)
{
    const DuStringPtr &tmp =
            getChildAs<DuString>(KEY_INSTRINFO_CATEGORY);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuInstrumentInfo::getName() const
{
    const DuStringConstPtr &tmp =
            getChildAs<DuString>(KEY_INSTRINFO_NAME);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuInstrumentInfo::setName(const QString &value)
{
    const DuStringPtr &tmp =
            getChildAs<DuString>(KEY_INSTRINFO_NAME);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

int DuInstrumentInfo::getID() const
{
    const DuNumericConstPtr &tmp =
            getChildAs<DuNumeric>(KEY_INSTRINFO_ID);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuInstrumentInfo::setID(int value)
{
    const DuNumericPtr &tmp =
            getChildAs<DuNumeric>(KEY_INSTRINFO_ID);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

QString DuInstrumentInfo::getUserID() const
{
    const DuStringConstPtr &tmp =
            getChildAs<DuString>(KEY_INSTRINFO_USERID);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuInstrumentInfo::setUserID(const QString &value)
{
    const DuStringPtr &tmp =
            getChildAs<DuString>(KEY_INSTRINFO_USERID);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}


int DuInstrumentInfo::getMidiProgramChange() const
{
    const DuNumericConstPtr &tmp =
            getChildAs<DuNumeric>(KEY_INSTRINFO_MIDIPROGRAMCHANGE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuInstrumentInfo::setMidiProgramChange(int value)
{
    const DuNumericPtr &tmp =
            getChildAs<DuNumeric>(KEY_INSTRINFO_MIDIPROGRAMCHANGE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuInstrumentInfo::getMidiControlChange0() const
{
    const DuNumericConstPtr &tmp =
            getChildAs<DuNumeric>(KEY_INSTRINFO_MIDICONTROLCHANGE0);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuInstrumentInfo::setMidiControlChange0(int value)
{
    const DuNumericPtr &tmp =
            getChildAs<DuNumeric>(KEY_INSTRINFO_MIDICONTROLCHANGE0);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuInstrumentInfo::getActiveNoteOff() const
{
    const DuNumericConstPtr &tmp =
            getChildAs<DuNumeric>(KEY_INSTRINFO_ACTIVENOTEOFF);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuInstrumentInfo::setActiveNoteOff(int value)
{
    const DuNumericPtr &tmp =
            getChildAs<DuNumeric>(KEY_INSTRINFO_ACTIVENOTEOFF);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuInstrumentInfo::getRelativeVolume() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_INSTRINFO_RELVOLUME);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuInstrumentInfo::setRelativeVolume(int value)
{
    const DuNumericPtr &tmp =
            getChildAs<DuNumeric>(KEY_INSTRINFO_RELVOLUME);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}
