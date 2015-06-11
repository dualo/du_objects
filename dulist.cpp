#include "dulist.h"

#include "general/dunumeric.h"
#include "general/dustring.h"

#include <QDebug>

DU_OBJECT_IMPL(DuList)

DuList::DuList(const QString &name, DuList::Type type) :
    DuContainer()
{
    addChild(KEY_LIST_NAME,   new DuString(name));
    addChild(KEY_LIST_TYPE,   new DuNumeric(type));
}

DuObjectPtr DuList::clone() const
{
    return DuListPtr(new DuList(*this));
}

bool DuList::equals(const DuListConstPtr &other) const
{
    return this->getName() == other->getName() && this->getType() == other->getType();
}

QString DuList::getName() const
{
    const DuStringConstPtr name = getChildAs<DuString>(KEY_LIST_NAME);

    if (name == NULL)
    {
        qCritical() << "Unable to cast" << KEY_LIST_NAME << "to DuString*";
        return QString();
    }

    return name->getString();
}

bool DuList::setName(const QString &value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_LIST_NAME);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

DuList::Type DuList::getType() const
{
    const DuNumericConstPtr type = getChildAs<DuNumeric>(KEY_LIST_TYPE);

    if (type == NULL)
    {
        qCritical() << "Unable to cast" << KEY_LIST_TYPE << "to DuNumeric*";
        return None;
    }

    return (Type)type->getNumeric();
}

bool DuList::setType(DuList::Type value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_LIST_TYPE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

