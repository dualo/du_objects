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
