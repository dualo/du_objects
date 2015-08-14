#include "dunote.h"

#include "../general/dunumeric.h"
#include "../general/dustring.h"

DuNote::DuNote() :
    DuContainer()
{
    addChild(KeyNoteGM,       new DuNumeric(0, NUMERIC_DEFAULT_SIZE, 0x7F, 0x00));
    addChild(KeyExclusive,    new DuNumeric(0, NUMERIC_DEFAULT_SIZE, 0x7F, 0x00));
    addChild(KeyNoteOff,      new DuNumeric(0, NUMERIC_DEFAULT_SIZE, 0x01, 0x00));
    addChild(KeyNote,         new DuNumeric(0, NUMERIC_DEFAULT_SIZE, (NUM_BUTTON_KEYBOARD * 2) - 1, 0x00));
    addChild(KeyName,         new DuString(NOTE_NAME_CARACT));
    addChild(KeyCategoryName, new DuString(NAME_CARACT));
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
    verif = note->setExclusive(data.note_excl) ? verif : false;
    verif = note->setNoteOff(data.note_off) ? verif : false;
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

    QString tmpStr;
    int tmpNum = 0;

    tmpNum = getNoteGM();
    if (tmpNum == -1)
        return QByteArray();
    data.note_gmref = tmpNum;

    tmpNum = getExclusive();
    if (tmpNum == -1)
        return QByteArray();
    data.note_excl = tmpNum;

    tmpNum = getNoteOff();
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

DU_KEY_ACCESSORS_IMPL(DuNote, NoteGM,       Numeric, int,    -1)
DU_KEY_ACCESSORS_IMPL(DuNote, Exclusive,    Numeric, int,    -1)
DU_KEY_ACCESSORS_IMPL(DuNote, NoteOff,      Numeric, int,    -1)
DU_KEY_ACCESSORS_IMPL(DuNote, Note,         Numeric, int,    -1)
DU_KEY_ACCESSORS_IMPL(DuNote, Name,         String,  QString, QString())
DU_KEY_ACCESSORS_IMPL(DuNote, CategoryName, String,  QString, QString())
