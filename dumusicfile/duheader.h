#ifndef DUHEADER_H
#define DUHEADER_H

#include "../general/ducontainer.h"
#include "../general/dustring.h"
#include "../general/dunumeric.h"

#include "../miditodumusic/midiconversionhelper.h"


#define KEY_HEAD_FILEVERSION        "FileVersion"

#define KEY_HEAD_ORIGINALSN         "OriginalSerialNumber"
#define KEY_HEAD_ORIGINALNAME       "OriginalName"
#define KEY_HEAD_ORIGINALUSER       "OriginalUser"
#define KEY_HEAD_ORIGINALUSERID     "OriginalUserID"

#define KEY_HEAD_LASTMODIFSN        "LastModifSerialNumber"
#define KEY_HEAD_LASTMODIFNAME      "LastModifName"
#define KEY_HEAD_LASTMODIFUSER      "LastModifUser"
#define KEY_HEAD_LASTMODIFUSERID    "LastModifUserID"

#define KEY_HEAD_SIZE               "Size"
#define KEY_HEAD_METADATA           "MetaData"

#define HEADER_NAME_SIZE            16
#define HEADER_SIZE                 4 + 8 * HEADER_NAME_SIZE + 10 + 3


DU_OBJECT(DuHeader)

class DuHeader : public DuContainer
{
public:
    explicit DuHeader();
    ~DuHeader();

    virtual DuObjectPtr clone() const;

    static DuHeaderPtr fromDuMusicBinary(const music_song &du_song);
    static DuHeaderPtr fromJson(const QJsonObject &jsonHeader);

    QByteArray toDuMusicBinary() const;

    int size() const;

    int getFileVersion() const;
    bool setFileVersion(int value);

    QString getOriginalSerialNumber() const;
    bool setOriginalSerialNumber(const QString value);

    QString getOriginalName() const;
    bool setOriginalName(const QString value);

    QString getOriginalUser() const;
    bool setOriginalUser(const QString value);

    QString getOriginalUserId() const;
    bool setOriginalUserId(const QString value);

    QString getLastModifSerialNumber() const;
    bool setLastModifSerialNumber(const QString value);

    QString getLastModifName() const;
    bool setLastModifName(const QString value);

    QString getLastModifUser() const;
    bool setLastModifUser(const QString value);

    QString getLastModifUserId() const;
    bool setLastModifUserId(const QString value);

    int getSize() const;
    bool setSize(int value);

    int getMetaData() const;
    bool setMetaData(int value);
};

#endif // DUHEADER_H
