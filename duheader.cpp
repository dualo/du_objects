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
    DuHeader *header = new DuHeader();

    header->setFileVersion(du_song.s_version_music);

    header->setOriginalSerialNumber(
            QString(QByteArray((char *)du_song.s_original_sn, HEADER_NAME_SIZE)));
    header->setOriginalName(
            QString(QByteArray((char *)du_song.s_original_name, HEADER_NAME_SIZE)));
    header->setOriginalUser(
            QString(QByteArray((char *)du_song.s_original_user, HEADER_NAME_SIZE)));
    header->setOriginalUserId(
            QString(QByteArray((char *)du_song.s_original_userid, HEADER_NAME_SIZE)));

    header->setLastModifSerialNumber(
            QString(QByteArray((char *)du_song.s_modif_sn, HEADER_NAME_SIZE)));
    header->setLastModifName(
            QString(QByteArray((char *)du_song.s_modif_name, HEADER_NAME_SIZE)));
    header->setLastModifUser(
            QString(QByteArray((char *)du_song.s_modif_user, HEADER_NAME_SIZE)));
    header->setLastModifUserId(
            QString(QByteArray((char *)du_song.s_modif_userid, HEADER_NAME_SIZE)));

    header->setSongId(du_song.s_id);
    header->setSongName(
            QString(QByteArray((char *)du_song.s_name, MUSIC_SONG_NAME_SIZE)));
    header->setSongVersion(du_song.s_version_song & 0x7FFFFFFF);

    return header;
}


DuHeader *DuHeader::fromJson(const QJsonObject &jsonHeader)
{
    DuHeader *header = new DuHeader();
    QStringList &keyList = header->keys();

    bool test = true;
    for (int i = 0; i < keyList.count(); i++)
    {
        test = test && jsonHeader.contains(keyList[i]);
    }

    if (!test)
        return NULL;

    header->setFileVersion(jsonHeader[KEY_HEAD_FILEVERSION].toInt());

    header->setOriginalSerialNumber(jsonHeader[KEY_HEAD_ORIGINALSN].toString());
    header->setOriginalName(jsonHeader[KEY_HEAD_ORIGINALNAME].toString());
    header->setOriginalUser(jsonHeader[KEY_HEAD_ORIGINALUSER].toString());
    header->setOriginalUserId(jsonHeader[KEY_HEAD_ORIGINALUSERID].toString());

    header->setLastModifSerialNumber(jsonHeader[KEY_HEAD_LASTMODIFSN].toString());
    header->setLastModifName(jsonHeader[KEY_HEAD_LASTMODIFNAME].toString());
    header->setLastModifUser(jsonHeader[KEY_HEAD_LASTMODIFUSER].toString());
    header->setLastModifUserId(jsonHeader[KEY_HEAD_LASTMODIFUSERID].toString());

    header->setSongId(jsonHeader[KEY_HEAD_SONGID].toInt());
    header->setSongName(jsonHeader[KEY_HEAD_SONGNAME].toString());
    header->setSongVersion(jsonHeader[KEY_HEAD_SONGVERSION].toInt());

    return header;
}


int DuHeader::getFileVersion() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_HEAD_FILEVERSION));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuHeader::setFileVersion(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_HEAD_FILEVERSION));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


QString DuHeader::getOriginalSerialNumber() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_ORIGINALSN));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

void DuHeader::setOriginalSerialNumber(const QString value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_ORIGINALSN));

    if (tmp == NULL)
        return;

    tmp->setString(value);
}

QString DuHeader::getOriginalName() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_ORIGINALNAME));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

void DuHeader::setOriginalName(const QString value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_ORIGINALNAME));

    if (tmp == NULL)
        return;

    tmp->setString(value);
}

QString DuHeader::getOriginalUser() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_ORIGINALUSER));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

void DuHeader::setOriginalUser(const QString value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_ORIGINALUSER));

    if (tmp == NULL)
        return;

    tmp->setString(value);
}

QString DuHeader::getOriginalUserId() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_ORIGINALUSERID));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

void DuHeader::setOriginalUserId(const QString value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_ORIGINALUSERID));

    if (tmp == NULL)
        return;

    tmp->setString(value);
}


QString DuHeader::getLastModifSerialNumber() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_LASTMODIFSN));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

void DuHeader::setLastModifSerialNumber(const QString value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_LASTMODIFSN));

    if (tmp == NULL)
        return;

    tmp->setString(value);
}

QString DuHeader::getLastModifName() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_LASTMODIFNAME));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

void DuHeader::setLastModifName(const QString value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_LASTMODIFNAME));

    if (tmp == NULL)
        return;

    tmp->setString(value);
}

QString DuHeader::getLastModifUser() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_LASTMODIFUSER));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

void DuHeader::setLastModifUser(const QString value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_LASTMODIFUSER));

    if (tmp == NULL)
        return;

    tmp->setString(value);
}

QString DuHeader::getLastModifUserId() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_LASTMODIFUSERID));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

void DuHeader::setLastModifUserId(const QString value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_LASTMODIFUSERID));

    if (tmp == NULL)
        return;

    tmp->setString(value);
}


int DuHeader::getSongId() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_HEAD_SONGID));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuHeader::setSongId(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_HEAD_SONGID));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

QString DuHeader::getSongName() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_SONGNAME));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

void DuHeader::setSongName(const QString value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_HEAD_SONGNAME));

    if (tmp == NULL)
        return;

    tmp->setString(value);
}

int DuHeader::getSongVersion() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_HEAD_SONGVERSION));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuHeader::setSongVersion(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_HEAD_SONGVERSION));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}
