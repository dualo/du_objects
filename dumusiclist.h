#ifndef DUMUSICLIST_H
#define DUMUSICLIST_H

#include "ducontainer.h"

#define KEY_MUSIC_LIST_NAME "Name"
#define KEY_MUSIC_LIST_TYPE "Type"


DU_OBJECT(DuMusicList)

class DuMusicList : public DuContainer
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

        // Keep in sync with DuMusicListObject::Type
    };

    DuMusicList(const QString& name = "", Type type = None);

    virtual bool equals(const DuMusicListConstPtr& other) const;

    QString getName() const;
    bool setName(const QString& value);

    Type getType() const;
    virtual bool setType(Type value);
};

Q_DECLARE_METATYPE(DuMusicListPtr)
Q_DECLARE_METATYPE(DuMusicListConstPtr)

#endif // DUMUSICLIST_H
