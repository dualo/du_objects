#ifndef DUPRESET_H
#define DUPRESET_H

#include "ducontainer.h"
#include "dunumeric.h"


#define KEY_PRESET_VOLUME                   "Volume"
#define KEY_PRESET_PANNING                  "Panning"
#define KEY_PRESET_EXPRESSION               "Expression"
#define KEY_PRESET_OCTAVE                   "Octave"
#define KEY_PRESET_SENDTOREVERB             "SendToReverb"

#define KEY_PRESET_PORTAMENTOONOFF          "PortamentoOnOff"
#define KEY_PRESET_PORTAMENTOCONTROL        "PortamentoControl"
#define KEY_PRESET_PORTAMENTOTIME           "PortamentoTime"

#define KEY_PRESET_PITCHBENDSENSITIVITY     "PitchBendSensitivity"
#define KEY_PRESET_DISPOSITION              "Disposition"

//TODO: ADD MISSING PARAMETERS (GYRO, SLIDERS...)


class DuPreset : public DuContainer
{
public:
    explicit DuPreset();
    ~DuPreset();

    static DuPreset *fromDuMusicFile(const preset_instr &du_preset);

    static DuPreset *fromJson(const QJsonObject &jsonPreset);

    int getVolume() const;
    void setVolume(int value);

    int getPanning() const;
    void setPanning(int value);

    int getExpression() const;
    void setExpression(int value);

    int getOctave() const;
    void setOctave(int value);

    int getSendToReverb() const;
    void setSendToReverb(int value);

    int getPortamentoOnOff() const;
    void setPortamentoOnOff(int value);

    int getPortamentoControl() const;
    void setPortamentoControl(int value);

    int getPortamentoTime() const;
    void setPortamentoTime(int value);

    int getPitchBendSensitivity() const;
    void setPitchBendSensitivity(int value);

    int getDisposition() const;
    void setDisposition(int value);
};

#endif // DUPRESET_H
