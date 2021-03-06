#ifndef DUCONTAINER_H
#define DUCONTAINER_H

#include "duobject.h"
#include <QMap>


#define DU_KEY_ACCESSORS(key, type) \
    type get ## key() const; \
    bool set ## key(const type &value); \
    static const QString Key ## key;

#define DU_KEY_ACCESSORS_IMPL(className, key, dutype, type, defaultReturn) \
    type className::get ## key() const \
    { \
        const Du ## dutype ## ConstPtr &tmp = getChildAs<Du ## dutype>(QStringLiteral(#key)); \
         \
        if (tmp == Q_NULLPTR) \
            return defaultReturn; \
         \
        return static_cast<type>(tmp->get ## dutype()); \
    } \
    \
    bool className::set ## key(const type &value) \
    { \
        const Du ## dutype ## Ptr &tmp = getChildAs<Du ## dutype>(QStringLiteral(#key)); \
         \
        if (tmp == Q_NULLPTR) \
            return false; \
         \
        return tmp->set ## dutype(value); \
    } \
    \
    const QString className::Key ## key = QStringLiteral(#key);

#define DU_KEY_ACCESSORS_TEMPLATE(key, type) \
    DU_KEY_ACCESSORS(key, type)

#define DU_KEY_ACCESSORS_TEMPLATE_IMPL(className, key, dutype, tpltype, type, defaultReturn) \
    type className::get ## key() const \
    { \
        const Du ## dutype ## ConstPtr<tpltype> &tmp = getChildAs<Du ## dutype<tpltype> >(QStringLiteral(#key)); \
         \
        if (tmp == Q_NULLPTR) \
            return defaultReturn; \
         \
        return static_cast<type>(tmp->get ## dutype()); \
    } \
    \
    bool className::set ## key(const type &value) \
    { \
        const Du ## dutype ## Ptr<tpltype> &tmp = getChildAs<Du ## dutype<tpltype> >(QStringLiteral(#key)); \
         \
        if (tmp == Q_NULLPTR) \
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


#define DU_KEY_ACCESSORS_OBJECT_TEMPLATE(key, dutype, tpltype) \
    dutype ## ConstPtr<tpltype> get ## key() const; \
    dutype ## Ptr<tpltype> get ## key(); \
    void set ## key(const dutype ## Ptr<tpltype> &value); \
    static const QString Key ## key;

#define DU_KEY_ACCESSORS_OBJECT_TEMPLATE_IMPL(className, key, dutype, tpltype) \
    dutype ## ConstPtr<tpltype> className::get ## key() const \
    { \
        return getChildAs< dutype<tpltype> >(QStringLiteral(#key)); \
    } \
    \
    void className::set ## key(const dutype ## Ptr<tpltype> &value) \
    { \
        addChild(QStringLiteral(#key), value); \
    } \
    \
    dutype ## Ptr<tpltype> className::get ## key() \
    { \
        return getChildAs< dutype<tpltype> >(QStringLiteral(#key)); \
    } \
    \
    const QString className::Key ## key = QStringLiteral(#key);


#define DU_KEY_ACCESSORS_IN_CHILD(key, type) \
    type get ## key() const; \
    bool set ## key(const type& value); \
    static const QString Key ## key;

#define DU_KEY_ACCESSORS_IN_CHILD_IMPL(className, key, childType, childKey, type, defaultReturn) \
    DU_KEY_ACCESSORS_IN_CHILD_IMPL_DIFF_KEYS(className, key, key, childType, childKey, type, defaultReturn)

#define DU_KEY_ACCESSORS_IN_CHILD_IMPL_DIFF_KEYS(className, key, keyInChild, childType, childKey, type, defaultReturn) \
    type className::get ## key() const \
    { \
        const childType ## ConstPtr& child = get ## childKey(); \
         \
        if (child == Q_NULLPTR) \
            return defaultReturn; \
         \
        return child->get ## keyInChild(); \
    } \
    \
    bool className::set ## key(const type& value) \
    { \
        const childType ## Ptr &child = getChildAs<childType>(QStringLiteral(#childKey)); \
         \
        if (child == Q_NULLPTR) \
            return false; \
         \
        return child->set ## keyInChild(value); \
    } \
    \
    const QString className::Key ## key = QStringLiteral(#key);


DU_OBJECT(DuContainer);

class DuContainer : public DuObject
{
public:
    DuContainer();
    virtual ~DuContainer() = default;

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

protected:
    virtual bool equals(const DuObject &other) const Q_DECL_OVERRIDE;

public:
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

    // STL container functions
    inline QMap<QString, DuObjectPtr>::iterator begin() { return m_children.begin(); }
    inline QMap<QString, DuObjectPtr>::const_iterator begin() const { return m_children.begin(); }
    inline QMap<QString, DuObjectPtr>::const_iterator cbegin() const { return m_children.cbegin(); }
    inline QMap<QString, DuObjectPtr>::iterator end() { return m_children.end(); }
    inline QMap<QString, DuObjectPtr>::const_iterator end() const { return m_children.end(); }
    inline QMap<QString, DuObjectPtr>::const_iterator cend() const { return m_children.cend(); }

private:
    QMap<QString, DuObjectPtr> m_children;
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
