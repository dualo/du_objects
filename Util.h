#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QVariantList>
#include <QUrl>

namespace Util
{
    QString sizeToString(quint64 size);
    QString timeToString(qint64 msTime);

    template <typename T>
    QVariantList listToVariantList(const QList<T> &list)
    {
        QVariantList newList;
        for (const T &item : list)
            newList << QVariant::fromValue(item);

        return newList;
    }

    template <typename T>
    QList<T> listFromVariantList(const QVariantList &list)
    {
        QList<T> newList;
        for (const QVariant &item : list)
            newList << item.value<T>();

        return newList;
    }

    // Key has to be convertible to QString
    template <class Key, class T>
    QVariantMap mapToVariantMap(const QMap<Key, T> &map)
    {
        QVariantMap newMap;
        QMapIterator<Key, T> it(map);
        while (it.hasNext())
        {
            it.next();
            newMap.insert(QVariant::fromValue(it.key()).toString(), QVariant::fromValue(it.value()));
        }

        return newMap;
    }

    // Key has to be convertible from QString
    template <class Key, class T>
    QMap<Key, T> mapFromVariantMap(const QVariantMap &map)
    {
        QMap<Key, T> newMap;
        QMapIterator<QString, QVariant> it(map);
        while (it.hasNext())
        {
            it.next();
            newMap.insert(QVariant::fromValue(it.key()).value<Key>(), it.value().value<T>());
        }

        return newMap;
    }

    template <typename T>
    QByteArray intToByteArray(T value)
    {
        static_assert(std::is_integral<T>::value, "Integer required.");
        return QByteArray(reinterpret_cast<const char*>(&value), sizeof(T));
    }

    template <typename T>
    T byteArrayToInt(const QByteArray& data)
    {
        static_assert(std::is_integral<T>::value, "Integer required.");
        return *(reinterpret_cast<const T*>(data.data()));
    }
}

#endif // UTIL_H
