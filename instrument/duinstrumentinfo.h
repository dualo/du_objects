#ifndef DUINSTRUMENTINFO_H
#define DUINSTRUMENTINFO_H

#include "../general/ducontainer.h"

// TODO: Get this from external
#ifndef SOUNDBANK_STARTADRESS
#define SOUNDBANK_STARTADRESS 0x10000
#endif


DU_OBJECT(DuInstrumentInfo);

class DuInstrumentInfo : public DuContainer
{
public:
    enum DreamFormat {
        FORMAT_ERROR = -1,
        SDK_3000 = 0,
        SDK_5000 = 1,
    };

    struct Category
    {
        int programChange;
        QString name;
        int type;
    };

    static QList<Category> categoryMap;
    static QString getCategoryNameFromProgramChange(int programChange);
    static int getProgramChangeFromCategoryName(const QString name);
    static QStringList getCategoriesFromType(int type);


    DuInstrumentInfo();
    virtual ~DuInstrumentInfo() = default;

    virtual DuObjectPtr clone() const;

    static DuInstrumentInfoPtr fromDuMusicBinary(const info_instr &du_instrInfo);

    bool toStruct(info_instr& outStruct, bool forDuTouchSOrL) const;
    QByteArray toDuMusicBinary() const;
    QByteArray toBinary(uint8_t nbLayer, int nbSamples, uint32_t sampleSize, bool forDuTouchSOrL) const;

    int size() const;

    DU_KEY_ACCESSORS(NameForDevice,      QString)

    DU_KEY_ACCESSORS(KeyMapping,         int)
    DU_KEY_ACCESSORS(Octave,             int)

    DU_KEY_ACCESSORS(UserID,             int)
    DU_KEY_ACCESSORS(ID,                 int)

    DU_KEY_ACCESSORS(ActiveNoteOff,      bool)

    DU_KEY_ACCESSORS(Category,           QString)

    DU_KEY_ACCESSORS(RelativeVolume,     int)

    DU_KEY_ACCESSORS(InstrType,          INSTRUMENT_TYPE)
    DU_KEY_ACCESSORS(InstrVersion,       int)
    DU_KEY_ACCESSORS(HardInstrVersion,   int)
    DU_KEY_ACCESSORS(SoftInstrVersion,   int)
};

#endif // DUINSTRUMENTINFO_H
