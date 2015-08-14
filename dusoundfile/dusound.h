#ifndef DUSOUND_H
#define DUSOUND_H

#include "../general/ducontainer.h"


DU_OBJECT(DuInstrumentInfo)
DU_OBJECT(DuSoundHeader)
DU_OBJECT(DuArray)
DU_OBJECT(DuMixer)
DU_OBJECT(DuNumeric)

DU_OBJECT(DuSound)

class DuSound : public DuContainer
{
public:
    DuSound();

    virtual DuObjectPtr clone() const;

    virtual int size() const;

    static DuSoundPtr fromBinary(const QByteArray &data);
    static DuSoundPtr fromBinary(QIODevice *input);

    QByteArray toBinary() const;

    int databaseId() const;
    void setDatabaseId(int databaseId);

    QStringList lists() const;
    void setLists(const QStringList &lists);

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

    DU_KEY_ACCESSORS_OBJECT(Header,                 DuSoundHeader)

    DU_KEY_ACCESSORS_OBJECT(InstrumentInfo,         DuInstrumentInfo)

    DU_KEY_ACCESSORS(PresetNum,       int) // editable
    DU_KEY_ACCESSORS(DisplayLed,      int)

    DU_KEY_ACCESSORS_OBJECT(PresetArray,            DuArray)

    DU_KEY_ACCESSORS_OBJECT(Mixer,                  DuMixer)
    DU_KEY_ACCESSORS_OBJECT(DistortionArray,        DuArray)
    DU_KEY_ACCESSORS_OBJECT(WahArray,               DuArray)
    DU_KEY_ACCESSORS_OBJECT(CompressorArray,        DuArray)
    DU_KEY_ACCESSORS_OBJECT(EqualizerArray,         DuArray)
    DU_KEY_ACCESSORS_OBJECT(DelayArray,             DuArray)
    DU_KEY_ACCESSORS_OBJECT(ChorusArray,            DuArray)
    DU_KEY_ACCESSORS_OBJECT(VibratoArray,           DuArray)
    DU_KEY_ACCESSORS_OBJECT(AdsrArray,              DuArray)

    DU_KEY_ACCESSORS_OBJECT(LedArray,               DuArray)

    DU_KEY_ACCESSORS_OBJECT(NbSamplesPerLayerArray, DuArray)

    DU_KEY_ACCESSORS_OBJECT(DreamInstrParamArray,   DuArray)
    DU_KEY_ACCESSORS_OBJECT(DreamSampleParamArray,  DuArray)

    DU_KEY_ACCESSORS_OBJECT(SampleArray,            DuArray)

    DU_KEY_ACCESSORS_OBJECT(Mapping,                DuArray)

    DU_KEY_ACCESSORS_OBJECT(Metadata,               DuArray)

private:
    int m_databaseId;
    QStringList m_lists;
};

Q_DECLARE_METATYPE(DuSoundPtr)
Q_DECLARE_METATYPE(DuSoundConstPtr)

#endif // DUSOUND_H
