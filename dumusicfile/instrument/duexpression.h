#ifndef DUEXPRESSION_H
#define DUEXPRESSION_H

#include "../../general/ducontainer.h"


#define EXPRESSION_SIZE             12 + 8 + 1
#define EXPRESSION_PRESET_OFFSET    0


DU_OBJECT(DuExpression);

class DuExpression : public DuContainer
{
public:
    explicit DuExpression();
    ~DuExpression();

    virtual DuObjectPtr clone() const;

    static DuExpressionPtr fromDuMusicBinary(const preset_instr &du_preset);
    static DuExpressionPtr fromJson(const QJsonObject &jsonExpression);

    QByteArray toDuMusicBinary() const;

    int size() const;

    DU_KEY_ACCESSORS(Volume,                int)
    DU_KEY_ACCESSORS(Panning,               int)
    DU_KEY_ACCESSORS(Octave,                int)
    DU_KEY_ACCESSORS(SendToReverb,          int)

    DU_KEY_ACCESSORS(PortamentoOnOff,       int)
    DU_KEY_ACCESSORS(PortamentoControl,     int)
    DU_KEY_ACCESSORS(PortamentoTime,        int)
    DU_KEY_ACCESSORS(Expression,            int)
    DU_KEY_ACCESSORS(PitchBendSensitivity,  int)
    DU_KEY_ACCESSORS(KeyCurve,              int)

    DU_KEY_ACCESSORS(CompressorPreset,      int)
    DU_KEY_ACCESSORS(DelayPreset,           int)
    DU_KEY_ACCESSORS(DistortionPreset,      int)
    DU_KEY_ACCESSORS(EqualizerPreset,       int)
    DU_KEY_ACCESSORS(ChorusPreset,          int)

    DU_KEY_ACCESSORS(ArpeggiatorType,       int)
    DU_KEY_ACCESSORS(ArpeggiatorBeat,       int)
};

#endif // DUEXPRESSION_H
