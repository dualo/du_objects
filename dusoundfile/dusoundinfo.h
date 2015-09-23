#ifndef DUSOUNDINFO_H
#define DUSOUNDINFO_H

#include "../general/ducontainer.h"


DU_OBJECT(DuMixer);
DU_OBJECT(DuArray);
DU_OBJECT(DuInstrumentInfo);

DU_OBJECT(DuSoundInfo);

class DuSoundInfo : public DuContainer
{
public:
    DuSoundInfo();

    virtual DuObjectPtr clone() const;
    virtual int size() const;

    static DuSoundInfoPtr fromBinary(const struct_instr& data);

    QByteArray toBinary() const;

    DU_KEY_ACCESSORS_IN_CHILD(Name,                 QString)
    DU_KEY_ACCESSORS_IN_CHILD(DreamProgramChange,   int)
    DU_KEY_ACCESSORS_IN_CHILD(Octave,               int)
    DU_KEY_ACCESSORS_IN_CHILD(ActiveNoteOff,        int)
    DU_KEY_ACCESSORS_IN_CHILD(Category,             QString)
    DU_KEY_ACCESSORS_IN_CHILD(RelativeVolume,       int)

    DU_KEY_ACCESSORS_OBJECT(InstrumentInfo,  DuInstrumentInfo)

    DU_KEY_ACCESSORS(PresetNum,  int) // editable
    DU_KEY_ACCESSORS(DisplayLed, int)

    DU_KEY_ACCESSORS_OBJECT(PresetArray,     DuArray)

    DU_KEY_ACCESSORS_OBJECT(Mixer,           DuMixer)
    DU_KEY_ACCESSORS_OBJECT(DistortionArray, DuArray)
    DU_KEY_ACCESSORS_OBJECT(WahArray,        DuArray)
    DU_KEY_ACCESSORS_OBJECT(CompressorArray, DuArray)
    DU_KEY_ACCESSORS_OBJECT(EqualizerArray,  DuArray)
    DU_KEY_ACCESSORS_OBJECT(DelayArray,      DuArray)
    DU_KEY_ACCESSORS_OBJECT(ChorusArray,     DuArray)
    DU_KEY_ACCESSORS_OBJECT(VibratoArray,    DuArray)
    DU_KEY_ACCESSORS_OBJECT(AdsrArray,       DuArray)

    DU_KEY_ACCESSORS_OBJECT(LedArray,        DuArray)
};

#endif // DUSOUNDINFO_H
