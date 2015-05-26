#include "duheader.h"

#include <QJsonObject>
#include <QDebug>

DU_OBJECT_IMPL(DuHeader)

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


DuHeaderPtr DuHeader::fromDuMusicFile(const music_song &du_song)
{
    DuHeaderPtr header(new DuHeader);
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

    verif = verif && header->setSongId(du_song.s_id & 0x7FFFFFFF);
    verif = verif && header->setSongName(
            QString(QByteArray((char *)du_song.s_name, MUSIC_SONG_NAME_SIZE)));
    verif = verif && header->setSongVersion(du_song.s_version_song & 0x7FFFFFFF);

    if (!verif)
    {
        return DuHeaderPtr();
    }

    return header;
}


DuHeaderPtr DuHeader::fromJson(const QJsonObject &jsonHeader)
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

        return DuHeaderPtr();


    DuHeaderPtr header(new DuHeader);
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
        return DuHeaderPtr();
    }

    return header;
}

QByteArray DuHeader::toDuMusicFile() const
{
    //TODO: restructure music_song into music_header and music_info
    music_song du_header;

    QByteArray tmpArray;
    QString tmpStr;
    int tmpNum = 0;

    QByteArray tmpClear(MUSIC_SONG_SIZE, (char)0x00);
    std::memcpy((char *)&(du_header), tmpClear.data(), MUSIC_SONG_SIZE);

    tmpNum = getFileVersion();
    if (tmpNum == -1)
        return QByteArray();
    du_header.s_version_music = tmpNum;

    tmpNum = getSongId();
    if (tmpNum == -1)
        return QByteArray();
    du_header.s_id = tmpNum;

    tmpNum = getSongVersion();
    if (tmpNum == -1)
        return QByteArray();
    du_header.s_version_song = tmpNum;


    tmpClear = QByteArray(HEADER_NAME_SIZE, char(0x00));

    tmpArray = tmpClear;
    tmpStr = getOriginalSerialNumber();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toUtf8());

    std::memcpy(du_header.s_original_sn, tmpArray.data(), HEADER_NAME_SIZE);

    tmpArray = tmpClear;
    tmpStr = getOriginalName();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toUtf8());

    std::memcpy(du_header.s_original_name, tmpArray.data(), HEADER_NAME_SIZE);

    tmpArray = tmpClear;
    tmpStr = getOriginalUser();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toUtf8());

    std::memcpy(du_header.s_original_user, tmpArray.data(), HEADER_NAME_SIZE);

    tmpArray = tmpClear;
    tmpStr = getOriginalUserId();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toUtf8());

    std::memcpy(du_header.s_original_userid, tmpArray.data(), HEADER_NAME_SIZE);

    tmpArray = tmpClear;
    tmpStr = getLastModifSerialNumber();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toUtf8());

    std::memcpy(du_header.s_modif_sn, tmpArray.data(), HEADER_NAME_SIZE);

    tmpArray = tmpClear;
    tmpStr = getLastModifName();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toUtf8());

    std::memcpy(du_header.s_modif_name, tmpArray.data(), HEADER_NAME_SIZE);

    tmpArray = tmpClear;
    tmpStr = getLastModifUser();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toUtf8());

    std::memcpy(du_header.s_modif_user, tmpArray.data(), HEADER_NAME_SIZE);

    tmpArray = tmpClear;
    tmpStr = getLastModifUserId();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toUtf8());

    std::memcpy(du_header.s_modif_userid, tmpArray.data(), HEADER_NAME_SIZE);

    tmpClear = QByteArray(MUSIC_SONG_NAME_SIZE, char(0x00));

    tmpArray = tmpClear;
    tmpStr = getSongName();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toUtf8());

    std::memcpy(du_header.s_name, tmpArray.data(), MUSIC_SONG_NAME_SIZE);


    return QByteArray((char *)&(du_header), MUSIC_SONG_SIZE);
}


int DuHeader::size() const
{
    //TODO: add defines for dummy sizes in music_parameters_mng.h
    return HEADER_SIZE;
}


int DuHeader::getFileVersion() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_HEAD_FILEVERSION);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuHeader::setFileVersion(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_HEAD_FILEVERSION);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


QString DuHeader::getOriginalSerialNumber() const
{
    const DuStringConstPtr& tmp = getChildAs<DuString>(KEY_HEAD_ORIGINALSN);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuHeader::setOriginalSerialNumber(const QString value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_HEAD_ORIGINALSN);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuHeader::getOriginalName() const
{
    const DuStringConstPtr& tmp = getChildAs<DuString>(KEY_HEAD_ORIGINALNAME);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuHeader::setOriginalName(const QString value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_HEAD_ORIGINALNAME);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuHeader::getOriginalUser() const
{
    const DuStringConstPtr& tmp = getChildAs<DuString>(KEY_HEAD_ORIGINALUSER);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuHeader::setOriginalUser(const QString value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_HEAD_ORIGINALUSER);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuHeader::getOriginalUserId() const
{
    const DuStringConstPtr& tmp = getChildAs<DuString>(KEY_HEAD_ORIGINALUSERID);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuHeader::setOriginalUserId(const QString value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_HEAD_ORIGINALUSERID);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}


QString DuHeader::getLastModifSerialNumber() const
{
    const DuStringConstPtr& tmp = getChildAs<DuString>(KEY_HEAD_LASTMODIFSN);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuHeader::setLastModifSerialNumber(const QString value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_HEAD_LASTMODIFSN);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuHeader::getLastModifName() const
{
    const DuStringConstPtr& tmp = getChildAs<DuString>(KEY_HEAD_LASTMODIFNAME);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuHeader::setLastModifName(const QString value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_HEAD_LASTMODIFNAME);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuHeader::getLastModifUser() const
{
    const DuStringConstPtr& tmp = getChildAs<DuString>(KEY_HEAD_LASTMODIFUSER);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuHeader::setLastModifUser(const QString value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_HEAD_LASTMODIFUSER);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuHeader::getLastModifUserId() const
{
    const DuStringConstPtr& tmp = getChildAs<DuString>(KEY_HEAD_LASTMODIFUSERID);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuHeader::setLastModifUserId(const QString value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_HEAD_LASTMODIFUSERID);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}


int DuHeader::getSongId() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_HEAD_SONGID);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuHeader::setSongId(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_HEAD_SONGID);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

QString DuHeader::getSongName() const
{
    const DuStringConstPtr& tmp = getChildAs<DuString>(KEY_HEAD_SONGNAME);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuHeader::setSongName(const QString value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_HEAD_SONGNAME);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

int DuHeader::getSongVersion() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_HEAD_SONGVERSION);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuHeader::setSongVersion(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_HEAD_SONGVERSION);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}
