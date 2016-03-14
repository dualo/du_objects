#ifndef DUPRESET_H
#define DUPRESET_H

#include "../general/ducontainer.h"


#define PARAMS_NUMERIC_SIZE     2


#define DuPreset_Children \
    X(Name,                 String, QString, QString()) \
    \
    X(Volume,               Numeric, int, -1) \
    X(Panning,              Numeric, int, -1) \
    X(SendToReverb,         Numeric, int, -1) \
    X(Octave,               Numeric, int, -1) \
    X(KeyCurve,             Numeric, int, -1) \
    X(Expression,           Numeric, int, -1) \
    \
    X(ActiveAftertouch,     Numeric, int, -1) \
    X(ActiveSliderL,        Numeric, int, -1) \
    X(ActiveSliderR,        Numeric, int, -1) \
    X(ActiveGyroP,          Numeric, int, -1) \
    X(ActiveGyroR,          Numeric, int, -1) \
    X(ActiveGyroY,          Numeric, int, -1) \
    \
    X(DirectionAftertouch,  Numeric, int, 0) \
    X(DirectionSliderL,     Numeric, int, 0) \
    X(DirectionSliderR,     Numeric, int, 0) \
    X(DirectionGyroP,       Numeric, int, 0) \
    X(DirectionGyroR,       Numeric, int, 0) \
    X(DirectionGyroY,       Numeric, int, 0) \
    \
    X(PortamentoOnOff,      Numeric, int, -1) \
    X(PortamentoControl,    Numeric, int, -1) \
    X(PortamentoTime,       Numeric, int, -1) \
    \
    X(DisplayLed,           Numeric, int, -1) \
    X_OBJECT_TEMPLATE(LedArray, DuArray, DuNumeric) \
    \
    X(Pitch,                Numeric, int, -1) \
    X(PitchBendSensitivity, Numeric, int, -1) \
    \
    X(AdsrAttack,           Numeric, int, -1) \
    X(AdsrRelease,          Numeric, int, -1) \
    \
    X(WahType,              Numeric, int, -1) \
    X(WahFrequency,         Numeric, int, -1) \
    X(WahResonance,         Numeric, int, -1) \
    \
    X(MultinoteAct,         Numeric, int, -1) \
    X_OBJECT_TEMPLATE(Multinote, DuArray, DuNumeric) \
    \
    X(ArpeggiatorType,      Numeric, int, -1) \
    X(ArpeggiatorBeat,      Numeric, int, -1) \
    \
    X(AutopitchRate,        Numeric, int, -1) \
    X(AutopitchRange,       Numeric, int, -1) \
    \
    X(TremoloRate,          Numeric, int, -1) \
    X(TremoloRange,         Numeric, int, -1) \
    \
    X(AutopanRate,          Numeric, int, -1) \
    X(AutopanRange,         Numeric, int, -1) \
    \
    X(AutowahRate,          Numeric, int, -1) \
    X(AutowahRange,         Numeric, int, -1) \
    \
    X(ChorusPreset,         Numeric, int, -1) \
    X(CompressorOnOff,      Numeric, int, -1) \
    X(DelayOnOff,           Numeric, int, -1) \
    X(DistortionOnOff,      Numeric, int, -1) \
    X(EqualizerOnOff,       Numeric, int, -1) \
    X(ChorusOnOff,          Numeric, int, -1) \
    X(ReverbOnOff,          Numeric, int, -1) \
    \
    X_OBJECT(Mixer,         DuMixer) \
    X_OBJECT(Distortion,    DuDistortion) \
    X_OBJECT(Compressor,    DuCompressor) \
    X_OBJECT(Equalizer,     DuEqualizer) \
    X_OBJECT(Delay,         DuDelay) \
    X_OBJECT_TEMPLATE(ChorusArray, DuArray, DuChorus)


DU_OBJECT_TEMPLATE(DuArray);
DU_OBJECT(DuMixer);
DU_OBJECT(DuDistortion);
DU_OBJECT(DuCompressor);
DU_OBJECT(DuEqualizer);
DU_OBJECT(DuDelay);
DU_OBJECT(DuChorus);
DU_OBJECT(DuNumeric);

DU_OBJECT(DuPreset)

class DuPreset : public DuContainer
{
public:
    explicit DuPreset();

    virtual DuObjectPtr clone() const;

    static DuPresetPtr fromDuMusicBinary(const preset_instr &du_preset);
    static DuPresetPtr fromJson(const QJsonObject &jsonPreset);

    QByteArray toDuMusicBinary() const;

    int size() const;

#define X(key, dutype, type, defaultReturn) DU_KEY_ACCESSORS(key, type)
#define X_OBJECT(key, dutype) DU_KEY_ACCESSORS_OBJECT(key, dutype)
#define X_OBJECT_TEMPLATE(key, dutype, tpltype) DU_KEY_ACCESSORS_OBJECT_TEMPLATE(key, dutype, tpltype)
    DuPreset_Children
#undef X_OBJECT_TEMPLATE
#undef X_OBJECT
#undef X
};

#endif // DUPRESET_H
