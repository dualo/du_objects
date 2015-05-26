#include "duprofile.h"

#include "dudate.h"
#include "dunumeric.h"
#include "dustring.h"
#include "dutouch.h"
#include "duurl.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QMimeDatabase>

DU_OBJECT_IMPL(DuProfile)

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
    addChild(KEY_PROFILE_DUTOUCH_LIST,  new DuArray);
    addChild(KEY_PROFILE_FRIENDS,       new DuArray);
}

DuProfilePtr DuProfile::fromJson(const QJsonObject &jsonProfile, int recursionLevel)
{
    if (jsonProfile.isEmpty())
    {
        return DuProfilePtr();
    }

    DuProfilePtr outProfile(new DuProfile);

    outProfile->setFirstname(jsonProfile.value("Firstname").toString());
    outProfile->setLastname(jsonProfile.value("Lastname").toString());
    outProfile->setPseudo(jsonProfile.value("username").toString());
    outProfile->setMail(jsonProfile.value("email").toString());
    outProfile->setCreationDate(QDateTime::fromTime_t(jsonProfile.value("time_created").toInt()));
    outProfile->setGUID(jsonProfile.value("global_id").toInt());

    QString role = jsonProfile.value("role").toString();
    if (role == "default" || role == "")
        outProfile->setRole(Default);
    else if (role == "beta")
        outProfile->setRole(Beta);
    else if (role == "temp")
        outProfile->setRole(Temp);
    else if (role == "test")
        outProfile->setRole(Test);
    else if (role == "dev")
        outProfile->setRole(Dev);
    else
    {
        qCritical() << "Unknown role :" << role;
        outProfile->setRole(Default);
    }

    // DU-TOUCH LIST
    DuArrayPtr list(new DuArray);
    QJsonArray array = jsonProfile.value("device_list").toArray();
    for (QJsonArray::iterator it = array.begin(); it != array.end(); ++it)
    {
        QString serial = (*it).toObject().value("dutouch_serial").toString();
        QString name = (*it).toObject().value("dutouch_name").toString();
        QString firmware = (*it).toObject().value("firmware").toString();
        QString firmwareUpdateDate = (*it).toObject().value("firmware_update_date").toString();
        QString soundbank = (*it).toObject().value("soundbank").toString();
        QString soundbankUpdateDate = (*it).toObject().value("soundbank_update_date").toString();

        DuTouchPtr device(new DuTouch);
        device->setSerialNumber(serial);
        device->setName(name);
        device->setOwner(outProfile->getPseudo());
        device->setOwnerId(outProfile->getGUID());
        device->setPlugged(false);
        device->setConnected(false);
        device->setBusy(false);
        device->setVersion(firmware);
        device->setUpdateDate(QDateTime::fromString(firmwareUpdateDate, "yyyy-MM-dd HH:mm:ss"));
        device->setSoundbankVersion(soundbank);
        device->setSoundbankUpdateDate(QDateTime::fromString(soundbankUpdateDate, "yyyy-MM-dd HH:mm:ss"));

        list->append(device);
    }
    outProfile->setDuTouchList(list);

    // AVATAR
    outProfile->setAvatarUrl(jsonProfile.value("avatar").toString());

    // FRIENDS
    if (recursionLevel > 0)
    {
        QJsonArray friendsList = jsonProfile.value("friends").toArray();

        DuArrayPtr friends(new DuArray);
        for (QJsonArray::iterator it = friendsList.begin(); it != friendsList.end(); ++it)
        {
            if (!(*it).isObject())
            {
                qCritical() << "user not an object :" << *it;
                continue;
            }

            DuProfilePtr friendProfile = fromJson((*it).toObject(), recursionLevel - 1);

            if (friendProfile != NULL)
            {
                friends->append(friendProfile);
            }
        }

        outProfile->setFriends(friends);
    }

    return outProfile;
}

QHttpMultiPart *DuProfile::toHttpMultiPart(const QByteArray &boundary) const
{
    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    multiPart->setBoundary(boundary);

    multiPart->append(getChild(KEY_PROFILE_GUID)->toHttpPart("guid"));
    multiPart->append(getChild(KEY_PROFILE_LASTNAME)->toHttpPart("Lastname"));
    multiPart->append(getChild(KEY_PROFILE_FIRSTNAME)->toHttpPart("Firstname"));
    multiPart->append(getChild(KEY_PROFILE_MAIL)->toHttpPart("email"));

    QUrl avatarUrl = getAvatarUrl();
    if (!avatarUrl.isEmpty() && avatarUrl.isLocalFile())
    {
        QFile* avatar = new QFile(avatarUrl.toLocalFile());
        if (avatar->open(QIODevice::ReadOnly))
        {
            QFileInfo info(*avatar);
            QMimeDatabase db;
            QMimeType mimeType = db.mimeTypeForFile(info);

            QHttpPart avatarPart;
            avatarPart.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"avatar\"; filename=\"" + info.fileName() + "\"");
            avatarPart.setHeader(QNetworkRequest::ContentTypeHeader, mimeType.name());
            avatarPart.setBodyDevice(avatar);
            avatar->setParent(multiPart);

            multiPart->append(avatarPart);
        }
        else
        {
            qCritical() << "Can't open file" << avatarUrl.toString();
            delete avatar;
        }
    }

    return multiPart;
}

QString DuProfile::getFirstname() const
{
    const DuStringConstPtr& firstname = getChildAs<DuString>(KEY_PROFILE_FIRSTNAME);

    if (firstname == NULL)
    {
        qCritical() << "Unable to cast" << KEY_PROFILE_FIRSTNAME << "to DuString*";
        return QString();
    }

    return firstname->getString();
}

bool DuProfile::setFirstname(const QString &value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_PROFILE_FIRSTNAME);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuProfile::getLastname() const
{
    const DuStringConstPtr& lastname = getChildAs<DuString>(KEY_PROFILE_LASTNAME);

    if (lastname == NULL)
    {
        qCritical() << "Unable to cast" << KEY_PROFILE_LASTNAME << "to DuString*";
        return QString();
    }

    return lastname->getString();
}

bool DuProfile::setLastname(const QString &value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_PROFILE_LASTNAME);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuProfile::getPseudo() const
{
    const DuStringConstPtr& pseudo = getChildAs<DuString>(KEY_PROFILE_PSEUDO);

    if (pseudo == NULL)
    {
        qCritical() << "Unable to cast" << KEY_PROFILE_PSEUDO << "to DuString*";
        return QString();
    }

    return pseudo->getString();
}

bool DuProfile::setPseudo(const QString &value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_PROFILE_PSEUDO);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QString DuProfile::getMail() const
{
    const DuStringConstPtr& mail = getChildAs<DuString>(KEY_PROFILE_MAIL);

    if (mail == NULL)
    {
        qCritical() << "Unable to cast" << KEY_PROFILE_MAIL << "to DuString*";
        return QString();
    }

    return mail->getString();
}

bool DuProfile::setMail(const QString &value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_PROFILE_MAIL);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

QUrl DuProfile::getAvatarUrl() const
{
    const DuUrlConstPtr& avatarUrl = getChildAs<DuUrl>(KEY_PROFILE_AVATAR_URL);

    if (avatarUrl == NULL)
    {
        qCritical() << "Unable to cast" << KEY_PROFILE_AVATAR_URL << "to DuUrl*";
        return QUrl();
    }

    return avatarUrl->getUrl();
}

bool DuProfile::setAvatarUrl(const QUrl &value)
{
    DuUrlPtr tmp = getChildAs<DuUrl>(KEY_PROFILE_AVATAR_URL);

    if (tmp == NULL)
        return false;

    tmp->setUrl(value);
    return true;
}

QDateTime DuProfile::getCreationDate() const
{
    const DuDateConstPtr& creationDate = getChildAs<DuDate>(KEY_PROFILE_CREATION_DATE);

    if (creationDate == NULL)
    {
        qCritical() << "Unable to cast" << KEY_PROFILE_CREATION_DATE << "to DuDate*";
        return QDateTime();
    }

    return creationDate->getDate();
}

bool DuProfile::setCreationDate(const QDateTime &value)
{
    DuDatePtr tmp = getChildAs<DuDate>(KEY_PROFILE_CREATION_DATE);

    if (tmp == NULL)
        return false;

    tmp->setDate(value);
    return true;
}

DuProfile::Role DuProfile::getRole() const
{
    const DuNumericConstPtr& role = getChildAs<DuNumeric>(KEY_PROFILE_ROLE);

    if (role == NULL)
    {
        qCritical() << "Unable to cast" << KEY_PROFILE_ROLE << "to DuNumeric*";
        return None;
    }

    return (Role)role->getNumeric();
}

bool DuProfile::setRole(DuProfile::Role value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_PROFILE_ROLE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuProfile::getGUID() const
{
    const DuNumericConstPtr& guid = getChildAs<DuNumeric>(KEY_PROFILE_GUID);

    if (guid == NULL)
    {
        qCritical() << "Unable to cast" << KEY_PROFILE_GUID << "to DuNumeric*";
        return -1;
    }

    return guid->getNumeric();
}

bool DuProfile::setGUID(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_PROFILE_GUID);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

DuArrayConstPtr DuProfile::getDuTouchList() const
{
    return getChildAs<const DuArray>(KEY_PROFILE_DUTOUCH_LIST);
}

void DuProfile::setDuTouchList(const DuArrayPtr &value)
{
    addChild(KEY_PROFILE_DUTOUCH_LIST, value);
}

DuArrayConstPtr DuProfile::getFriends() const
{
    return getChildAs<const DuArray>(KEY_PROFILE_FRIENDS);
}

void DuProfile::setFriends(const DuArrayPtr &value)
{
    addChild(KEY_PROFILE_FRIENDS, value);
}
