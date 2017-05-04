#ifndef DUOBJECT_H
#define DUOBJECT_H

#include <QHttpPart>
#include <QSharedPointer>
#include <QLoggingCategory>

#pragma pack(push, 4)
#include "../du-touch/dualo_structs/music_structs.h"
#pragma pack(pop)

Q_DECLARE_LOGGING_CATEGORY(LOG_CAT_DU_OBJECT)
Q_DECLARE_LOGGING_CATEGORY(LOG_CAT_MIDI)


#define DU_OBJECT(name) \
    class name; \
    typedef QSharedPointer<name> name ## Ptr; \
    typedef QSharedPointer<const name> name ## ConstPtr; \
    QDebug operator<<(QDebug dbg, const name ## ConstPtr& obj); \
    QDebug operator<<(QDebug dbg, const name ## Ptr& obj)

#define DU_OBJECT_IMPL(name) \
    QDebug operator<<(QDebug dbg, const name ## ConstPtr& obj) \
    { \
        if (obj.isNull()) \
            return dbg << #name "(0x0) "; \
        return obj->debugPrint(dbg); \
    } \
    QDebug operator<<(QDebug dbg, const name ## Ptr& obj) \
    { \
        if (obj.isNull()) \
            return dbg << #name "(0x0) "; \
        return obj->debugPrint(dbg); \
    }

#define DU_OBJECT_TEMPLATE(name) \
    template <class T> \
    class name; \
    template <class T> \
    using name ## Ptr = QSharedPointer< name<T> >; \
    template <class T> \
    using name ## ConstPtr = QSharedPointer< const name<T> >; \
    template <class T> \
    QDebug operator<<(QDebug dbg, const name ## ConstPtr<T>& obj); \
    template <class T> \
    QDebug operator<<(QDebug dbg, const name ## Ptr<T>& obj)

#define DU_OBJEC_TEMPLATE_IMPL(name) \
    template <class T> \
    QDebug operator<<(QDebug dbg, const name ## ConstPtr<T>& obj) \
    { \
        if (obj.isNull()) \
            return dbg << #name "(0x0) "; \
        return obj->debugPrint(dbg); \
    } \
    template <class T> \
    QDebug operator<<(QDebug dbg, const name ## Ptr<T>& obj) \
    { \
        if (obj.isNull()) \
            return dbg << #name "(0x0) "; \
        return obj->debugPrint(dbg); \
    }

DU_OBJECT(DuObject);

class DuObject
{
public:
    DuObject();
    virtual ~DuObject() = default;

protected:
    explicit DuObject(const DuObject &other);

public:
    virtual DuObjectPtr clone() const = 0;
    template <class T>
    QSharedPointer<T> cloneAs() const;

    virtual QByteArray toDuMusicBinary() const = 0;
    virtual QByteArray toMidiBinary() const;
    virtual QJsonValue toJson() const = 0;
    virtual QHttpPart toHttpPart(const QString &name) const;

    virtual bool parseJson(const QJsonValue &jsonValue) = 0;

    virtual QDebug debugPrint(QDebug dbg) const;

    virtual int size() const = 0;

    friend bool operator==(const DuObject& lhs, const DuObject& rhs);
    friend bool operator!=(const DuObject& lhs, const DuObject& rhs);

protected:
    virtual bool equals(const DuObject& other) const;
};

template <class T>
inline QSharedPointer<T> DuObject::cloneAs() const
{
    return clone().dynamicCast<T>();
}

#endif // DUOBJECT_H
