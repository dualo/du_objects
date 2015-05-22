#include "ducontainer.h"

#include <QDebug>

DuContainer::DuContainer() :
    DuObject()
{
}

DuContainer::~DuContainer()
{
}


QJsonValue DuContainer::toJson() const
{
    QJsonObject object;

    QMapIterator<QString, QSharedPointer<DuObject> > i(children);
    while (i.hasNext())
    {
        i.next();
        if (i.value() == NULL)
            return QJsonValue();

        const QJsonValue &jsonValue = i.value()->toJson();
        if (jsonValue.isNull())
            return QJsonValue();

        object[i.key()] = jsonValue;
    }

    return QJsonValue(object);
}

QHttpMultiPart *DuContainer::toHttpMultiPart(const QByteArray &boundary) const
{
    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    multiPart->setBoundary(boundary);

    QMapIterator<QString, QSharedPointer<DuObject> > i(children);
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

    QMapIterator<QString, QSharedPointer<DuObject> > i(children);
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


QByteArray DuContainer::toDuMusicFile() const
{
    QByteArray retArray;

    QMapIterator<QString, QSharedPointer<DuObject> > i(children);
    while (i.hasNext())
    {
        i.next();
        if (i.value() == NULL)
            return QByteArray();

        const QByteArray &tmpArray = i.value()->toDuMusicFile();
        if (tmpArray.isNull())
            return QByteArray();

        retArray.append(i.value()->toDuMusicFile());
    }

    return retArray;
}


int DuContainer::size() const
{
    int size = 0;

    QMapIterator<QString, QSharedPointer<DuObject> > i(children);
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


QSharedPointer<DuObject> DuContainer::operator[](const QString &label)
{
    if (!children.contains(label))
        return QSharedPointer<DuObject>();

    return children[label];
}


void DuContainer::addChild(const QString &key, DuObject* child)
{
    if (children.value(key, NULL) != NULL)
    {
        delete children.take(key);
    }

    children.insert(key, QSharedPointer<DuObject>(child));
}

QSharedPointer<DuObject> DuContainer::getChild(const QString &key) const
{
    return children.value(key);
}
