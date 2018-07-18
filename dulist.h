#ifndef DULIST_H
#define DULIST_H

#include "general/ducontainer.h"


DU_OBJECT(DuList);

class DuList : public DuContainer
{
    Q_GADGET

public:
    enum Type
    {
        None = 0,
        Custom,
        All,
        Official,
        Device,
        Shared,
        Games,
        Trash,
        Shop
    };
    Q_ENUM(Type)

    explicit DuList(const QString& name = QString(), Type type = None);
    virtual ~DuList() = default;

    virtual DuObjectPtr clone() const Q_DECL_OVERRIDE;

    virtual bool hasSameAttributes(const DuListConstPtr& other) const;

    DU_KEY_ACCESSORS(Name, QString)
    DU_KEY_ACCESSORS(Type, Type)
};

Q_DECLARE_METATYPE(DuListPtr)
Q_DECLARE_METATYPE(DuListConstPtr)

#endif // DULIST_H
