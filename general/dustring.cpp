#include "dustring.h"

#include <QDebug>

DU_OBJECT_IMPL(DuString)

DuString::DuString(const QString &value, int maxSize) :
    DuValue(maxSize)
{
    setString(value);
}

DuString::DuString(int maxSize) :
    DuValue(maxSize)
{
    setString("");
}

DuString::~DuString()
{
}

DuObjectPtr DuString::clone() const
{
    return DuStringPtr(new DuString(*this));
}


QByteArray DuString::toDuMusicFile() const
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
    if (getMaxSize() != -1 && value.size() > getMaxSize())
    {
        setValue(value.left(getMaxSize()));
        return false;
    }

    setValue(value);
    return true;
}
