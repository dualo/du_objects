#ifndef DUHEADER_H
#define DUHEADER_H

#include "ducontainer.h"
#include "dustring.h"
#include "dunumeric.h"


#define KEY_HEAD_FILEVERSION        "FileVersion"

#define KEY_HEAD_ORIGINALSN         "OriginalSerialNumber"
#define KEY_HEAD_ORIGINALNAME       "OriginalName"
#define KEY_HEAD_ORIGINALUSER       "OriginalUser"
#define KEY_HEAD_ORIGINALUSERID     "OriginalUserID"

#define KEY_HEAD_LASTMODIFSN        "LastModifSerialNumber"
#define KEY_HEAD_LASTMODIFNAME      "LastModifName"
#define KEY_HEAD_LASTMODIFUSER      "LastModifUser"
#define KEY_HEAD_LASTMODIFUSERID    "LastModifUserID"

#define KEY_HEAD_SONGID             "SongID"
#define KEY_HEAD_SONGNAME           "SongName"
#define KEY_HEAD_SONGVERSION        "SongVersion"

#define HEADER_NAME_SIZE            16


class DuHeader : public DuContainer
{
public:
    explicit DuHeader();
    ~DuHeader();

    static DuHeader *fromDuMusicFile(const music_song &du_song);

    static DuHeader *fromJson(const QJsonObject &jsonHeader);

    int getFileVersion() const;
    void setFileVersion(int value);

    QString getOriginalSerialNumber() const;
    void setOriginalSerialNumber(const QString value);

    QString getOriginalName() const;
    void setOriginalName(const QString value);

    QString getOriginalUser() const;
    void setOriginalUser(const QString value);

    QString getOriginalUserId() const;
    void setOriginalUserId(const QString value);

    QString getLastModifSerialNumber() const;
    void setLastModifSerialNumber(const QString value);

    QString getLastModifName() const;
    void setLastModifName(const QString value);

    QString getLastModifUser() const;
    void setLastModifUser(const QString value);

    QString getLastModifUserId() const;
    void setLastModifUserId(const QString value);

    int getSongId() const;
    void setSongId(int value);

    QString getSongName() const;
    void setSongName(const QString value);

    int getSongVersion() const;
    void setSongVersion(int value);
};

#endif // DUHEADER_H
