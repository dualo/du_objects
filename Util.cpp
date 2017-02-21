#include "Util.h"

#include <QObject>

QString Util::sizeToString(int size)
{
    if (size < 0)
    {
        return QString();
    }
    else if (size < 1000)
    {
        return QObject::tr("%1 Bytes").arg(size);
    }
    else if (size < 1000000)
    {
        return QObject::tr("%1 KB").arg(QString::number((double)size/1000.0, 'f', 1));
    }
    else if (size < 1000000000)
    {
        return QObject::tr("%1 MB").arg(QString::number((double)size/1000000.0, 'f', 1));
    }
    else
    {
        return QObject::tr("%1 GB").arg(QString::number((double)size/1000000000.0, 'f', 1));
    }
}
