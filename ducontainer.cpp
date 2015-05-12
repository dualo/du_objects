#include "ducontainer.h"

DuContainer::DuContainer() :
    DuObject()
{
}

DuContainer::~DuContainer()
{
    QMutableMapIterator<QString, DuObject *> i(children);

    while (i.hasNext())
    {
        i.next();
        if (i.value() != NULL)
        {
            delete i.value();
            i.remove();
        }
    }
}


QJsonValue DuContainer::toJson() const
{
    QJsonObject object;

    QMapIterator<QString, DuObject *> i(children);

    while (i.hasNext())
    {
        i.next();
        if (i.value() != NULL)
            object[i.key()] = i.value()->toJson();
    }

    return QJsonValue(object);
}


void DuContainer::addChild(const QString &key, DuObject *child)
{
    children.insert(key, child);
}

DuObject *DuContainer::getChild(const QString &key) const
{
    return children.value(key);
}


QStringList DuContainer::keys() const
{
    return children.keys();
}


DuObject *DuContainer::operator[](const QString &label)
{
    if (!children.contains(label))
        return NULL;

    return children[label];
}
