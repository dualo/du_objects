#ifndef DUCONTAINER_H
#define DUCONTAINER_H

#include "duobject.h"
#include <QMap>
#include <QStringList>
#include <QHttpMultiPart>


#define DU_KEY_ACCESSORS(key, type) \
    type get ## key() const; \
    bool set ## key(const type &value); \
    static const QString Key ## key;

#define DU_KEY_ACCESSORS_IMPL(className, key, dutype, type, defaultReturn) \
    type className::get ## key() const \
    { \
        const Du ## dutype ## ConstPtr &tmp = getChildAs<Du ## dutype>(QStringLiteral(#key)); \
         \
        if (tmp == NULL) \
            return defaultReturn; \
         \
        return static_cast<type>(tmp->get ## dutype()); \
    } \
    \
    bool className::set ## key(const type &value) \
    { \
        const Du ## dutype ## Ptr &tmp = getChildAs<Du ## dutype>(QStringLiteral(#key)); \
         \
        if (tmp == NULL) \
            return false; \
         \
        return tmp->set ## dutype(value); \
    } \
    \
    const QString className::Key ## key = QStringLiteral(#key);

#define DU_KEY_ACCESSORS_OBJECT(key, dutype) \
    dutype ## ConstPtr get ## key() const; \
    dutype ## Ptr get ## key(); \
    void set ## key(const dutype ## Ptr &value); \
    static const QString Key ## key;

#define DU_KEY_ACCESSORS_OBJECT_IMPL(className, key, dutype) \
    dutype ## ConstPtr className::get ## key() const \
    { \
        return getChildAs<dutype>(QStringLiteral(#key)); \
    } \
    \
    void className::set ## key(const dutype ## Ptr &value) \
    { \
        addChild(QStringLiteral(#key), value); \
    } \
    \
    dutype ## Ptr className::get ## key() \
    { \
        return getChildAs<dutype>(QStringLiteral(#key)); \
    } \
    \
    const QString className::Key ## key = QStringLiteral(#key);

#define DU_KEY_ACCESSORS_IN_CHILD(key, type) \
    type get ## key() const; \
    bool set ## key(const type& value); \
    static const QString Key ## key;

#define DU_KEY_ACCESSORS_IN_CHILD_IMPL(className, key, childType, childKey, type, defaultReturn) \
    type className::get ## key() const \
    { \
        const childType ## ConstPtr& child = get ## childKey(); \
         \
        if (child == NULL) \
            return defaultReturn; \
         \
        return child->get ## key(); \
    } \
    \
    bool className::set ## key(const type& value) \
    { \
        const childType ## Ptr &child = getChildAs<childType>(QStringLiteral(#childKey)); \
         \
        if (child == NULL) \
            return false; \
         \
        return child->set ## key(value); \
    } \
    \
    const QString className::Key ## key = QStringLiteral(#key);


DU_OBJECT(DuContainer)

class DuContainer : public DuObject
{
public:
    explicit DuContainer();
    virtual ~DuContainer();

protected:
    explicit DuContainer(const DuContainer &other);

public:
    virtual DuObjectPtr clone() const override;

    virtual QByteArray toDuMusicBinary() const override;
    virtual QJsonValue toJson() const override;
    virtual QHttpMultiPart* toHttpMultiPart(const QByteArray &boundary) const;

    virtual bool parseJson(const QJsonValue &jsonValue) override;

    QDebug debugPrint(QDebug dbg) const override;

    virtual int size() const override;

    QStringList keys() const;

    DuObjectPtr operator[](const QString &key);

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

public:
    virtual DuObjectPtr getChild(const QString &key);
    virtual DuObjectConstPtr getChild(const QString &key) const;

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
