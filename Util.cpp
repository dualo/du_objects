#include "Util.h"

#include <QObject>

QString Util::sizeToString(int size)
{
    if (size < 1000)
    {
        return QObject::tr("%1 bytes").arg(size);
    }
    else if (size < 1000000)
    {
        return QObject::tr("%1 Ko").arg(QString::number((double)size/1000.0, 'f', 1));
    }
    else if (size < 1000000000)
    {
        return QObject::tr("%1 Mo").arg(QString::number((double)size/1000000.0, 'f', 1));
    }
    else
    {
        return QObject::tr("%1 Go").arg(QString::number((double)size/1000000000.0, 'f', 1));
    }
}
