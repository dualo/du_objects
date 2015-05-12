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
    DuInstrumentInfo *instrInfo = new DuInstrumentInfo();

    instrInfo->setCategory(
            QString(QByteArray((char *)du_instrInfo.instr_cat, NAME_CARACT)));
    instrInfo->setName(
            QString(QByteArray((char *)du_instrInfo.instr_name, NAME_CARACT)));
    instrInfo->setID(du_instrInfo.instr_id);
    //instrInfo->setUserID(instrInfo.instr_userid);

    instrInfo->setMidiProgramChange(du_instrInfo.instr_midi_pc);
    instrInfo->setMidiControlChange0(du_instrInfo.instr_midi_C0);

    instrInfo->setActiveNoteOff(du_instrInfo.instr_noteoff);
    instrInfo->setRelativeVolume(du_instrInfo.instr_relvolume);

    return instrInfo;
}


DuInstrumentInfo *DuInstrumentInfo::fromJson(const QJsonObject &jsonInstrInfo)
{
    DuInstrumentInfo *instrInfo = new DuInstrumentInfo();
    const QStringList &keyList = instrInfo->keys();

    bool test = true;
    for (int i = 0; i < keyList.count(); i++)
    {
        test = test && jsonInstrInfo.contains(keyList[i]);
    }

    if (!test)
        return NULL;

    instrInfo->setCategory(jsonInstrInfo[KEY_INSTRINFO_CATEGORY].toString());
    instrInfo->setName(jsonInstrInfo[KEY_INSTRINFO_NAME].toString());
    instrInfo->setID(jsonInstrInfo[KEY_INSTRINFO_ID].toInt());
    instrInfo->setUserID(jsonInstrInfo[KEY_INSTRINFO_USERID].toString());

    instrInfo->setMidiProgramChange(
                jsonInstrInfo[KEY_INSTRINFO_MIDIPROGRAMCHANGE].toInt());
    instrInfo->setMidiControlChange0(
                jsonInstrInfo[KEY_INSTRINFO_MIDICONTROLCHANGE0].toInt());
    instrInfo->setActiveNoteOff(jsonInstrInfo[KEY_INSTRINFO_ACTIVENOTEOFF].toInt());
    instrInfo->setRelativeVolume(jsonInstrInfo[KEY_INSTRINFO_RELVOLUME].toInt());

    return instrInfo;
}


QString DuInstrumentInfo::getCategory() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_INSTRINFO_CATEGORY));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

void DuInstrumentInfo::setCategory(const QString &value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_INSTRINFO_CATEGORY));

    if (tmp == NULL)
        return;

    tmp->setString(value);
}

QString DuInstrumentInfo::getName() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_INSTRINFO_NAME));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

void DuInstrumentInfo::setName(const QString &value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_INSTRINFO_NAME));

    if (tmp == NULL)
        return;

    tmp->setString(value);
}

int DuInstrumentInfo::getID() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_INSTRINFO_ID));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuInstrumentInfo::setID(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_INSTRINFO_ID));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

QString DuInstrumentInfo::getUserID() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_INSTRINFO_USERID));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

void DuInstrumentInfo::setUserID(const QString &value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_INSTRINFO_USERID));

    if (tmp == NULL)
        return;

    tmp->setString(value);
}


int DuInstrumentInfo::getMidiProgramChange() const
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_INSTRINFO_MIDIPROGRAMCHANGE));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuInstrumentInfo::setMidiProgramChange(int value)
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_INSTRINFO_MIDIPROGRAMCHANGE));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuInstrumentInfo::getMidiControlChange0() const
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_INSTRINFO_MIDICONTROLCHANGE0));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuInstrumentInfo::setMidiControlChange0(int value)
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_INSTRINFO_MIDICONTROLCHANGE0));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


int DuInstrumentInfo::getActiveNoteOff() const
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_INSTRINFO_ACTIVENOTEOFF));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuInstrumentInfo::setActiveNoteOff(int value)
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_INSTRINFO_ACTIVENOTEOFF));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuInstrumentInfo::getRelativeVolume() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_INSTRINFO_RELVOLUME));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuInstrumentInfo::setRelativeVolume(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_INSTRINFO_RELVOLUME));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}
