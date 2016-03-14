#include "duheader.h"

#include <cstring>

#include <QJsonObject>


DU_OBJECT_IMPL(DuHeader)

DuHeader::DuHeader() :
    DuContainer()
{
    addChild(KeyFileVersion,
             new DuNumeric(2));

    addChild(KeyOriginalSerialNumber,
             new DuString(HEADER_NAME_SIZE));
    addChild(KeyOriginalName,
             new DuString(HEADER_NAME_SIZE));
    addChild(KeyOriginalUser,
             new DuString(HEADER_NAME_SIZE));
    addChild(KeyOriginalUserId,
             new DuString(HEADER_NAME_SIZE));

    addChild(KeyLastModifSerialNumber,
             new DuString(HEADER_NAME_SIZE));
    addChild(KeyLastModifName,
             new DuString(HEADER_NAME_SIZE));
    addChild(KeyLastModifUser,
             new DuString(HEADER_NAME_SIZE));
    addChild(KeyLastModifUserId,
             new DuString(HEADER_NAME_SIZE));

    addChild(KeySize,
             new DuNumeric(0));
    addChild(KeyMetaData,
             new DuNumeric(0));
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
    
    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuHeader::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    return header;
}


DuHeaderPtr DuHeader::fromJson(const QJsonObject &jsonHeader)
{
    QJsonValue jsonFileVersion      = jsonHeader[KeyFileVersion];
    QJsonValue jsonOrigSerialNum    = jsonHeader[KeyOriginalSerialNumber];
    QJsonValue jsonOrigName         = jsonHeader[KeyOriginalName];
    QJsonValue jsonOrigUser         = jsonHeader[KeyOriginalUser];
    QJsonValue jsonOrigUserId       = jsonHeader[KeyOriginalUserId];
    QJsonValue jsonLastSerialNum    = jsonHeader[KeyLastModifSerialNumber];
    QJsonValue jsonLastName         = jsonHeader[KeyLastModifName];
    QJsonValue jsonLastUser         = jsonHeader[KeyLastModifUser];
    QJsonValue jsonLastUserId       = jsonHeader[KeyLastModifUserId];
    QJsonValue jsonSize             = jsonHeader[KeySize];
    QJsonValue jsonMetaData         = jsonHeader[KeyMetaData];

    if (        !jsonFileVersion.isDouble() ||  !jsonOrigSerialNum.isString()
            ||  !jsonOrigName.isString()    ||  !jsonOrigUser.isString()
            ||  !jsonOrigUserId.isString()  ||  !jsonLastSerialNum.isString()
            ||  !jsonLastName.isString()    ||  !jsonLastUser.isString()
            ||  !jsonLastUserId.isString()
            ||  !jsonSize.isDouble()        ||  !jsonMetaData.isDouble())
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

    std::memset((char*)&du_header, 0, MUSIC_SONG_SIZE);

    tmpNum = getFileVersion();
    if (tmpNum == -1)
        return QByteArray();
    du_header.s_version_music = tmpNum;


    tmpArray.fill(0x00, HEADER_NAME_SIZE);
    tmpStr = getOriginalSerialNumber();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toUtf8());

    std::memcpy(du_header.s_original_sn, tmpArray.constData(), HEADER_NAME_SIZE);

    tmpArray.fill(0x00, HEADER_NAME_SIZE);
    tmpStr = getOriginalName();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toUtf8());

    std::memcpy(du_header.s_original_name, tmpArray.constData(), HEADER_NAME_SIZE);

    tmpArray.fill(0x00, HEADER_NAME_SIZE);
    tmpStr = getOriginalUser();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toUtf8());

    std::memcpy(du_header.s_original_user, tmpArray.constData(), HEADER_NAME_SIZE);

    tmpArray.fill(0x00, HEADER_NAME_SIZE);
    tmpStr = getOriginalUserId();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toUtf8());

    std::memcpy(du_header.s_original_userid, tmpArray.constData(), HEADER_NAME_SIZE);

    tmpArray.fill(0x00, HEADER_NAME_SIZE);
    tmpStr = getLastModifSerialNumber();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toUtf8());

    std::memcpy(du_header.s_modif_sn, tmpArray.constData(), HEADER_NAME_SIZE);

    tmpArray.fill(0x00, HEADER_NAME_SIZE);
    tmpStr = getLastModifName();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toUtf8());

    std::memcpy(du_header.s_modif_name, tmpArray.constData(), HEADER_NAME_SIZE);

    tmpArray.fill(0x00, HEADER_NAME_SIZE);
    tmpStr = getLastModifUser();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toUtf8());

    std::memcpy(du_header.s_modif_user, tmpArray.constData(), HEADER_NAME_SIZE);

    tmpArray.fill(0x00, HEADER_NAME_SIZE);
    tmpStr = getLastModifUserId();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toUtf8());

    std::memcpy(du_header.s_modif_userid, tmpArray.constData(), HEADER_NAME_SIZE);


    tmpNum = getSize();
    if (tmpNum == -1)
        return QByteArray();
    du_header.s_size = tmpNum;

    tmpNum = getMetaData();
    if (tmpNum == -1)
        return QByteArray();
    du_header.s_metadata = tmpNum;


    return QByteArray((char *)&(du_header), size());
}


int DuHeader::size() const
{
    return HEADER_SIZE;
}


DU_KEY_ACCESSORS_IMPL(DuHeader, FileVersion,    Numeric,    int,    -1)

DU_KEY_ACCESSORS_IMPL(DuHeader, OriginalSerialNumber,  String,  QString,  QString())
DU_KEY_ACCESSORS_IMPL(DuHeader, OriginalName,          String,  QString,  QString())
DU_KEY_ACCESSORS_IMPL(DuHeader, OriginalUser,          String,  QString,  QString())
DU_KEY_ACCESSORS_IMPL(DuHeader, OriginalUserId,        String,  QString,  QString())

DU_KEY_ACCESSORS_IMPL(DuHeader, LastModifSerialNumber, String,  QString,  QString())
DU_KEY_ACCESSORS_IMPL(DuHeader, LastModifName,         String,  QString,  QString())
DU_KEY_ACCESSORS_IMPL(DuHeader, LastModifUser,         String,  QString,  QString())
DU_KEY_ACCESSORS_IMPL(DuHeader, LastModifUserId,       String,  QString,  QString())

DU_KEY_ACCESSORS_IMPL(DuHeader, Size,           Numeric,    int,    -1)
DU_KEY_ACCESSORS_IMPL(DuHeader, MetaData,       Numeric,    int,    -1)
