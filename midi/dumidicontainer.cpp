#include "dumidicontainer.h"

#include <QMapIterator>
#include <QDebug>

DU_OBJECT_IMPL(DuMidiContainer)


DuMidiContainer::DuMidiContainer() :
    DuMidiObject()
{
}

DuMidiContainer::~DuMidiContainer()
{
}

DuMidiContainer::DuMidiContainer(const DuMidiContainer &other) :
    DuMidiObject(other)
{
    QMapIterator<QString, DuMidiObjectPtr> i(other.children);
    while (i.hasNext())
    {
        i.next();
        children.insert(i.key(), i.value()->clone().dynamicCast<DuMidiObject>());
    }
}


DuObjectPtr DuMidiContainer::clone() const
{
    return DuMidiContainerPtr(new DuMidiContainer(*this));
}


const QByteArray DuMidiContainer::toMidiBinary() const
{
    QByteArray retArray;

    QMapIterator<QString, DuMidiObjectPtr> i(children);
    while (i.hasNext())
    {
        i.next();
        if (i.value() == NULL)
            return QByteArray();

        const QByteArray &tmpArray = i.value()->toMidiBinary();
        if (tmpArray.isNull())
            return QByteArray();

        retArray.append(tmpArray);
    }

    return retArray;
}


int DuMidiContainer::size() const
{
    int size = 0;

    QMapIterator<QString, DuMidiObjectPtr> i(children);
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


QDebug DuMidiContainer::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuMidiContainer(";

    QMapIterator<QString, DuMidiObjectPtr> i(children);
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


void DuMidiContainer::addChild(const QString &key, const DuMidiObjectPtr &child)
{
    children.insert(key, child);
}

void DuMidiContainer::addChild(const QString &key, DuMidiObject *child)
{
    addChild(key, DuMidiObjectPtr(child));
}

DuMidiObjectPtr DuMidiContainer::getChild(const QString &key)
{
    return children.value(key);
}

DuMidiObjectConstPtr DuMidiContainer::getChild(const QString &key) const
{
    return children.value(key);
}
