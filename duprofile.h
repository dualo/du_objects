#ifndef DUPROFILE_H
#define DUPROFILE_H

#include "general/ducontainer.h"


DU_OBJECT(DuArray)

DU_OBJECT(DuProfile)

class DuProfile : public DuContainer
{
public:
    enum Role
    {
        None = 0,
        Default,
        Beta,
        Temp,
        Test,
        Dev

        // Keep in sync with DuProfileObject::Type
    };

    DuProfile();

    virtual DuObjectPtr clone() const;

    /**
     * @brief Parse a json object to extract the profile infos
     * @param jsonProfile  The json object we want to parse
     * @param recursionLevel  0: don't get friends, 1: get friends, 2: get friends and friends of friends, etc.
     * @return A DuProfilePtr (NULL if error occured)
     */
    static DuProfilePtr fromJson(const QJsonObject &jsonProfile, int recursionLevel);

    QHttpMultiPart *toHttpMultiPart(const QByteArray &boundary) const;

    DU_KEY_ACCESSORS(Firstname,    QString)
    DU_KEY_ACCESSORS(Lastname,     QString)
    DU_KEY_ACCESSORS(Pseudo,       QString)
    DU_KEY_ACCESSORS(Mail,         QString)
    DU_KEY_ACCESSORS(AvatarUrl,    QUrl)
    DU_KEY_ACCESSORS(CreationDate, QDateTime)
    DU_KEY_ACCESSORS(Role,         Role)
    DU_KEY_ACCESSORS(GUID,         int)

    DU_KEY_ACCESSORS_OBJECT(DuTouchList, DuArray)
    DU_KEY_ACCESSORS_OBJECT(Friends,     DuArray)
};

Q_DECLARE_METATYPE(DuProfilePtr)

#endif // DUPROFILE_H
