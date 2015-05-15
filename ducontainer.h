#ifndef DUCONTAINER_H
#define DUCONTAINER_H

#include "duobject.h"
#include <QMap>
#include <QMapIterator>
#include <QList>
#include <QStringList>


class DuContainer : public DuObject
{
public:
    explicit DuContainer();
    virtual ~DuContainer();

    virtual QJsonValue toJson() const;

    virtual int size() const;

    void addChild(const QString &key, DuObject *child);
    DuObject *getChild(const QString &key) const;

    QStringList keys() const;

    DuObject *operator[](const QString &label);

private:
    QMap<QString, DuObject *> children;
};

#endif // DUCONTAINER_H
