#include "dunote.h"

#include "../general/duboolean.h"
#include "../general/dunumeric.h"
#include "../general/dustring.h"

#include <cstring>

DU_OBJECT_IMPL(DuNote);

DuNote::DuNote() :
    DuContainer()
{
    addChild(KeyNoteGM,         new DuNumeric(0, NUMERIC_DEFAULT_SIZE, 0x7F, 0x00));
    addChild(KeyIsExclusive,    new DuBoolean(false));
    addChild(KeyExclusiveNote,  new DuNumeric(0, NUMERIC_DEFAULT_SIZE, 0x7F, 0x00));
    addChild(KeyNoteOff,        new DuBoolean(false));
    addChild(KeyNote,           new DuNumeric(1, NUMERIC_DEFAULT_SIZE, NUM_BUTTON_KEYBOARD * 2, 1));
    addChild(KeyName,           new DuString(NOTE_NAME_CARACT));
    addChild(KeyCategoryName,   new DuString(NAME_CARACT));
}

DuObjectPtr DuNote::clone() const
{
    return DuNotePtr(new DuNote(*this));
}

int DuNote::size() const
{
    return S_NOTE_SIZE;
}

DuNotePtr DuNote::fromBinary(const s_note &data)
{
    DuNotePtr note(new DuNote);
    bool verif = true;

    verif = note->setNoteGM(data.note_gmref) ? verif : false;
    if (data.note_excl == 0xFF)
    {
        verif = note->setIsExclusive(false) ? verif : false;
    }
    else
    {
        verif = note->setIsExclusive(true) ? verif : false;
        verif = note->setExclusiveNote(data.note_excl) ? verif : false;
    }
    verif = note->setNoteOff(data.note_off == 1) ? verif : false;
    verif = note->setNote(data.note_key) ? verif : false;
    verif = note->setName(QString(QByteArray((char *)data.note_name, NOTE_NAME_CARACT))) ? verif : false;
    verif = note->setCategoryName(QString(QByteArray((char *)data.cat_name, NAME_CARACT))) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "an attribute was not properly set";
    }

    return note;
}

QByteArray DuNote::toDuMusicBinary() const
{
    s_note data;
    std::memset((char*)&data, 0, size());

    QString tmpStr;
    int tmpNum = 0;

    tmpNum = getNoteGM();
    if (tmpNum == -1)
        return QByteArray();
    data.note_gmref = tmpNum;

    if (getIsExclusive())
    {
        tmpNum = getExclusiveNote();
        if (tmpNum == -1)
            return QByteArray();
        data.note_excl = tmpNum;
    }
    else
    {
        data.note_excl = 0xFF;
    }

    tmpNum = getNoteOff() ? 1 : 0;
    if (tmpNum == -1)
        return QByteArray();
    data.note_off = tmpNum;

    tmpNum = getNote();
    if (tmpNum == -1)
        return QByteArray();
    data.note_key = tmpNum;

    QByteArray tmpName(NOTE_NAME_CARACT, (char)0x00);
    tmpStr = getName();
    if (tmpStr.isNull())
        return QByteArray();
    tmpName.prepend(tmpStr.toUtf8());

    std::memcpy(data.note_name, tmpName.data(), NOTE_NAME_CARACT);

    QByteArray tmpCatName(NAME_CARACT, (char)0x00);
    tmpStr = getCategoryName();
    if (tmpStr.isNull())
        return QByteArray();
    tmpCatName.prepend(tmpStr.toUtf8());

    std::memcpy(data.cat_name, tmpCatName.data(), NAME_CARACT);

    return QByteArray((char *)&data, size());
}

DU_KEY_ACCESSORS_IMPL(DuNote, NoteGM,           Numeric, int,   -1)
DU_KEY_ACCESSORS_IMPL(DuNote, IsExclusive,      Boolean, bool,  false)
DU_KEY_ACCESSORS_IMPL(DuNote, ExclusiveNote,    Numeric, int,   -1)
DU_KEY_ACCESSORS_IMPL(DuNote, NoteOff,          Boolean, bool,  false)
DU_KEY_ACCESSORS_IMPL(DuNote, Note,             Numeric, int,   -1)
DU_KEY_ACCESSORS_IMPL(DuNote, Name,             String,  QString, QString())
DU_KEY_ACCESSORS_IMPL(DuNote, CategoryName,     String,  QString, QString())

const int DuNote::drumMapping[] = {
    // Right
     1,  2,  3,
   4,  5,  6,  7,
     8,  9, 10,
  21, 22, 23, 24,
    11, 12, 13,
  14, 15, 16, 17,
    18, 19, 20,
    25, 26, 27,
  28, 29, 30, 31,
    32, 33, 34,
  45, 46, 47, 48,
    35, 36, 37,
  38, 39, 40, 41,
    42, 43, 44,
    49, 50, 51,
  52, 53, 54, 55,
    56, 57, 58,

    // Left
     1,  2,  3,
   4,  5,  6,  7,
     8,  9, 10,
    11, 12, 13,
  14, 15, 16, 17,
    18, 19, 20,
  21, 22, 23, 24,
    25, 26, 27,
  28, 29, 30, 31,
    32, 33, 34,
    35, 36, 37,
  38, 39, 40, 41,
    42, 43, 44,
  45, 46, 47, 48,
    49, 50, 51,
  52, 53, 54, 55,
    56, 57, 58
};
