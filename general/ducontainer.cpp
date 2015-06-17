#include "ducontainer.h"

#include <QMapIterator>
#include <QJsonObject>


DU_OBJECT_IMPL(DuContainer)

DuContainer::DuContainer() :
    DuObject()
{
}

DuContainer::~DuContainer()
{
}

DuContainer::DuContainer(const DuContainer &other) :
    DuObject(other)
{
    QMapIterator<QString, DuObjectPtr> i(other.children);
    while (i.hasNext())
    {
        i.next();
        children.insert(i.key(), i.value()->clone());
    }
}

DuObjectPtr DuContainer::clone() const
{
    return DuContainerPtr(new DuContainer(*this));
}


QByteArray DuContainer::toDuMusicBinary() const
{
    QByteArray retArray;

    QMapIterator<QString, DuObjectPtr> i(children);
    while (i.hasNext())
    {
        i.next();
        if (i.value() == NULL)
        {
            qCWarning(LOG_CAT_DU_OBJECT) << "DuContainer::toDuMusicBinary():\n"
                       << "element was null";

            return QByteArray();
        }

        const QByteArray &tmpArray = i.value()->toDuMusicBinary();
        if (tmpArray.isNull())
        {
            qCWarning(LOG_CAT_DU_OBJECT) << "DuContainer::toDuMusicBinary():\n"
                       << "element binary data was null";

            return QByteArray();
        }

        retArray.append(tmpArray);
    }

    return retArray;
}


QJsonValue DuContainer::toJson() const
{
    QJsonObject object;

    QMapIterator<QString, DuObjectPtr> i(children);
    while (i.hasNext())
    {
        i.next();
        if (i.value() == NULL)
        {
            qCWarning(LOG_CAT_DU_OBJECT) << "DuContainer::toJson():\n"
                       << "element was null";

            return QJsonValue(QJsonValue::Undefined);
        }

        const QJsonValue &jsonValue = i.value()->toJson();
        if (jsonValue.isUndefined())
        {
            qCWarning(LOG_CAT_DU_OBJECT) << "DuContainer::toJson():\n"
                       << "element json value was undefined";

            return QJsonValue(QJsonValue::Undefined);
        }

        object[i.key()] = jsonValue;
    }

    return QJsonValue(object);
}


QHttpMultiPart *DuContainer::toHttpMultiPart(const QByteArray &boundary) const
{
    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    multiPart->setBoundary(boundary);

    QMapIterator<QString, DuObjectPtr> i(children);
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

    QMapIterator<QString, DuObjectPtr> i(children);
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

    QMapIterator<QString, DuObjectPtr> i(children);
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


DuObjectPtr DuContainer::operator[](const QString &key)
{
    if (!children.contains(key))
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuContainer::operator[]:\n"
                   << key << "was not contained\n"
                   << "default constructed value returned";

        return DuObjectPtr();
    }

    return children[key];
}

void DuContainer::addChild(const QString &key, const DuObjectPtr &child)
{
    children.insert(key, child);
}

void DuContainer::addChild(const QString &key, DuObject *child)
{
    addChild(key, DuObjectPtr(child));
}

DuObjectPtr DuContainer::getChild(const QString &key)
{
    return children.value(key);
}

DuObjectConstPtr DuContainer::getChild(const QString &key) const
{
    return children.value(key);
}
