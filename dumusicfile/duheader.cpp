#include "duheader.h"

#include <cstring>

#include <QJsonObject>


DU_OBJECT_IMPL(DuHeader)

DuHeader::DuHeader() :
    DuContainer()
{
    addChild(KEY_HEAD_FILEVERSION, new DuNumeric(2));

    addChild(KEY_HEAD_ORIGINALSN, new DuString(HEADER_NAME_SIZE));
    addChild(KEY_HEAD_ORIGINALNAME, new DuString(HEADER_NAME_SIZE));
    addChild(KEY_HEAD_ORIGINALUSER, new DuString(HEADER_NAME_SIZE));
    addChild(KEY_HEAD_ORIGINALUSERID, new DuString(HEADER_NAME_SIZE));

    addChild(KEY_HEAD_LASTMODIFSN, new DuString(HEADER_NAME_SIZE));
    addChild(KEY_HEAD_LASTMODIFNAME, new DuString(HEADER_NAME_SIZE));
    addChild(KEY_HEAD_LASTMODIFUSER, new DuString(HEADER_NAME_SIZE));
    addChild(KEY_HEAD_LASTMODIFUSERID, new DuString(HEADER_NAME_SIZE));

    addChild(KEY_HEAD_SIZE, new DuNumeric(0));
    addChild(KEY_HEAD_METADATA, new DuNumeric(0));

    addChild(KEY_HEAD_TRANSPOSE,
             new DuNumeric(12, NUMERIC_DEFAULT_SIZE,
                           23, 0));
}

DuHeader::~DuHeader()
{
}

DuObjectPtr DuHeader::clone() const
{
    return DuHeaderPtr(new DuHeader(*this));
}


DuHeaderPtr DuHeader::fromDuMusicBinary(const music_song &du_song)
{
    DuHeaderPtr header(new DuHeader);
    bool verif = true;

    verif = header->setFileVersion(du_song.s_version_music) ? verif : false;

    verif = header->setOriginalSerialNumber(QString(QByteArray((char *)du_song.s_original_sn, HEADER_NAME_SIZE))) ? verif : false;
    verif = header->setOriginalName(QString(QByteArray((char *)du_song.s_original_name, HEADER_NAME_SIZE))) ? verif : false;
    verif = header->setOriginalUser(QString(QByteArray((char *)du_song.s_original_user, HEADER_NAME_SIZE))) ? verif : false;
    verif = header->setOriginalUserId(QString(QByteArray((char *)du_song.s_original_userid, HEADER_NAME_SIZE))) ? verif : false;

    verif = header->setLastModifSerialNumber(QString(QByteArray((char *)du_song.s_modif_sn, HEADER_NAME_SIZE))) ? verif : false;
    verif = header->setLastModifName(QString(QByteArray((char *)du_song.s_modif_name, HEADER_NAME_SIZE))) ? verif : false;
    verif = header->setLastModifUser(QString(QByteArray((char *)du_song.s_modif_user, HEADER_NAME_SIZE))) ? verif : false;
    verif = header->setLastModifUserId(QString(QByteArray((char *)du_song.s_modif_userid, HEADER_NAME_SIZE))) ? verif : false;

    verif = header->setSize(du_song.s_size) ? verif : false;
    verif = header->setMetaData(du_song.s_metadata) ? verif : false;

    verif = header->setTranspose(du_song.s_transpose) ? verif : false;
    
    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuHeader::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
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
    QJsonValue jsonSize             = jsonHeader[KEY_HEAD_SIZE];
    QJsonValue jsonMetaData         = jsonHeader[KEY_HEAD_METADATA];
    QJsonValue jsonTranspose        = jsonHeader[KEY_HEAD_TRANSPOSE];

    if (        !jsonFileVersion.isDouble() ||  !jsonOrigSerialNum.isString()
            ||  !jsonOrigName.isString()    ||  !jsonOrigUser.isString()
            ||  !jsonOrigUserId.isString()  ||  !jsonLastSerialNum.isString()
            ||  !jsonLastName.isString()    ||  !jsonLastUser.isString()
            ||  !jsonLastUserId.isString()
            ||  !jsonSize.isDouble()        ||  !jsonMetaData.isDouble()
            ||  !jsonTranspose.isDouble())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuHeader::fromJson():\n"
                    << "failed to generate DuHeader\n"
                    << "a json key did not contain the proper type";

        return DuHeaderPtr();
    }


    DuHeaderPtr header(new DuHeader);
    bool verif = true;

    verif = header->setFileVersion(jsonFileVersion.toInt()) ? verif : false;

    verif = header->setOriginalSerialNumber(jsonOrigSerialNum.toString()) ? verif : false;
    verif = header->setOriginalName(jsonOrigName.toString()) ? verif : false;
    verif = header->setOriginalUser(jsonOrigUser.toString()) ? verif : false;
    verif = header->setOriginalUserId(jsonOrigUserId.toString()) ? verif : false;

    verif = header->setLastModifSerialNumber(jsonLastSerialNum.toString()) ? verif : false;
    verif = header->setLastModifName(jsonLastName.toString()) ? verif : false;
    verif = header->setLastModifUser(jsonLastUser.toString()) ? verif : false;
    verif = header->setLastModifUserId(jsonLastUserId.toString()) ? verif : false;

    verif = header->setSize(jsonSize.toInt()) ? verif : false;
    verif = header->setMetaData(jsonMetaData.toInt()) ? verif : false;

    verif = header->setTranspose(jsonTranspose.toInt()) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuHeader::fromJson():\n"
                   << "an attribute was not properly set";
    }

    return header;
}


QByteArray DuHeader::toDuMusicBinary() const
{
    //NOTE: optimization possible if music_song matched du-objects
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


    tmpNum = getSize();
    if (tmpNum == -1)
        return QByteArray();
    du_header.s_size = tmpNum;

    tmpNum = getMetaData();
    if (tmpNum == -1)
        return QByteArray();
    du_header.s_metadata = tmpNum;

    tmpNum = getTranspose();
    if (tmpNum == -1)
        return QByteArray();
    du_header.s_transpose = tmpNum;


    return QByteArray((char *)&(du_header), size());
}


int DuHeader::size() const
{
    return HEADER_SIZE;
}


int DuHeader::getFileVersion() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_HEAD_FILEVERSION);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuHeader::setFileVersion(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_HEAD_FILEVERSION);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


QString DuHeader::getOriginalSerialNumber() const
{
    const DuStringConstPtr &tmp = getChildAs<DuString>(KEY_HEAD_ORIGINALSN);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuHeader::setOriginalSerialNumber(const QString value)
{
    const DuStringPtr &tmp = getChildAs<DuString>(KEY_HEAD_ORIGINALSN);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuHeader::getOriginalName() const
{
    const DuStringConstPtr &tmp = getChildAs<DuString>(KEY_HEAD_ORIGINALNAME);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuHeader::setOriginalName(const QString value)
{
    const DuStringPtr &tmp = getChildAs<DuString>(KEY_HEAD_ORIGINALNAME);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuHeader::getOriginalUser() const
{
    const DuStringConstPtr &tmp = getChildAs<DuString>(KEY_HEAD_ORIGINALUSER);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuHeader::setOriginalUser(const QString value)
{
    const DuStringPtr &tmp = getChildAs<DuString>(KEY_HEAD_ORIGINALUSER);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuHeader::getOriginalUserId() const
{
    const DuStringConstPtr &tmp = getChildAs<DuString>(KEY_HEAD_ORIGINALUSERID);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuHeader::setOriginalUserId(const QString value)
{
    const DuStringPtr &tmp = getChildAs<DuString>(KEY_HEAD_ORIGINALUSERID);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}


QString DuHeader::getLastModifSerialNumber() const
{
    const DuStringConstPtr &tmp = getChildAs<DuString>(KEY_HEAD_LASTMODIFSN);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuHeader::setLastModifSerialNumber(const QString value)
{
    const DuStringPtr &tmp = getChildAs<DuString>(KEY_HEAD_LASTMODIFSN);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuHeader::getLastModifName() const
{
    const DuStringConstPtr &tmp = getChildAs<DuString>(KEY_HEAD_LASTMODIFNAME);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuHeader::setLastModifName(const QString value)
{
    const DuStringPtr &tmp = getChildAs<DuString>(KEY_HEAD_LASTMODIFNAME);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuHeader::getLastModifUser() const
{
    const DuStringConstPtr &tmp = getChildAs<DuString>(KEY_HEAD_LASTMODIFUSER);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuHeader::setLastModifUser(const QString value)
{
    const DuStringPtr &tmp = getChildAs<DuString>(KEY_HEAD_LASTMODIFUSER);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuHeader::getLastModifUserId() const
{
    const DuStringConstPtr &tmp = getChildAs<DuString>(KEY_HEAD_LASTMODIFUSERID);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuHeader::setLastModifUserId(const QString value)
{
    const DuStringPtr &tmp = getChildAs<DuString>(KEY_HEAD_LASTMODIFUSERID);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}


int DuHeader::getSize() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_HEAD_SIZE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuHeader::setSize(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_HEAD_SIZE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuHeader::getMetaData() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_HEAD_METADATA);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuHeader::setMetaData(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_HEAD_METADATA);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuHeader::getTranspose() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_HEAD_TRANSPOSE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuHeader::setTranspose(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_HEAD_TRANSPOSE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}
