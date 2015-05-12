#ifndef DUINSTRUMENTINFO_H
#define DUINSTRUMENTINFO_H

#include "ducontainer.h"
#include "dustring.h"
#include "dunumeric.h"


#define KEY_INSTRINFO_CATEGORY              "Category"
#define KEY_INSTRINFO_NAME                  "Name"
#define KEY_INSTRINFO_ID                    "ID"
#define KEY_INSTRINFO_USERID                "UserID"
#define KEY_INSTRINFO_MIDIPROGRAMCHANGE     "MidiProgramChange"
#define KEY_INSTRINFO_MIDICONTROLCHANGE0    "MidiControlChange0"
#define KEY_INSTRINFO_ACTIVENOTEOFF         "ActiveNoteOff"
#define KEY_INSTRINFO_RELVOLUME             "RelativeVolume"


class DuInstrumentInfo : public DuContainer
{
public:
    explicit DuInstrumentInfo();
    ~DuInstrumentInfo();

    static DuInstrumentInfo *fromDuMusicFile(const s_instr &du_instrInfo);

    static DuInstrumentInfo *fromJson(const QJsonObject &jsonInstrInfo);

    QString getCategory() const;
    void setCategory(const QString &value);

    QString getName() const;
    void setName(const QString &value);

    int getID() const;
    void setID(int value);

    QString getUserID() const;
    void setUserID(const QString &value);

    int getMidiProgramChange() const;
    void setMidiProgramChange(int value);

    int getMidiControlChange0() const;
    void setMidiControlChange0(int value);

    int getActiveNoteOff() const;
    void setActiveNoteOff(int value);

    int getRelativeVolume() const;
    void setRelativeVolume(int value);
};

#endif // DUINSTRUMENTINFO_H
