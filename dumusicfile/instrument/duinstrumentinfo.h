#ifndef DUINSTRUMENTINFO_H
#define DUINSTRUMENTINFO_H

#include "../../general/ducontainer.h"

// TODO: Get this from external
#ifndef SOUNDBANK_STARTADRESS
#define SOUNDBANK_STARTADRESS 0x10000
#endif


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

    bool toStruct(s_instr& outStruct) const;
    QByteArray toDuMusicBinary() const;
    QByteArray toBinary(uint32_t sampleAddress, uint8_t nbLayer, int nbSamples, uint32_t sampleSize) const;

    static uint32_t sampleAddressDreamToReadable(uint32_t dreamValue);
    static uint32_t sampleAddressReadableToDream(uint32_t readableValue);

    int size() const;

    DU_KEY_ACCESSORS(Name,               QString)

    DU_KEY_ACCESSORS(DreamProgramChange, int)
    DU_KEY_ACCESSORS(MidiControlChange0, int)
    DU_KEY_ACCESSORS(KeyMapping,         int)
    DU_KEY_ACCESSORS(Octave,             int)

    DU_KEY_ACCESSORS(UserID,             int)
    DU_KEY_ACCESSORS(ID,                 int)

    DU_KEY_ACCESSORS(ActiveNoteOff,      int)

    DU_KEY_ACCESSORS(Category,           QString)

    DU_KEY_ACCESSORS(RelativeVolume,     int)

    DU_KEY_ACCESSORS(DreamFormatId,      DreamFormat)

    DU_KEY_ACCESSORS(InstrType,          INSTRUMENT_TYPE)
    DU_KEY_ACCESSORS(InstrVersion,       int)
    DU_KEY_ACCESSORS(HardInstrVersion,   int)
    DU_KEY_ACCESSORS(SoftInstrVersion,   int)
};

#endif // DUINSTRUMENTINFO_H
