#ifndef DUINSTRUMENTINFO_H
#define DUINSTRUMENTINFO_H

#include "../../general/ducontainer.h"
#include "../../general/dustring.h"
#include "../../general/dunumeric.h"


#define KEY_INSTRINFO_NAME                  "Name"
#define KEY_INSTRINFO_MIDIPROGRAMCHANGE     "MidiProgramChange"
#define KEY_INSTRINFO_MIDICONTROLCHANGE0    "MidiControlChange0"
#define KEY_INSTRINFO_KEYMAP                "KepMapping"
#define KEY_INSTRINFO_OCTAVE                "Octave"
#define KEY_INSTRINFO_ID                    "ID"
#define KEY_INSTRINFO_ACTIVENOTEOFF         "ActiveNoteOff"
#define KEY_INSTRINFO_CATEGORY              "Category"
#define KEY_INSTRINFO_RELVOLUME             "RelativeVolume"
#define KEY_INSTRINFO_TYPE                  "Type"
#define KEY_INSTRINFO_USERID                "UserID"

//TODO: add parameters for du-sounds when possible


DU_OBJECT(DuInstrumentInfo)

class DuInstrumentInfo : public DuContainer
{
public:
    explicit DuInstrumentInfo();
    ~DuInstrumentInfo();

    virtual DuObjectPtr clone() const;

    static DuInstrumentInfoPtr fromDuMusicBinary(const s_instr &du_instrInfo);
    static DuInstrumentInfoPtr fromJson(const QJsonObject &jsonInstrInfo);

    QByteArray toDuMusicBinary() const;

    int size() const;

    QString getName() const;
    bool setName(const QString &value);

    int getMidiProgramChange() const;
    bool setMidiProgramChange(int value);

    int getMidiControlChange0() const;
    bool setMidiControlChange0(int value);

    int getKeyMap() const;
    bool setKeyMap(int value);

    int getOctave() const;
    bool setOctave(int value);

    int getID() const;
    bool setID(int value);

    int getActiveNoteOff() const;
    bool setActiveNoteOff(int value);

    QString getCategory() const;
    bool setCategory(const QString &value);

    int getRelativeVolume() const;
    bool setRelativeVolume(int value);

    int getType() const;
    bool setType(int value);

    QString getUserID() const;
    bool setUserID(const QString &value);
};

#endif // DUINSTRUMENTINFO_H
