#ifndef DUCONTAINER_H
#define DUCONTAINER_H

#include "duobject.h"
#include <QMap>
#include <QMapIterator>
#include <QList>
#include <QStringList>
#include <QHttpMultiPart>
#include <QSharedPointer>


class DuContainer : public DuObject
{
public:
    explicit DuContainer();
    virtual ~DuContainer();

    virtual QJsonValue toJson() const;
    virtual QByteArray toDuMusicFile() const;

    virtual QHttpMultiPart* toHttpMultiPart(const QByteArray &boundary) const;
    QDebug debugPrint(QDebug dbg) const;

    virtual int size() const;

    QStringList keys() const;

    QSharedPointer<DuObject> operator[](const QString &label);

protected:
    void addChild(const QString &key, DuObject *child);
    QSharedPointer<DuObject> getChild(const QString &key) const;

    template <class T>
    QSharedPointer<T> getChildAs(const QString &key) const;

private:
    QMap<QString, QSharedPointer<DuObject> > children;
};

template <class T>
inline QSharedPointer<T> DuContainer::getChildAs(const QString &key) const
{
    return getChild(key).dynamicCast<T>();
}

#endif // DUCONTAINER_H
