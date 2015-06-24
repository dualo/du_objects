#ifndef DUEFFECTSET_H
#define DUEFFECTSET_H

#include "../../general/ducontainer.h"
#include "../../general/dunumeric.h"


#define KEY_EFFECTSET_ADSR_ONOFF               "AdsrOnOff"
#define KEY_EFFECTSET_COMPR_ONOFF              "CompressorOnOff"
#define KEY_EFFECTSET_DELAY_ONOFF              "DelayOnOff"
#define KEY_EFFECTSET_DISTO_ONOFF              "DistortionOnOff"
#define KEY_EFFECTSET_EQUAL_ONOFF              "EqualizerOnOff"
#define KEY_EFFECTSET_CHORUS_ONOFF             "ChorusOnOff"
#define KEY_EFFECTSET_VIB_ONOFF                "VibratoOnOff"
#define KEY_EFFECTSET_WAH_ONOFF                "WahOnOff"

#define KEY_EFFECTSET_PITCH                    "Pitch"

#define KEY_EFFECTSET_AUTOPITCHRATE            "AutopitchRate"
#define KEY_EFFECTSET_AUTOPITCHRANGE           "AutopitchRange"
#define KEY_EFFECTSET_TREMOLORATE              "TremoloRate"
#define KEY_EFFECTSET_TREMOLORANGE             "TremoloRange"
#define KEY_EFFECTSET_AUTOPANRATE              "AutopanRate"
#define KEY_EFFECTSET_AUTOPANRANGE             "AutopanRange"
#define KEY_EFFECTSET_AUTOWAHRATE              "AutowahRate"
#define KEY_EFFECTSET_AUTOWAHRANGE             "AutowahRange"


#define EFFECTSET_SIZE              8 + 16 + 9 + 59
#define EFFECTSET_PRESET_OFFSET     36


DU_OBJECT(DuEffectSet)

class DuEffectSet : public DuContainer
{
public:
    explicit DuEffectSet();
    ~DuEffectSet();

    DuObjectPtr clone() const;

    static DuEffectSetPtr fromDuMusicBinary(const preset_instr &du_preset);
    static DuEffectSetPtr fromJson(const QJsonObject &jsonEffectSet);

    QByteArray toDuMusicBinary() const;

    int size() const;

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

#endif // DUEFFECTSET_H
