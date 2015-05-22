#include "duprofile.h"

#include "dudate.h"
#include "dunumeric.h"
#include "dustring.h"
#include "duurl.h"

#include <QDebug>

DuProfile::DuProfile()
{
    addChild(KEY_PROFILE_FIRSTNAME,     new DuString(""));
    addChild(KEY_PROFILE_LASTNAME,      new DuString(""));
    addChild(KEY_PROFILE_PSEUDO,        new DuString(""));
    addChild(KEY_PROFILE_MAIL,          new DuString(""));
    addChild(KEY_PROFILE_AVATAR_URL,    new DuUrl);
    addChild(KEY_PROFILE_CREATION_DATE, new DuDate);
    addChild(KEY_PROFILE_ROLE,          new DuNumeric(None));
    addChild(KEY_PROFILE_GUID,          new DuNumeric(-1));
}

QString DuProfile::getFirstname() const
{
    const DuString* firstname = getChildAs<DuString*>(KEY_PROFILE_FIRSTNAME);

    if (firstname == NULL)
    {
        qCritical() << "Unable to cast" << KEY_PROFILE_FIRSTNAME << "to DuString*";
        return QString();
    }

    return firstname->getString();
}

bool DuProfile::setFirstname(const QString &value)
{
    DuString *tmp = getChildAs<DuString*>(KEY_PROFILE_FIRSTNAME);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuProfile::getLastname() const
{
    const DuString* lastname = getChildAs<DuString*>(KEY_PROFILE_LASTNAME);

    if (lastname == NULL)
    {
        qCritical() << "Unable to cast" << KEY_PROFILE_LASTNAME << "to DuString*";
        return QString();
    }

    return lastname->getString();
}

bool DuProfile::setLastname(const QString &value)
{
    DuString *tmp = getChildAs<DuString*>(KEY_PROFILE_LASTNAME);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuProfile::getPseudo() const
{
    const DuString* pseudo = getChildAs<DuString*>(KEY_PROFILE_PSEUDO);

    if (pseudo == NULL)
    {
        qCritical() << "Unable to cast" << KEY_PROFILE_PSEUDO << "to DuString*";
        return QString();
    }

    return pseudo->getString();
}

bool DuProfile::setPseudo(const QString &value)
{
    DuString *tmp = getChildAs<DuString*>(KEY_PROFILE_PSEUDO);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuProfile::getMail() const
{
    const DuString* mail = getChildAs<DuString*>(KEY_PROFILE_MAIL);

    if (mail == NULL)
    {
        qCritical() << "Unable to cast" << KEY_PROFILE_MAIL << "to DuString*";
        return QString();
    }

    return mail->getString();
}

bool DuProfile::setMail(const QString &value)
{
    DuString *tmp = getChildAs<DuString*>(KEY_PROFILE_MAIL);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QUrl DuProfile::getAvatarUrl() const
{
    const DuUrl* avatarUrl = getChildAs<DuUrl*>(KEY_PROFILE_AVATAR_URL);

    if (avatarUrl == NULL)
    {
        qCritical() << "Unable to cast" << KEY_PROFILE_AVATAR_URL << "to DuUrl*";
        return QUrl();
    }

    return avatarUrl->getUrl();
}

bool DuProfile::setAvatarUrl(const QUrl &value)
{
    DuUrl *tmp = getChildAs<DuUrl*>(KEY_PROFILE_AVATAR_URL);

    if (tmp == NULL)
        return false;

    tmp->setUrl(value);
    return true;
}

QDateTime DuProfile::getCreationDate() const
{
    const DuDate* creationDate = getChildAs<DuDate*>(KEY_PROFILE_CREATION_DATE);

    if (creationDate == NULL)
    {
        qCritical() << "Unable to cast" << KEY_PROFILE_CREATION_DATE << "to DuDate*";
        return QDateTime();
    }

    return creationDate->getDate();
}

bool DuProfile::setCreationDate(const QDateTime &value)
{
    DuDate *tmp = getChildAs<DuDate*>(KEY_PROFILE_CREATION_DATE);

    if (tmp == NULL)
        return false;

    tmp->setDate(value);
    return true;
}

DuProfile::Role DuProfile::getRole() const
{
    const DuNumeric* role = getChildAs<DuNumeric*>(KEY_PROFILE_ROLE);

    if (role == NULL)
    {
        qCritical() << "Unable to cast" << KEY_PROFILE_ROLE << "to DuNumeric*";
        return None;
    }

    return (Role)role->getNumeric();
}

bool DuProfile::setRole(DuProfile::Role value)
{
    DuNumeric *tmp = getChildAs<DuNumeric*>(KEY_PROFILE_ROLE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuProfile::getGUID() const
{
    const DuNumeric* guid = getChildAs<DuNumeric*>(KEY_PROFILE_GUID);

    if (guid == NULL)
    {
        qCritical() << "Unable to cast" << KEY_PROFILE_GUID << "to DuNumeric*";
        return -1;
    }

    return guid->getNumeric();
}

bool DuProfile::setGUID(int value)
{
    DuNumeric *tmp = getChildAs<DuNumeric*>(KEY_PROFILE_GUID);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}
