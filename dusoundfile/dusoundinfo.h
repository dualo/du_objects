#ifndef DUSOUNDINFO_H
#define DUSOUNDINFO_H

#include "../general/ducontainer.h"


#ifdef Q_OS_WIN
#include "../general/duarray.h"
#else
DU_OBJECT_TEMPLATE(DuArray);
#endif
DU_OBJECT(DuInstrumentInfo);
DU_OBJECT(DuPreset);

DU_OBJECT(DuSoundInfo);

class DuSoundInfo : public DuContainer
{
public:
    DuSoundInfo();

    virtual DuObjectPtr clone() const override;
    virtual int size() const override;

    static DuSoundInfoPtr fromBinary(const sound_instr& data);

    QByteArray toBinary(uint8_t nbLayer, int nbSamples, uint32_t sampleSize) const;

    virtual DuObjectPtr getChild(const QString &key) override;
    virtual DuObjectConstPtr getChild(const QString &key) const override;

public:
    DU_KEY_ACCESSORS_IN_CHILD(NameForDevice,        QString)
    DU_KEY_ACCESSORS_IN_CHILD(KeyMapping,           int)
    DU_KEY_ACCESSORS_IN_CHILD(Octave,               int)
    DU_KEY_ACCESSORS_IN_CHILD(UserID,               int)
    DU_KEY_ACCESSORS_IN_CHILD(ID,                   int)
    DU_KEY_ACCESSORS_IN_CHILD(ActiveNoteOff,        int)
    DU_KEY_ACCESSORS_IN_CHILD(Category,             QString)
    DU_KEY_ACCESSORS_IN_CHILD(RelativeVolume,       int)
    DU_KEY_ACCESSORS_IN_CHILD(InstrType,            INSTRUMENT_TYPE)
    DU_KEY_ACCESSORS_IN_CHILD(InstrVersion,         int)
    DU_KEY_ACCESSORS_IN_CHILD(HardInstrVersion,     int)
    DU_KEY_ACCESSORS_IN_CHILD(SoftInstrVersion,     int)

    DU_KEY_ACCESSORS_OBJECT(InstrumentInfo,  DuInstrumentInfo)

    DU_KEY_ACCESSORS(PresetNum,  int) // editable

    DU_KEY_ACCESSORS(Name,       QString)

    DU_KEY_ACCESSORS_OBJECT_TEMPLATE(PresetArray, DuArray, DuPreset)
};

#endif // DUSOUNDINFO_H
