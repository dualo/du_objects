#ifndef DUPRESET_H
#define DUPRESET_H

#include "../../general/ducontainer.h"
#include "../../general/dunumeric.h"


#define KEY_PRESET_VOLUME                   "Volume"
#define KEY_PRESET_PANNING                  "Panning"
#define KEY_PRESET_OCTAVE                   "Octave"
#define KEY_PRESET_SENDTOREVERB             "SendToReverb"

#define KEY_PRESET_PORTAMENTOONOFF          "PortamentoOnOff"
#define KEY_PRESET_PORTAMENTOCONTROL        "PortamentoControl"
#define KEY_PRESET_PORTAMENTOTIME           "PortamentoTime"
#define KEY_PRESET_EXPRESSION               "Expression"
#define KEY_PRESET_PITCHBENDSENSITIVITY     "PitchBendSensitivity"
#define KEY_PRESET_DISPOSITION              "Disposition"

#define KEY_PRESET_ARPEGIATORTYPE           "ArpeggiatorType"
#define KEY_PRESET_ARPEGIATORBEAT           "ArpeggiatorBeat"

#define KEY_PRESET_DIRECTIONGYROP           "DirectionGyroP"
#define KEY_PRESET_DIRECTIONGYROR           "DirectionGyroR"
#define KEY_PRESET_DIRECTIONGYROY           "DirectionGyroY"

#define KEY_PRESET_ACTIVEAFTERTOUCH         "ActiveAftertouch"
#define KEY_PRESET_ACTIVESLIDERL            "ActiveSliderLeft"
#define KEY_PRESET_ACTIVESLIDERR            "ActiveSliderRight"

#define KEY_PRESET_ACTIVEGYROP              "ActiveGyroP"
#define KEY_PRESET_ACTIVEGYROR              "ActiveGyroR"
#define KEY_PRESET_ACTIVEGYROY              "ActiveGyroY"

#define KEY_PRESET_ADSR_ONOFF               "AdsrOnOff"
#define KEY_PRESET_COMPR_ONOFF              "CompressorOnOff"
#define KEY_PRESET_DELAY_ONOFF              "DelayOnOff"
#define KEY_PRESET_DISTO_ONOFF              "DistortionOnOff"
#define KEY_PRESET_EQUAL_ONOFF              "EqualizerOnOff"
#define KEY_PRESET_CHORUS_ONOFF             "ChorusOnOff"
#define KEY_PRESET_VIB_ONOFF                "VibratoOnOff"
#define KEY_PRESET_WAH_ONOFF                "WahOnOff"

#define KEY_PRESET_PITCH                    "Pitch"

#define KEY_PRESET_AUTOPITCHRATE            "AutopitchRate"
#define KEY_PRESET_AUTOPITCHRANGE           "AutopitchRange"

#define KEY_PRESET_TREMOLORATE              "TremoloRate"
#define KEY_PRESET_TREMOLORANGE             "TremoloRange"

#define KEY_PRESET_AUTOPANRATE              "AutopanRate"
#define KEY_PRESET_AUTOPANRANGE             "AutopanRange"

#define KEY_PRESET_AUTOWAHRATE              "AutowahRate"
#define KEY_PRESET_AUTOWAHRANGE             "AutowahRange"


#define PARAMS_NUMERIC_SIZE     2
#define DIRECTIONGYRO_MINVALUE  -1
#define DIRECTIONGYRO_MAXVALUE  1


DU_OBJECT(DuPreset)

class DuPreset : public DuContainer
{
public:
    explicit DuPreset();
    ~DuPreset();

    virtual DuObjectPtr clone() const;

    static DuPresetPtr fromDuMusicBinary(const preset_instr &du_preset);
    static DuPresetPtr fromJson(const QJsonObject &jsonPreset);

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

    int getDirectionGyroP() const;
    bool setDirectionGyroP(int value);

    int getDirectionGyroR() const;
    bool setDirectionGyroR(int value);

    int getDirectionGyroY() const;
    bool setDirectionGyroY(int value);

    int getActiveAftertouch() const;
    bool setActiveAftertouch(int value);

    int getActiveSliderL() const;
    bool setActiveSliderL(int value);

    int getActiveSliderR() const;
    bool setActiveSliderR(int value);

    int getActiveGyroP() const;
    bool setActiveGyroP(int value);

    int getActiveGyroR() const;
    bool setActiveGyroR(int value);

    int getActiveGyroY() const;
    bool setActiveGyroY(int value);

    //TODO: finish adding children from here

    int getAdsrOnOff() const;
    bool setAdsrOnOff(int value);

    int getCompressorOnOff() const;
    bool setCompressorOnOff(int value);

    int getDelayOnOff() const;
    bool setDelayOnOff(int value);

    int getDistortionOnOff() const;
    bool setDistortionOnOff(int value);

    int getEqualizerOnOff() const;
    bool setEqualizerOnOff(int value);

    int getChorusOnOff() const;
    bool setChorusOnOff(int value);

    int getVibratoOnOff() const;
    bool setVibratoOnOff(int value);

    int getWahOnOff() const;
    bool setWahOnOff(int value);

    int getPitch() const;
    bool setPitch(int value);

    int getAutopitchRate() const;
    bool setAutopitchRate(int value);

    int getAutopitchRange() const;
    bool setAutopitchRange(int value);

    int getTremoloRate() const;
    bool setTremoloRate(int value);

    int getTremoloRange() const;
    bool setTremoloRange(int value);

    int getAutopanRate() const;
    bool setAutopanRate(int value);

    int getAutopanRange() const;
    bool setAutopanRange(int value);

    int getAutowahRate() const;
    bool setAutowahRate(int value);

    int getAutowahRange() const;
    bool setAutowahRange(int value);
};

#endif // DUPRESET_H
