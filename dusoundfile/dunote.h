#ifndef DUNOTE_H
#define DUNOTE_H

#include "../general/ducontainer.h"


DU_OBJECT(DuNote);

class DuNote : public DuContainer
{
public:
    DuNote();
    virtual ~DuNote() = default;

    virtual DuObjectPtr clone() const;
    virtual int size() const;

    static DuNotePtr fromBinary(const s_note& data);

    virtual QByteArray toDuMusicBinary() const;

    DU_KEY_ACCESSORS(NoteGM,        int)
    DU_KEY_ACCESSORS(IsExclusive,   bool)
    DU_KEY_ACCESSORS(ExclusiveNote, int)
    DU_KEY_ACCESSORS(NoteOff,       bool)
    DU_KEY_ACCESSORS(Note,          int)
    DU_KEY_ACCESSORS(Name,          QString)
    DU_KEY_ACCESSORS(CategoryName,  QString)

    static const int drumMapping[];
};

#endif // DUNOTE_H
