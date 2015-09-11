#include "dustring.h"


DU_OBJECT_IMPL(DuString)

DuString::DuString(const QString &value, int maxSize) :
    DuValue(maxSize)
{
    setString(value);
}

DuString::DuString(int maxSize) :
    DuValue(maxSize)
{
    setString(QString());
}

DuString::~DuString()
{
}

DuObjectPtr DuString::clone() const
{
    return DuStringPtr(new DuString(*this));
}


QByteArray DuString::toDuMusicBinary() const
{
    QByteArray array;
    array.clear();

    array.append(getString());

    int bytesLeft = getMaxSize() - getString().size();
    for (int i= 0; i < bytesLeft; i++)
    {
        array.append((char)0x00);
    }

    return array;
}


QByteArray DuString::toMidiBinary() const
{
    QByteArray array;
    array.clear();

    array.append(getString());

    return array;
}


QJsonValue DuString::toJson() const
{
    return QJsonValue(getString());
}


QHttpPart DuString::toHttpPart(const QString &name) const
{
    QHttpPart part;
    part.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"" + name + "\"");
    part.setBody(getString().toUtf8());

    return part;
}


QDebug DuString::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuString(" << getString() << ")";

    return dbg.space();
}


QString DuString::getString() const
{
    return getValue().toString();
}


/**
 * @brief If the size of the parameter string is <= getMaxSize() or getMaxSize() is -1,
 *        sets the value of the DuString to the parameter value and returns true,
 *        otherwise sets the value of the DuString to the getMaxSize()
 *        first characters of the parameter value and returns false.
 * @param value
 * @return
 */

bool DuString::setString(const QString &value)
{
    int size = getMaxSize();
    if (getMaxSize() != -1 && value.size() > size)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuString::setString()\n"
                   << "the byte array was longer than the"
                   << "maximum size" << size
                   << "and was truncated before being set";

        setValue(value.left(size).toUtf8());
        return false;
    }

    setValue(value.toUtf8());
    return true;
}
