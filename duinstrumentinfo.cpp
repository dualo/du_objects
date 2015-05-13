#include "duinstrumentinfo.h"

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
                           0x7F, 0x00));
}

DuInstrumentInfo::~DuInstrumentInfo()
{
}


DuInstrumentInfo *DuInstrumentInfo::fromDuMusicFile(const s_instr &du_instrInfo)
{
    DuInstrumentInfo *instrInfo = new DuInstrumentInfo;
    bool verif = true;

    verif = verif && instrInfo->setCategory(
            QString(QByteArray((char *)du_instrInfo.instr_cat, NAME_CARACT)));
    verif = verif && instrInfo->setName(
            QString(QByteArray((char *)du_instrInfo.instr_name, NAME_CARACT)));
    verif = verif && instrInfo->setID(du_instrInfo.instr_id);
    //verif = verif && instrInfo->setUserID(instrInfo.instr_userid);

    verif = verif && instrInfo->setMidiProgramChange(du_instrInfo.instr_midi_pc);
    verif = verif && instrInfo->setMidiControlChange0(du_instrInfo.instr_midi_C0);

    verif = verif && instrInfo->setActiveNoteOff(du_instrInfo.instr_noteoff);
    verif = verif && instrInfo->setRelativeVolume(du_instrInfo.instr_relvolume);

    if (!verif)
    {
        delete instrInfo;
        return NULL;
    }

    return instrInfo;
}


DuInstrumentInfo *DuInstrumentInfo::fromJson(const QJsonObject &jsonInstrInfo)
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

        return NULL;


    DuInstrumentInfo *instrInfo = new DuInstrumentInfo;
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
        delete instrInfo;
        return NULL;
    }

    return instrInfo;
}


QString DuInstrumentInfo::getCategory() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_INSTRINFO_CATEGORY));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

bool DuInstrumentInfo::setCategory(const QString &value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_INSTRINFO_CATEGORY));

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuInstrumentInfo::getName() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_INSTRINFO_NAME));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

bool DuInstrumentInfo::setName(const QString &value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_INSTRINFO_NAME));

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

int DuInstrumentInfo::getID() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_INSTRINFO_ID));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuInstrumentInfo::setID(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_INSTRINFO_ID));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

QString DuInstrumentInfo::getUserID() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_INSTRINFO_USERID));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

bool DuInstrumentInfo::setUserID(const QString &value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_INSTRINFO_USERID));

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}


int DuInstrumentInfo::getMidiProgramChange() const
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_INSTRINFO_MIDIPROGRAMCHANGE));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuInstrumentInfo::setMidiProgramChange(int value)
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_INSTRINFO_MIDIPROGRAMCHANGE));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuInstrumentInfo::getMidiControlChange0() const
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_INSTRINFO_MIDICONTROLCHANGE0));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuInstrumentInfo::setMidiControlChange0(int value)
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_INSTRINFO_MIDICONTROLCHANGE0));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuInstrumentInfo::getActiveNoteOff() const
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_INSTRINFO_ACTIVENOTEOFF));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuInstrumentInfo::setActiveNoteOff(int value)
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_INSTRINFO_ACTIVENOTEOFF));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuInstrumentInfo::getRelativeVolume() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_INSTRINFO_RELVOLUME));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuInstrumentInfo::setRelativeVolume(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_INSTRINFO_RELVOLUME));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}
