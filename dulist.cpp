#include "dulist.h"

#include "general/dunumeric.h"
#include "general/dustring.h"


DU_OBJECT_IMPL(DuList)

DuList::DuList(const QString &name, DuList::Type type) :
    DuContainer()
{
    addChild(KeyName, new DuString(name));
    addChild(KeyType, new DuNumeric(type));
}

DuObjectPtr DuList::clone() const
{
    return DuListPtr(new DuList(*this));
}

bool DuList::hasSameAttributes(const DuListConstPtr &other) const
{
    return this->getName() == other->getName() && this->getType() == other->getType();
}

DU_KEY_ACCESSORS_IMPL(DuList, Name, String, QString, QString())
DU_KEY_ACCESSORS_IMPL(DuList, Type, Numeric, DuList::Type, None)
