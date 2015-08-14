#ifndef DUEXPRESSION_H
#define DUEXPRESSION_H

#include "../../general/ducontainer.h"


#define KEY_EXPR_VOLUME                   "Volume"
#define KEY_EXPR_PANNING                  "Panning"
#define KEY_EXPR_OCTAVE                   "Octave"
#define KEY_EXPR_SENDTOREVERB             "SendToReverb"

#define KEY_EXPR_PORTAMENTOONOFF          "PortamentoOnOff"
#define KEY_EXPR_PORTAMENTOCONTROL        "PortamentoControl"
#define KEY_EXPR_PORTAMENTOTIME           "PortamentoTime"
#define KEY_EXPR_EXPRESSION               "Expression"
#define KEY_EXPR_PITCHBENDSENSITIVITY     "PitchBendSensitivity"
#define KEY_EXPR_DISPOSITION              "Disposition"

#define KEY_EXPR_ARPEGIATORTYPE           "ArpeggiatorType"
#define KEY_EXPR_ARPEGIATORBEAT           "ArpeggiatorBeat"


#define EXPRESSION_SIZE             12 + 8 + 1
#define EXPRESSION_PRESET_OFFSET    0


DU_OBJECT(DuExpression)

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

    int getVolume() const;
    bool setVolume(int value);

    int getPanning() const;
    bool setPanning(int value);

    int getSendToReverb() const;
    bool setSendToReverb(int value);

    int getOctave() const;
    bool setOctave(int value);

    int getPortamentoOnOff() const;
    bool setPortamentoOnOff(int value);

    int getPortamentoControl() const;
    bool setPortamentoControl(int value);

    int getPortamentoTime() const;
    bool setPortamentoTime(int value);

    int getExpression() const;
    bool setExpression(int value);

    int getPitchBendSensitivity() const;
    bool setPitchBendSensitivity(int value);

    int getDisposition() const;
    bool setDisposition(int value);

    int getArpeggiatorType() const;
    bool setArpeggiatorType(int value);

    int getArpeggiatorBeat() const;
    bool setArpeggiatorBeat(int value);
};

#endif // DUEXPRESSION_H
