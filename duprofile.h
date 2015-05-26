#ifndef DUPROFILE_H
#define DUPROFILE_H

#include "ducontainer.h"

#define KEY_PROFILE_FIRSTNAME     "Firstname"
#define KEY_PROFILE_LASTNAME      "Lastname"
#define KEY_PROFILE_PSEUDO        "Pseudo"
#define KEY_PROFILE_MAIL          "Mail"
#define KEY_PROFILE_AVATAR_URL    "AvatarUrl"
#define KEY_PROFILE_CREATION_DATE "CreationDate"
#define KEY_PROFILE_ROLE          "Role"
#define KEY_PROFILE_GUID          "GUID"


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
};

Q_DECLARE_METATYPE(DuProfilePtr)

#endif // DUPROFILE_H
