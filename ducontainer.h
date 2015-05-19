#ifndef DUCONTAINER_H
#define DUCONTAINER_H

#include "duobject.h"
#include <QMap>
#include <QMapIterator>
#include <QList>
#include <QStringList>
#include <QHttpMultiPart>


class DuContainer : public DuObject
{
public:
    explicit DuContainer();
    virtual ~DuContainer();

    virtual QJsonValue toJson() const;
    virtual QHttpMultiPart* toHttpMultiPart(const QByteArray &boundary) const;
    QDebug debugPrint(QDebug dbg) const;

    virtual int size() const;

    QStringList keys() const;

    DuObject *operator[](const QString &label);

protected:
    void addChild(const QString &key, DuObject *child);
    DuObject *getChild(const QString &key) const;

    template <class T>
    T getChildAs(const QString &key) const;

private:
    QMap<QString, DuObject *> children;
};

template <class T>
inline T DuContainer::getChildAs(const QString &key) const
{
    return dynamic_cast<T>(getChild(key));
}

#endif // DUCONTAINER_H
