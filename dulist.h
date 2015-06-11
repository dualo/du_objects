#ifndef DULIST_H
#define DULIST_H

#include "general/ducontainer.h"

#define KEY_LIST_NAME "Name"
#define KEY_LIST_TYPE "Type"


DU_OBJECT(DuList)

class DuList : public DuContainer
{
public:
    enum Type
    {
        None = 0,
        Custom,
        All,
        Device,
        Shared,
        Trash,
        Lessons,
        Shop

        // Keep in sync with DuListObject::Type
    };

    DuList(const QString& name = "", Type type = None);

    virtual DuObjectPtr clone() const;

    virtual bool equals(const DuListConstPtr& other) const;

    QString getName() const;
    bool setName(const QString& value);

    Type getType() const;
    virtual bool setType(Type value);
};

Q_DECLARE_METATYPE(DuListPtr)
Q_DECLARE_METATYPE(DuListConstPtr)

#endif // DULIST_H
