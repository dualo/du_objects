#ifndef DUCONTAINER_H
#define DUCONTAINER_H

#include "duobject.h"
#include <QMap>
#include <QMapIterator>
#include <QList>
#include <QStringList>
#include <QHttpMultiPart>


DU_OBJECT(DuContainer)

class DuContainer : public DuObject
{
public:
    explicit DuContainer();
    virtual ~DuContainer();

protected:
    explicit DuContainer(const DuContainer &other);

public:
    virtual DuObjectPtr clone() const;

    virtual QJsonValue toJson() const;
    virtual QByteArray toDuMusicFile() const;

    virtual QHttpMultiPart* toHttpMultiPart(const QByteArray &boundary) const;
    QDebug debugPrint(QDebug dbg) const;

    virtual int size() const;

    QStringList keys() const;

    DuObjectPtr operator[](const QString &label);

protected:
    void addChild(const QString &key, const DuObjectPtr &child);

    /**
     * @brief Convenience function.
     *
     * The pointer "child" must not be used outside of the call to this function.
     * Usage :
     *     addChild("key", new DuObjectSubclass);
     * Allows to avoid writing :
     *     addChild("key", DuObjectPtr(new DuObjectSubclass));
     *
     * @param key
     * @param child
     */
    void addChild(const QString &key, DuObject* child);

    DuObjectPtr getChild(const QString &key);
    DuObjectConstPtr getChild(const QString &key) const;

    template <class T>
    QSharedPointer<T> getChildAs(const QString &key);
    template <class T>
    QSharedPointer<const T> getChildAs(const QString &key) const;

private:
    QMap<QString, DuObjectPtr> children;
};

template <class T>
inline QSharedPointer<T> DuContainer::getChildAs(const QString &key)
{
    return getChild(key).dynamicCast<T>();
}

template <class T>
inline QSharedPointer<const T> DuContainer::getChildAs(const QString &key) const
{
    return getChild(key).dynamicCast<const T>();
}

#endif // DUCONTAINER_H
