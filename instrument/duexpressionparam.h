#ifndef DUEXPRESSIONPARAM_H
#define DUEXPRESSIONPARAM_H

#include "../general/ducontainer.h"


#define EXPRESSION_SIZE             12 + 8 + 1
#define EXPRESSION_PRESET_OFFSET    0

#define DuExpressionParam_Children \
    X(Volume,               Numeric, int, -1) \
    X(Panning,              Numeric, int, -1) \
    X(Octave,               Numeric, int, -1) \
    X(SendToReverb,         Numeric, int, -1) \
     \
    X(PortamentoOnOff,      Numeric, int, -1) \
    X(PortamentoControl,    Numeric, int, -1) \
    X(PortamentoTime,       Numeric, int, -1) \
    X(Expression,           Numeric, int, -1) \
    X(PitchBendSensitivity, Numeric, int, -1) \
    X(KeyCurve,             Numeric, int, -1) \
     \
    X(CompressorPreset,     Numeric, int, -1) \
    X(DelayPreset,          Numeric, int, -1) \
    X(DistortionPreset,     Numeric, int, -1) \
    X(EqualizerPreset,      Numeric, int, -1) \
    X(ChorusPreset,         Numeric, int, -1) \
     \
    X(ArpeggiatorType,      Numeric, int, -1) \
    X(ArpeggiatorBeat,      Numeric, int, -1)

DU_OBJECT(DuExpressionParam);

class DuExpressionParam : public DuContainer
{
public:
    explicit DuExpressionParam();
    ~DuExpressionParam();

    virtual DuObjectPtr clone() const;

    static DuExpressionParamPtr fromDuMusicBinary(const preset_instr &du_preset);
    static DuExpressionParamPtr fromJson(const QJsonObject &jsonExpression);

    QByteArray toDuMusicBinary() const;

    int size() const;

#define X(key, dutype, type, defaultReturn) DU_KEY_ACCESSORS(key, type)
    DuExpressionParam_Children
#undef X
};

#endif // DUEXPRESSIONPARAM_H
