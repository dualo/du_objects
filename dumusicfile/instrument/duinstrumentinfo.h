#ifndef DUINSTRUMENTINFO_H
#define DUINSTRUMENTINFO_H

#include "../../general/ducontainer.h"


DU_OBJECT(DuInstrumentInfo);

class DuInstrumentInfo : public DuContainer
{
public:
    enum DreamFormat {
        NO_FORMAT = 0,
        SDK_3000 = 1,
        SDK_5000 = 2
    };

    explicit DuInstrumentInfo();
    ~DuInstrumentInfo();

    virtual DuObjectPtr clone() const;

    static DuInstrumentInfoPtr fromDuMusicBinary(const s_instr &du_instrInfo);
    static DuInstrumentInfoPtr fromJson(const QJsonObject &jsonInstrInfo);

    QByteArray toDuMusicBinary() const;

    int size() const;

    DU_KEY_ACCESSORS(Name,               QString)

    DU_KEY_ACCESSORS(DreamProgramChange, int)
    DU_KEY_ACCESSORS(MidiControlChange0, int)
    DU_KEY_ACCESSORS(KeyMapping,         int)
    DU_KEY_ACCESSORS(Octave,             int)

    DU_KEY_ACCESSORS(UserID,             int)
    DU_KEY_ACCESSORS(ID,                 int)
    DU_KEY_ACCESSORS(SampleAddress,      int)

    DU_KEY_ACCESSORS(ActiveNoteOff,      int)

    DU_KEY_ACCESSORS(Category,           QString)

    DU_KEY_ACCESSORS(RelativeVolume,     int)

    DU_KEY_ACCESSORS(DreamFormatId,      DreamFormat)
    DU_KEY_ACCESSORS(NbLayer,            int)

    DU_KEY_ACCESSORS(IPSize,             int)
    DU_KEY_ACCESSORS(SPSize,             int)
    DU_KEY_ACCESSORS(SampleSize,         int)

    DU_KEY_ACCESSORS(InstrType,          INSTRUMENT_TYPE)
    DU_KEY_ACCESSORS(InstrVersion,       int)
};

#endif // DUINSTRUMENTINFO_H
