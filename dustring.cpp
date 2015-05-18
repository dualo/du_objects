#include "dustring.h"

#include <QDebug>

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
 * @brief If the size of the parameter string is <= getMaxSize(), sets
 *        the value of the DuString to the parameter value and returns
 *        true, otherwise sets the value of the DuString to the
 *        getMaxSize() first characters of the parameter value and
 *        returns false;
 * @param value
 * @return
 */

bool DuString::setString(const QString &value)
{
    if (value.size() > getMaxSize())
    {
        QString tmp(value);
        tmp.truncate(getMaxSize());

        setValue(tmp);
        return false;
    }

    setValue(value);
    return true;
}
