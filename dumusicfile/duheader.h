#ifndef DUHEADER_H
#define DUHEADER_H

#include "../general/ducontainer.h"


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

    DU_KEY_ACCESSORS(FileVersion,   int)

    DU_KEY_ACCESSORS(OriginalSerialNumber,  QString)
    DU_KEY_ACCESSORS(OriginalName,          QString)
    DU_KEY_ACCESSORS(OriginalUser,          QString)
    DU_KEY_ACCESSORS(OriginalUserId,        QString)

    DU_KEY_ACCESSORS(LastModifSerialNumber, QString)
    DU_KEY_ACCESSORS(LastModifName,         QString)
    DU_KEY_ACCESSORS(LastModifUser,         QString)
    DU_KEY_ACCESSORS(LastModifUserId,       QString)

    DU_KEY_ACCESSORS(Size,          int)
    DU_KEY_ACCESSORS(MetaData,      int)
};

#endif // DUHEADER_H
