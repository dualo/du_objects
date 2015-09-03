#ifndef DUSOUNDINFO_H
#define DUSOUNDINFO_H

#include "../general/ducontainer.h"


DU_OBJECT(DuMixer)
DU_OBJECT(DuArray)
DU_OBJECT(DuInstrumentInfo)

DU_OBJECT(DuSoundInfo)

class DuSoundInfo : public DuContainer
{
public:
    DuSoundInfo();

    virtual DuObjectPtr clone() const;
    virtual int size() const;

    static DuSoundInfoPtr fromBinary(const struct_instr& data);

    QByteArray toBinary() const;

    QString name() const;
    bool setName(const QString &name);

    int dreamProgramChange() const;
    bool setDreamProgramChange(int value);

    int octave() const;
    bool setOctave(int value);

    int activeNoteOff() const;
    bool setActiveNoteOff(int value);

    QString category() const;
    bool setCategory(const QString& value);

    int relativeVolume() const;
    bool setRelativeVolume(int value);

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
