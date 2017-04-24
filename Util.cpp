#include "Util.h"

#include <QObject>

QString Util::sizeToString(quint64 size)
{
    if (size < 1024)
    {
        return QObject::tr("%1 Bytes").arg(size);
    }
    else if (size < 1024*1024)
    {
        return QObject::tr("%1 KB").arg(QString::number(static_cast<double>(size)/1024.0, 'f', 1));
    }
    else if (size < 1024*1024*1024)
    {
        return QObject::tr("%1 MB").arg(QString::number(static_cast<double>(size)/(1024.0*1024.0), 'f', 1));
    }
    else
    {
        return QObject::tr("%1 GB").arg(QString::number(static_cast<double>(size)/(1024.0*1024.0*1024.0), 'f', 1));
    }
}

QString Util::timeToString(qint64 msTime)
{
    if (msTime < 0)
    {
        return QString();
    }
    else if (msTime < 1000)
    {
        return QObject::tr("%1 ms").arg(msTime);
    }
    else if (msTime < 60000)
    {
        return QObject::tr("%1 sec").arg(QString::number(static_cast<double>(msTime)/1000.0, 'f', 1));
    }
    else if (msTime < 3600000)
    {
        return QObject::tr("%1 min").arg(QString::number(static_cast<double>(msTime)/60000.0, 'f', 1));
    }
    else
    {
        return QObject::tr("%1 hrs").arg(QString::number(static_cast<double>(msTime)/3600000.0, 'f', 1));
    }
}
