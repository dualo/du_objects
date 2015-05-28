#ifndef DUMIDICONTAINER_H
#define DUMIDICONTAINER_H

#include "dumidiobject.h"
#include <QMap>


DU_OBJECT(DuMidiContainer)

class DuMidiContainer : public DuMidiObject
{
public:
    explicit DuMidiContainer();
    virtual ~DuMidiContainer();

protected:
    explicit DuMidiContainer(const DuMidiContainer &other);

public:
    virtual DuObjectPtr clone() const;

    virtual const QByteArray toMidiBinary() const;

    virtual int size() const;

    virtual QDebug debugPrint(QDebug dbg) const;

protected:
    void addChild(const QString &key, const DuMidiObjectPtr &child);

    /**
     * @brief Convenience function.
     *
     * The pointer "child" must not be used outside of the call to this function.
     * Usage :
     *     addChild("key", new DuMidiObjectSubclass);
     * Allows to avoid writing :
     *     addChild("key", DuMidiObjectPtr(new DuMidiObjectSubclass));
     *
     * @param key
     * @param child
     */
    void addChild(const QString &key, DuMidiObject *child);

    DuMidiObjectPtr getChild(const QString &key);
    DuMidiObjectConstPtr getChild(const QString &key) const;

    template <class T>
    QSharedPointer<T> getChildAs(const QString &key);
    template <class T>
    QSharedPointer<const T> getChildAs(const QString &key) const;

private:
    QMap<QString, DuMidiObjectPtr> children;
};

    template <class T>
    inline QSharedPointer<T> DuMidiContainer::getChildAs(const QString &key)
    {
        return getChild(key).dynamicCast<T>();
    }

    template <class T>
    inline QSharedPointer<const T> DuMidiContainer::getChildAs(const QString &key) const
    {
        return getChild(key).dynamicCast<const T>();
    }

#endif // DUMIDICONTAINER_H
