#include "ducontainer.h"

#include <QDebug>

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

QHttpMultiPart *DuContainer::toHttpMultiPart(const QByteArray &boundary) const
{
    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    multiPart->setBoundary(boundary);

    QMapIterator<QString, DuObject*> i(children);
    while (i.hasNext())
    {
        i.next();
        if (i.value() != NULL)
        {
            QHttpPart part = i.value()->toHttpPart(i.key());
            if (part != QHttpPart())
            {
                multiPart->append(part);
            }
        }
    }

    return multiPart;
}

QDebug DuContainer::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuContainer(";

    QMapIterator<QString, DuObject*> i(children);
    while (i.hasNext())
    {
        i.next();
        dbg.nospace() << i.key();
        dbg.nospace() << "=";
        dbg.nospace() << i.value();
        if (i.hasNext())
            dbg.nospace() << ", ";
    }

    dbg.nospace() << ")";

    return dbg.space();
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
