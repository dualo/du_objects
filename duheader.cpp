#include "duheader.h"

DuHeader::DuHeader() :
    DuContainer()
{
    addChild(KEY_HEAD_FILEVERSION, new DuNumeric(1));

    addChild(KEY_HEAD_ORIGINALSN, new DuString(HEADER_NAME_SIZE));
    addChild(KEY_HEAD_ORIGINALNAME, new DuString(HEADER_NAME_SIZE));
    addChild(KEY_HEAD_ORIGINALUSER, new DuString(HEADER_NAME_SIZE));
    addChild(KEY_HEAD_ORIGINALUSERID, new DuString(HEADER_NAME_SIZE));

    addChild(KEY_HEAD_LASTMODIFSN, new DuString(HEADER_NAME_SIZE));
    addChild(KEY_HEAD_LASTMODIFNAME, new DuString(HEADER_NAME_SIZE));
    addChild(KEY_HEAD_LASTMODIFUSER, new DuString(HEADER_NAME_SIZE));
    addChild(KEY_HEAD_LASTMODIFUSERID, new DuString(HEADER_NAME_SIZE));

    addChild(KEY_HEAD_SONGID, new DuNumeric(0));
    addChild(KEY_HEAD_SONGNAME, new DuString(MUSIC_SONG_NAME_SIZE));
    addChild(KEY_HEAD_SONGVERSION, new DuNumeric(0));
}

DuHeader::~DuHeader()
{
}


DuHeader *DuHeader::fromDuMusicFile(const music_song &du_song)
{
    DuHeader *header = new DuHeader;
    bool verif = true;

    verif = verif && header->setFileVersion(du_song.s_version_music);

    verif = verif && header->setOriginalSerialNumber(
            QString(QByteArray((char *)du_song.s_original_sn, HEADER_NAME_SIZE)));
    verif = verif && header->setOriginalName(
            QString(QByteArray((char *)du_song.s_original_name, HEADER_NAME_SIZE)));
    verif = verif && header->setOriginalUser(
            QString(QByteArray((char *)du_song.s_original_user, HEADER_NAME_SIZE)));
    verif = verif && header->setOriginalUserId(
            QString(QByteArray((char *)du_song.s_original_userid, HEADER_NAME_SIZE)));

    verif = verif && header->setLastModifSerialNumber(
            QString(QByteArray((char *)du_song.s_modif_sn, HEADER_NAME_SIZE)));
    verif = verif && header->setLastModifName(
            QString(QByteArray((char *)du_song.s_modif_name, HEADER_NAME_SIZE)));
    verif = verif && header->setLastModifUser(
            QString(QByteArray((char *)du_song.s_modif_user, HEADER_NAME_SIZE)));
    verif = verif && header->setLastModifUserId(
            QString(QByteArray((char *)du_song.s_modif_userid, HEADER_NAME_SIZE)));

    verif = verif && header->setSongId(du_song.s_id);
    verif = verif && header->setSongName(
            QString(QByteArray((char *)du_song.s_name, MUSIC_SONG_NAME_SIZE)));
    verif = verif && header->setSongVersion(du_song.s_version_song & 0x7FFFFFFF);

    if (!verif)
    {
        delete header;
        return NULL;
    }

    return header;
}


DuHeader *DuHeader::fromJson(const QJsonObject &jsonHeader)
{
    QJsonValue jsonFileVersion      = jsonHeader[KEY_HEAD_FILEVERSION];
    QJsonValue jsonOrigSerialNum    = jsonHeader[KEY_HEAD_ORIGINALSN];
    QJsonValue jsonOrigName         = jsonHeader[KEY_HEAD_ORIGINALNAME];
    QJsonValue jsonOrigUser         = jsonHeader[KEY_HEAD_ORIGINALUSER];
    QJsonValue jsonOrigUserId       = jsonHeader[KEY_HEAD_ORIGINALUSERID];
    QJsonValue jsonLastSerialNum    = jsonHeader[KEY_HEAD_LASTMODIFSN];
    QJsonValue jsonLastName         = jsonHeader[KEY_HEAD_LASTMODIFNAME];
    QJsonValue jsonLastUser         = jsonHeader[KEY_HEAD_LASTMODIFUSER];
    QJsonValue jsonLastUserId       = jsonHeader[KEY_HEAD_LASTMODIFUSERID];
    QJsonValue jsonSongId           = jsonHeader[KEY_HEAD_SONGID];
    QJsonValue jsonSongName         = jsonHeader[KEY_HEAD_SONGNAME];
    QJsonValue jsonSongVersion      = jsonHeader[KEY_HEAD_SONGVERSION];

    if (        !jsonFileVersion.isDouble() ||  !jsonOrigSerialNum.isString()
            ||  !jsonOrigName.isString()    ||  !jsonOrigUser.isString()
            ||  !jsonOrigUserId.isString()  ||  !jsonLastSerialNum.isString()
            ||  !jsonLastName.isString()    ||  !jsonLastUser.isString()
            ||  !jsonLastUserId.isString()  ||  !jsonSongId.isDouble()
            ||  !jsonSongName.isString()    ||  !jsonSongVersion.isDouble())

        return NULL;


    DuHeader *header = new DuHeader;
    bool verif = true;

    verif = verif && header->setFileVersion(jsonFileVersion.toInt());

    verif = verif && header->setOriginalSerialNumber(jsonOrigSerialNum.toString());
    verif = verif && header->setOriginalName(jsonOrigName.toString());
    verif = verif && header->setOriginalUser(jsonOrigUser.toString());
    verif = verif && header->setOriginalUserId(jsonOrigUserId.toString());

    verif = verif && header->setLastModifSerialNumber(jsonLastSerialNum.toString());
    verif = verif && header->setLastModifName(jsonLastName.toString());
    verif = verif && header->setLastModifUser(jsonLastUser.toString());
    verif = verif && header->setLastModifUserId(jsonLastUserId.toString());

    verif = verif && header->setSongId(jsonSongId.toInt());
    verif = verif && header->setSongName(jsonSongName.toString());
    verif = verif && header->setSongVersion(jsonSongVersion.toInt());

    if (!verif)
    {
        delete header;
        return NULL;
    }

    return header;
}


int DuHeader::size() const
{
    //TODO: add defines for dummy sizes in music_parameters_mng.h
    return HEADER_SIZE;
}


int DuHeader::getFileVersion() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_HEAD_FILEVERSION));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuHeader::setFileVersion(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_HEAD_FILEVERSION));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


QString DuHeader::getOriginalSerialNumber() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_ORIGINALSN));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

bool DuHeader::setOriginalSerialNumber(const QString value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_ORIGINALSN));

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuHeader::getOriginalName() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_ORIGINALNAME));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

bool DuHeader::setOriginalName(const QString value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_ORIGINALNAME));

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuHeader::getOriginalUser() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_ORIGINALUSER));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

bool DuHeader::setOriginalUser(const QString value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_ORIGINALUSER));

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuHeader::getOriginalUserId() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_ORIGINALUSERID));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

bool DuHeader::setOriginalUserId(const QString value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_ORIGINALUSERID));

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}


QString DuHeader::getLastModifSerialNumber() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_LASTMODIFSN));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

bool DuHeader::setLastModifSerialNumber(const QString value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_LASTMODIFSN));

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuHeader::getLastModifName() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_LASTMODIFNAME));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

bool DuHeader::setLastModifName(const QString value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_LASTMODIFNAME));

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuHeader::getLastModifUser() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_LASTMODIFUSER));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

bool DuHeader::setLastModifUser(const QString value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_LASTMODIFUSER));

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuHeader::getLastModifUserId() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_LASTMODIFUSERID));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

bool DuHeader::setLastModifUserId(const QString value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_LASTMODIFUSERID));

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}


int DuHeader::getSongId() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_HEAD_SONGID));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuHeader::setSongId(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_HEAD_SONGID));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

QString DuHeader::getSongName() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_SONGNAME));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

bool DuHeader::setSongName(const QString value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_SONGNAME));

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

int DuHeader::getSongVersion() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_HEAD_SONGVERSION));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuHeader::setSongVersion(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_HEAD_SONGVERSION));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}
