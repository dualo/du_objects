#ifndef DUOBJECT_H
#define DUOBJECT_H

#include <QHttpPart>
#include <QJsonValue>
#include <QSharedPointer>

#pragma pack(push, 4)
#include "../du-touch/parameters/music_parameters_mng.h"
#pragma pack(pop)


#define DU_OBJECT(name) \
    class name; \
    typedef QSharedPointer<name> name ## Ptr; \
    typedef QSharedPointer<const name> name ## ConstPtr; \
    QDebug operator<<(QDebug dbg, const name ## ConstPtr& obj); \
    QDebug operator<<(QDebug dbg, const name ## Ptr& obj);

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


DU_OBJECT(DuObject)

class DuObject
{
public:
    explicit DuObject();
    virtual ~DuObject();

protected:
    explicit DuObject(const DuObject &other);

public:
    virtual DuObjectPtr clone() const = 0;

    virtual QByteArray toDuMusicBinary() const = 0;
    virtual QByteArray toMidiBinary() const;
    virtual QJsonValue toJson() const = 0;

    virtual QHttpPart toHttpPart(const QString &name) const;
    virtual QDebug debugPrint(QDebug dbg) const;

    virtual int size() const = 0;
};

#endif // DUOBJECT_H
