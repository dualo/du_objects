#include "DuGameEventMessage.h"

#include "../general/dustring.h"

#include <cstring>

DU_OBJECT_IMPL(DuGameEventMessage)

DuGameEventMessage::DuGameEventMessage() : DuContainer()
{
    addChild(KeyLine1, new DuString(ARRANGEMENT_MSG_MAXSIZE));
    addChild(KeyLine2, new DuString(ARRANGEMENT_MSG_MAXSIZE));
    addChild(KeyLine3, new DuString(ARRANGEMENT_MSG_MAXSIZE));
}

DuGameEventMessagePtr DuGameEventMessage::fromStruct(const s_arrangement_msg &messageStruct)
{
    DuGameEventMessagePtr message(new DuGameEventMessage);

    bool verif = true;

    verif = message->setLine1(DuString::fromStruct(messageStruct.am_line1, ARRANGEMENT_MSG_MAXSIZE)) ? verif : false;
    verif = message->setLine2(DuString::fromStruct(messageStruct.am_line2, ARRANGEMENT_MSG_MAXSIZE)) ? verif : false;
    verif = message->setLine3(DuString::fromStruct(messageStruct.am_line3, ARRANGEMENT_MSG_MAXSIZE)) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "An attribute was not properly set";
    }

    return message;
}

DuObjectPtr DuGameEventMessage::clone() const
{
    return DuGameEventMessagePtr(new DuGameEventMessage(*this));
}

QByteArray DuGameEventMessage::toDuMusicBinary() const
{
    s_arrangement_msg message;

    QByteArray tmpArray;
    QString tmpStr;

    tmpArray.fill(0x00, ARRANGEMENT_MSG_MAXSIZE);
    tmpStr = getLine1();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toLatin1());
    std::memcpy(message.am_line1, tmpArray.constData(), ARRANGEMENT_MSG_MAXSIZE);

    tmpArray.fill(0x00, ARRANGEMENT_MSG_MAXSIZE);
    tmpStr = getLine2();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toLatin1());
    std::memcpy(message.am_line2, tmpArray.constData(), ARRANGEMENT_MSG_MAXSIZE);

    tmpArray.fill(0x00, ARRANGEMENT_MSG_MAXSIZE);
    tmpStr = getLine3();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toLatin1());
    std::memcpy(message.am_line3, tmpArray.constData(), ARRANGEMENT_MSG_MAXSIZE);

    return QByteArray(reinterpret_cast<const char*>(&message), ARRANGEMENT_MSG_SIZE);
}

int DuGameEventMessage::size() const
{
    return ARRANGEMENT_MSG_SIZE;
}

DU_KEY_ACCESSORS_IMPL(DuGameEventMessage, Line1, String, QString, QString())
DU_KEY_ACCESSORS_IMPL(DuGameEventMessage, Line2, String, QString, QString())
DU_KEY_ACCESSORS_IMPL(DuGameEventMessage, Line3, String, QString, QString())
