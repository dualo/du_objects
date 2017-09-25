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

DuProfilePtr DuProfile::fromJsonApi(const JsonApiResourceObject &jsonProfile, int recursionLevel)
{
    if (jsonProfile.isNull())
        return DuProfilePtr();

    // CHECK TYPE
    if (jsonProfile.type() != "users")
        return DuProfilePtr();

    // ATTRIBUTES
    const QJsonObject& dataObject = jsonProfile.attributes();

    const QJsonValue& pseudoValue       = dataObject.value("name");
    const QJsonValue& emailValue        = dataObject.value("email");
    const QJsonValue& creationDateValue = dataObject.value("created_at");
    const QJsonValue& roleValue         = dataObject.value("authorization");
    if (!pseudoValue.isString()
            || !emailValue.isString()
            || !creationDateValue.isString()
            || !roleValue.isString())
        return DuProfilePtr();

    // RELATIONSHIPS
    const QList<JsonApiResourceObject>& dutouchsArray = jsonProfile.relationships().values("du-touchs");


    // CREATE PROFILE

    DuProfilePtr outProfile(new DuProfile);

    outProfile->setPseudo(pseudoValue.toString());
    outProfile->setMail(emailValue.toString());
    outProfile->setCreationDate(QDateTime::fromString(creationDateValue.toString(), "yyyy-MM-dd HH:mm:ss"));
    outProfile->setGUID(jsonProfile.id());

    const QString& role = roleValue.toString();
    if (role == QLatin1String("all") || role.isEmpty())
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
    for (const JsonApiResourceObject& duTouch : dutouchsArray)
    {
        const DuTouchPtr& device = DuTouch::fromJsonApi(duTouch);
        if (device == NULL)
            continue;

        device->setOwner(outProfile->getPseudo());
        device->setOwnerId(outProfile->getGUID());

        list->append(device);
    }
    outProfile->setDuTouchList(list);

    // FIRSTNAME
    if (dataObject.contains("firstname"))
    {
        const QJsonValue& firstnameValue = dataObject.value("firstname");
        if (!firstnameValue.isString())
            return DuProfilePtr();

        outProfile->setFirstname(firstnameValue.toString());
    }

    // LASTNAME
    if (dataObject.contains("lastname"))
    {
        const QJsonValue& lastnameValue = dataObject.value("lastname");
        if (!lastnameValue.isString())
            return DuProfilePtr();

        outProfile->setLastname(lastnameValue.toString());
    }

    // AVATAR
    if (dataObject.contains("avatar"))
    {
        const QJsonValue& avatarValue = dataObject.value("avatar");
        if (!avatarValue.isString())
            return DuProfilePtr();

        outProfile->setAvatarUrl(avatarValue.toString());
    }

    // FRIENDS
    //TODO
    Q_UNUSED(recursionLevel)
//    if (recursionLevel > 0)
//    {
//        QJsonArray friendsList = jsonProfile.value(QStringLiteral("friends")).toArray();

//        DuArrayPtr<DuProfile> friends(new DuArray<DuProfile>);
//        for (QJsonArray::iterator it = friendsList.begin(); it != friendsList.end(); ++it)
//        {
//            if (!(*it).isObject())
//            {
//                qCCritical(LOG_CAT_DU_OBJECT) << "user not an object :" << *it;
//                continue;
//            }

//            DuProfilePtr friendProfile = fromJson((*it).toObject(), recursionLevel - 1);

//            if (friendProfile != NULL)
//            {
//                friends->append(friendProfile);
//            }
//        }

//        outProfile->setFriends(friends);
//    }

    return outProfile;
}

QHttpMultiPart *DuProfile::toHttpMultiPart(const QByteArray &boundary) const
{
    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    multiPart->setBoundary(boundary);

    multiPart->append(getChild(KeyGUID)->toHttpPart(QStringLiteral("id")));
    multiPart->append(getChild(KeyLastname)->toHttpPart(QStringLiteral("lastname")));
    multiPart->append(getChild(KeyFirstname)->toHttpPart(QStringLiteral("firstname")));
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
