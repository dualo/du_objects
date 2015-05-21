#ifndef DUMUSICLIST_H
#define DUMUSICLIST_H

#include "ducontainer.h"

#include <QSharedPointer>

#define KEY_MUSIC_LIST_NAME "Name"
#define KEY_MUSIC_LIST_TYPE "Type"

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

    virtual bool equals(const DuMusicList* other) const;

    QString getName() const;
    bool setName(const QString& value);

    Type getType() const;
    virtual bool setType(Type value);
};

Q_DECLARE_METATYPE(DuMusicList*)
Q_DECLARE_METATYPE(QSharedPointer<DuMusicList>)

#endif // DUMUSICLIST_H
