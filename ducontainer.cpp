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


int DuContainer::size() const
{
    int size = 0;

    QMapIterator<QString, DuObject *> i(children);
    while (i.hasNext())
    {
        i.next();
        if (i.value() == NULL)
            return -1;

        int tmpSize = i.value()->size();
        if (tmpSize == -1)
            return -1;

        size += tmpSize;
    }

    return size;
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


void DuContainer::addChild(const QString &key, DuObject *child)
{
    children.insert(key, child);
}

DuObject *DuContainer::getChild(const QString &key) const
{
    return children.value(key);
}
