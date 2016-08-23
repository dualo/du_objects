#ifndef DULIST_H
#define DULIST_H

#include "general/ducontainer.h"


DU_OBJECT(DuList);

class DuList : public DuContainer
{
public:
    enum Type
    {
        None = 0,
        Custom,
        All,
        Official,
        Device,
        Shared,
        Trash,
        Lessons,
        Shop

        // Keep in sync with DuListObject::Type
    };

    explicit DuList(const QString& name = QString(), Type type = None);
    virtual ~DuList() = default;

    virtual DuObjectPtr clone() const;

    virtual bool equals(const DuListConstPtr& other) const;

    DU_KEY_ACCESSORS(Name, QString)
    DU_KEY_ACCESSORS(Type, Type)
};

Q_DECLARE_METATYPE(DuListPtr)
Q_DECLARE_METATYPE(DuListConstPtr)

#endif // DULIST_H
