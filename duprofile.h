#ifndef DUPROFILE_H
#define DUPROFILE_H

#include "general/ducontainer.h"

#define KEY_PROFILE_FIRSTNAME     "Firstname"
#define KEY_PROFILE_LASTNAME      "Lastname"
#define KEY_PROFILE_PSEUDO        "Pseudo"
#define KEY_PROFILE_MAIL          "Mail"
#define KEY_PROFILE_AVATAR_URL    "AvatarUrl"
#define KEY_PROFILE_CREATION_DATE "CreationDate"
#define KEY_PROFILE_ROLE          "Role"
#define KEY_PROFILE_GUID          "GUID"
#define KEY_PROFILE_DUTOUCH_LIST  "DuTouchList"
#define KEY_PROFILE_FRIENDS       "Friends"


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

    QString getFirstname() const;
    bool setFirstname(const QString& value);

    QString getLastname() const;
    bool setLastname(const QString& value);

    QString getPseudo() const;
    bool setPseudo(const QString& value);

    QString getMail() const;
    bool setMail(const QString& value);

    QUrl getAvatarUrl() const;
    bool setAvatarUrl(const QUrl& value);

    QDateTime getCreationDate() const;
    bool setCreationDate(const QDateTime& value);

    Role getRole() const;
    bool setRole(Role value);

    int getGUID() const;
    bool setGUID(int value);

    DuArrayConstPtr getDuTouchList() const;
    DuArrayPtr getDuTouchList();
    void setDuTouchList(const DuArrayPtr& value);

    DuArrayConstPtr getFriends() const;
    void setFriends(const DuArrayPtr& value);
};

Q_DECLARE_METATYPE(DuProfilePtr)

#endif // DUPROFILE_H
