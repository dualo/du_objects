#ifndef DUPRESET_H
#define DUPRESET_H

#include "duexpression.h"
#include "ducontrollers.h"


#define KEY_PRESET_EXPRESSION           "ExpressionSettings"
#define KEY_PRESET_CONTROLLERS          "ControllerParameters"


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

    DuExpressionConstPtr getExpression() const;
    void setExpression(const DuExpressionPtr &expression);

    DuControllersConstPtr getControllers() const;
    void setControllers(const DuControllersPtr &controllers);

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
