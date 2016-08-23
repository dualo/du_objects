#include "duprofile.h"

#include "dutouch.h"

#include "general/duarray.h"
#include "general/dudate.h"
#include "general/dunumeric.h"
#include "general/dustring.h"
#include "general/duurl.h"

#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QMimeDatabase>

DU_OBJECT_IMPL(DuProfile)

DuProfile::DuProfile() :
    DuContainer()
{
    addChild(KeyFirstname,    new DuString);
    addChild(KeyLastname,     new DuString);
    addChild(KeyPseudo,       new DuString);
    addChild(KeyMail,         new DuString);
    addChild(KeyAvatarUrl,    new DuUrl);
    addChild(KeyCreationDate, new DuDate);
    addChild(KeyRole,         new DuNumeric(None));
    addChild(KeyGUID,         new DuNumeric(-1));
    addChild(KeyDuTouchList,  new DuArray<DuTouch>);
    addChild(KeyFriends,      new DuArray<DuProfile>);
}

DuObjectPtr DuProfile::clone() const
{
    return DuProfilePtr(new DuProfile(*this));
}

DuProfilePtr DuProfile::fromJson(const QJsonObject &jsonProfile, int recursionLevel)
{
    if (jsonProfile.isEmpty())
    {
        return DuProfilePtr();
    }

    DuProfilePtr outProfile(new DuProfile);

    outProfile->setFirstname(jsonProfile.value(QStringLiteral("Firstname")).toString());
    outProfile->setLastname(jsonProfile.value(QStringLiteral("Lastname")).toString());
    outProfile->setPseudo(jsonProfile.value(QStringLiteral("username")).toString());
    outProfile->setMail(jsonProfile.value(QStringLiteral("email")).toString());
    outProfile->setCreationDate(QDateTime::fromTime_t(static_cast<uint>(jsonProfile.value(QStringLiteral("time_created")).toInt())));
    outProfile->setGUID(jsonProfile.value(QStringLiteral("global_id")).toInt());

    QString role = jsonProfile.value(QStringLiteral("role")).toString();
    if (role == QLatin1String("default") || role.isEmpty())
        outProfile->setRole(Default);
    else if (role == QLatin1String("beta"))
        outProfile->setRole(Beta);
    else if (role == QLatin1String("temp"))
        outProfile->setRole(Temp);
    else if (role == QLatin1String("test"))
        outProfile->setRole(Test);
    else if (role == QLatin1String("dev"))
        outProfile->setRole(Dev);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Unknown role :" << role;
        outProfile->setRole(Default);
    }

    // DU-TOUCH LIST
    DuArrayPtr<DuTouch> list(new DuArray<DuTouch>);
    QJsonArray array = jsonProfile.value(QStringLiteral("device_list")).toArray();
    for (QJsonArray::iterator it = array.begin(); it != array.end(); ++it)
    {
        const QJsonValueRef& value = (*it);
        if (!value.isObject())
        {
            continue;
        }

        const QJsonObject& object = value.toObject();
        const QString& serial               = object.value(QStringLiteral("dutouch_serial")).toString();
        const QString& name                 = object.value(QStringLiteral("dutouch_name")).toString();
        const QString& firmware             = object.value(QStringLiteral("firmware")).toString();
        const QString& firmwareUpdateDate   = object.value(QStringLiteral("firmware_update_date")).toString();
        const QString& soundbank            = object.value(QStringLiteral("soundbank")).toString();
        const QString& soundbankUpdateDate  = object.value(QStringLiteral("soundbank_update_date")).toString();
        const QString& dissocScheduled      = object.value(QStringLiteral("dissociation_scheduled")).toString();

        DuTouchPtr device(new DuTouch);
        device->setSerialNumber(serial);
        device->setName(name);
        device->setOwner(outProfile->getPseudo());
        device->setOwnerId(outProfile->getGUID());
        device->setPlugged(false);
        device->setConnected(false);
        device->setBusy(false);
        device->setVersion(firmware);
        device->setUpdateDate(QDateTime::fromString(firmwareUpdateDate, QStringLiteral("yyyy-MM-dd HH:mm:ss")));
        device->setSoundbankVersion(soundbank);
        device->setSoundbankUpdateDate(QDateTime::fromString(soundbankUpdateDate, QStringLiteral("yyyy-MM-dd HH:mm:ss")));
        device->setDissocScheduled(dissocScheduled == QStringLiteral("yes"));

        list->append(device);
    }
    outProfile->setDuTouchList(list);

    // AVATAR
    outProfile->setAvatarUrl(jsonProfile.value(QStringLiteral("avatar")).toString());

    // FRIENDS
    if (recursionLevel > 0)
    {
        QJsonArray friendsList = jsonProfile.value(QStringLiteral("friends")).toArray();

        DuArrayPtr<DuProfile> friends(new DuArray<DuProfile>);
        for (QJsonArray::iterator it = friendsList.begin(); it != friendsList.end(); ++it)
        {
            if (!(*it).isObject())
            {
                qCCritical(LOG_CAT_DU_OBJECT) << "user not an object :" << *it;
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

    multiPart->append(getChild(KeyGUID)->toHttpPart(QStringLiteral("guid")));
    multiPart->append(getChild(KeyLastname)->toHttpPart(QStringLiteral("Lastname")));
    multiPart->append(getChild(KeyFirstname)->toHttpPart(QStringLiteral("Firstname")));
    multiPart->append(getChild(KeyMail)->toHttpPart(QStringLiteral("email")));

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
            qCCritical(LOG_CAT_DU_OBJECT) << "Can't open file" << avatarUrl.toString();
            delete avatar;
        }
    }

    return multiPart;
}

DU_KEY_ACCESSORS_IMPL(DuProfile, Firstname,    String, QString, QString())
DU_KEY_ACCESSORS_IMPL(DuProfile, Lastname,     String, QString, QString())
DU_KEY_ACCESSORS_IMPL(DuProfile, Pseudo,       String, QString, QString())
DU_KEY_ACCESSORS_IMPL(DuProfile, Mail,         String, QString, QString())
DU_KEY_ACCESSORS_IMPL(DuProfile, AvatarUrl,    Url, QUrl, QUrl())
DU_KEY_ACCESSORS_IMPL(DuProfile, CreationDate, Date, QDateTime, QDateTime())
DU_KEY_ACCESSORS_IMPL(DuProfile, Role,         Numeric, DuProfile::Role, None)
DU_KEY_ACCESSORS_IMPL(DuProfile, GUID,         Numeric, int, -1)

DU_KEY_ACCESSORS_OBJECT_TEMPLATE_IMPL(DuProfile, DuTouchList, DuArray, DuTouch)
DU_KEY_ACCESSORS_OBJECT_TEMPLATE_IMPL(DuProfile, Friends,     DuArray, DuProfile)
