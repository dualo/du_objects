#ifndef DUINSTRUMENTINFO_H
#define DUINSTRUMENTINFO_H

#include "../general/ducontainer.h"

// TODO: Get this from external
#ifndef SOUNDBANK_STARTADRESS
#define SOUNDBANK_STARTADRESS 0x10000
#endif


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

    //TODO: add key for GM program change when possible
    //TODO: add parameters for du-sounds when possible

    DU_KEY_ACCESSORS(Name,               QString)

    DU_KEY_ACCESSORS(DreamProgramChange, int)
    DU_KEY_ACCESSORS(MidiControlChange0, int)
    DU_KEY_ACCESSORS(KeyMapping,         int)
    DU_KEY_ACCESSORS(Octave,             int)

    DU_KEY_ACCESSORS(UserID,             QString)
    DU_KEY_ACCESSORS(ID,                 int)

    DU_KEY_ACCESSORS(ActiveNoteOff,      int)

    DU_KEY_ACCESSORS(Category,           QString)

    DU_KEY_ACCESSORS(RelativeVolume,     int)

    DU_KEY_ACCESSORS(Type,               int)
};

#endif // DUINSTRUMENTINFO_H
