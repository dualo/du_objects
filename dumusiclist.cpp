#include "dumusiclist.h"

#include "dunumeric.h"
#include "dustring.h"

#include <QDebug>

DuMusicList::DuMusicList(const QString &name, DuMusicList::Type type) :
    DuContainer()
{
    addChild(KEY_MUSIC_LIST_NAME,   new DuString(name));
    addChild(KEY_MUSIC_LIST_TYPE,   new DuNumeric(type));
}

bool DuMusicList::equals(const DuMusicList *other) const
{
    return this->getName() == other->getName() && this->getType() == other->getType();
}

QString DuMusicList::getName() const
{
    const DuString* name = getChildAs<DuString*>(KEY_MUSIC_LIST_NAME);

    if (name == NULL)
    {
        qCritical() << "Unable to cast" << KEY_MUSIC_LIST_NAME << "to DuString*";
        return QString();
    }

    return name->getString();
}

bool DuMusicList::setName(const QString &value)
{
    DuString *tmp = getChildAs<DuString*>(KEY_MUSIC_LIST_NAME);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

DuMusicList::Type DuMusicList::getType() const
{
    const DuNumeric* type = getChildAs<DuNumeric*>(KEY_MUSIC_LIST_TYPE);

    if (type == NULL)
    {
        qCritical() << "Unable to cast" << KEY_MUSIC_LIST_TYPE << "to DuNumeric*";
        return None;
    }

    return (Type)type->getNumeric();
}

bool DuMusicList::setType(DuMusicList::Type value)
{
    DuNumeric *tmp = getChildAs<DuNumeric*>(KEY_MUSIC_LIST_TYPE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

