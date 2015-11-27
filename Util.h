#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QVariantList>
#include <QUrl>

namespace Util
{
    QString sizeToString(int size);

    template <typename T>
    QVariantList listToVariantList(const QList<T> &list)
    {
        QVariantList newList;
        foreach(const T &item, list)
            newList << QVariant::fromValue(item);

        return newList;
    }

    template <typename T>
    QList<T> listFromVariantList(const QVariantList &list)
    {
        QList<T> newList;
        foreach(const QVariant &item, list)
            newList << item.value<T>();

        return newList;
    }
}

#endif // UTIL_H
