#include "dustring.h"

#include <QJsonValue>


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

DuObjectPtr DuString::clone() const
{
    return DuStringPtr(new DuString(*this));
}


QByteArray DuString::toDuMusicBinary() const
{
    QByteArray array;
    array.clear();

    array.append(getString().toLatin1());

    int bytesLeft = getMaxSize() - getString().size();
    for (int i= 0; i < bytesLeft; i++)
    {
        array.append('0');
    }

    return array;
}


QByteArray DuString::toMidiBinary() const
{
    QByteArray array;
    array.clear();

    array.append(getString().toLocal8Bit());

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

QVariant DuString::checkValue(const QVariant &value, bool &success)
{
    if (!value.canConvert<QString>())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "value is not of type QString:" << value;
        success = false;
        return QVariant();
    }

    const QString& convertedValue = value.toString();

    int size = getMaxSize();
    if (size != -1 && convertedValue.size() > size)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                   << "the byte array was longer than the"
                   << "maximum size" << size
                   << "and was truncated before being set";

        success = false;
        return convertedValue.left(size);
    }

    success = true;
    return convertedValue;
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
    return setValue(value);
}

QString DuString::fromStruct(const quint8 *str, uint maxSize)
{
    const char* charStr = reinterpret_cast<const char*>(str);
    return QString::fromLatin1(charStr, static_cast<int>(qstrnlen(charStr, maxSize)));
}
