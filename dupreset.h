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

//TODO: add missing parameters (gyro, sliders...)


DU_OBJECT(DuPreset)

class DuPreset : public DuContainer
{
public:
    explicit DuPreset();
    ~DuPreset();

    virtual DuObjectPtr clone() const;

    static DuPresetPtr fromDuMusicFile(const preset_instr &du_preset);
    static DuPresetPtr fromJson(const QJsonObject &jsonPreset);

    QByteArray toDuMusicFile() const;

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
};

#endif // DUPRESET_H
