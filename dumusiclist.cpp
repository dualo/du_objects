#include "dumusiclist.h"

#include "general/dunumeric.h"
#include "general/dustring.h"

#include <QDebug>

DU_OBJECT_IMPL(DuMusicList)

DuMusicList::DuMusicList(const QString &name, DuMusicList::Type type) :
    DuContainer()
{
    addChild(KEY_MUSIC_LIST_NAME,   new DuString(name));
    addChild(KEY_MUSIC_LIST_TYPE,   new DuNumeric(type));
}

DuObjectPtr DuMusicList::clone() const
{
    return DuMusicListPtr(new DuMusicList(*this));
}

bool DuMusicList::equals(const DuMusicListConstPtr &other) const
{
    return this->getName() == other->getName() && this->getType() == other->getType();
}

QString DuMusicList::getName() const
{
    const DuStringConstPtr name = getChildAs<DuString>(KEY_MUSIC_LIST_NAME);

    if (name == NULL)
    {
        qCritical() << "Unable to cast" << KEY_MUSIC_LIST_NAME << "to DuString*";
        return QString();
    }

    return name->getString();
}

bool DuMusicList::setName(const QString &value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_MUSIC_LIST_NAME);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}

DuMusicList::Type DuMusicList::getType() const
{
    const DuNumericConstPtr type = getChildAs<DuNumeric>(KEY_MUSIC_LIST_TYPE);

    if (type == NULL)
    {
        qCritical() << "Unable to cast" << KEY_MUSIC_LIST_TYPE << "to DuNumeric*";
        return None;
    }

    return (Type)type->getNumeric();
}

bool DuMusicList::setType(DuMusicList::Type value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_MUSIC_LIST_TYPE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

