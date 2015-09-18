#include "dueffectset.h"

#include <cstring>

#include <QJsonObject>


DU_OBJECT_IMPL(DuEffectSet)

DuEffectSet::DuEffectSet() :
    DuContainer()
{
    addChild(KeyCompressorOnOff,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x01, 0x00));

    addChild(KeyDelayOnOff,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x01, 0x00));

    addChild(KeyDistortionOnOff,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x01, 0x00));

    addChild(KeyEqualizerOnOff,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x01, 0x00));

    addChild(KeyChorusOnOff,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x01, 0x00));


    addChild(KeyMultinoteAct,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KeyMultinote,
             new DuBinaryData(4));


    addChild(KeyPitch,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));


    addChild(KeyAutopitchRate,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KeyAutopitchRange,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));


    addChild(KeyTremoloRate,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KeyTremoloRange,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));


    addChild(KeyAutopanRate,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KeyAutopanRange,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));


    addChild(KeyAutowahRate,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KeyAutowahRange,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));


    addChild(KeyAdsrAttack,
             new DuNumeric(FX_ADSR_ATTACK_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_ADSR_ATTACK_MAXVALUE, FX_ADSR_ATTACK_MINVALUE));

    addChild(KeyAdsrRelease,
             new DuNumeric(FX_ADSR_RELEAS_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_ADSR_RELEAS_MAXVALUE, FX_ADSR_RELEAS_MINVALUE));


    addChild(KeyWahType,
             new DuNumeric(FX_WAH_FILTERTYPE_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_WAH_FILTERTYPE_MAXVALUE, FX_WAH_FILTERTYPE_MINVALUE));

    addChild(KeyWahFrequency,
             new DuNumeric(FX_WAH_FILTERFREQ_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_WAH_FILTERFREQ_MAXVALUE, FX_WAH_FILTERFREQ_MINVALUE));

    addChild(KeyWahResonance,
             new DuNumeric(FX_WAH_FILTERRES_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_WAH_FILTERRES_MAXVALUE, FX_WAH_FILTERRES_MINVALUE));
}

DuEffectSet::~DuEffectSet()
{
}


DuObjectPtr DuEffectSet::clone() const
{
    return DuEffectSetPtr(new DuEffectSet(*this));
}


DuEffectSetPtr DuEffectSet::fromDuMusicBinary(const preset_instr &du_preset)
{
    DuEffectSetPtr effectSet(new DuEffectSet);
    bool verif = true;

    verif = effectSet->setCompressorOnOff(du_preset.s_compressor_onoff) ? verif : false;
    verif = effectSet->setDelayOnOff(du_preset.s_delay_onoff) ? verif : false;
    verif = effectSet->setDistortionOnOff(du_preset.s_distortion_onoff) ? verif : false;
    verif = effectSet->setEqualizerOnOff(du_preset.s_eq_onoff) ? verif : false;
    verif = effectSet->setChorusOnOff(du_preset.s_chorus_onoff) ? verif : false;

    verif = effectSet->setMultinoteAct(du_preset.s_multinote_act) ? verif : false;
    verif = effectSet->setMultinote(QByteArray((char *)du_preset.s_multinote, 4)) ? verif : false;

    verif = effectSet->setPitch(du_preset.s_pitch) ? verif : false;

    verif = effectSet->setAutopitchRate(du_preset.s_autopitch_rate) ? verif : false;
    verif = effectSet->setAutopitchRange(du_preset.s_autopitch_range) ? verif : false;
    verif = effectSet->setTremoloRate(du_preset.s_tremolo_rate) ? verif : false;
    verif = effectSet->setTremoloRange(du_preset.s_tremolo_range) ? verif : false;
    verif = effectSet->setAutopanRate(du_preset.s_autopan_rate) ? verif : false;
    verif = effectSet->setAutopanRange(du_preset.s_autopan_range) ? verif : false;
    verif = effectSet->setAutowahRate(du_preset.s_autowah_rate) ? verif : false;
    verif = effectSet->setAutowahRange(du_preset.s_autowah_range) ? verif : false;

    verif = effectSet->setAdsrAttack(du_preset.s_adsr_attack) ? verif : false;
    verif = effectSet->setAdsrRelease(du_preset.s_adsr_release) ? verif : false;
    verif = effectSet->setWahType(du_preset.s_wah_type) ? verif : false;
    verif = effectSet->setWahFrequency(du_preset.s_wah_freq) ? verif : false;
    verif = effectSet->setWahResonance(du_preset.s_wah_res) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuEffectSet::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    return effectSet;
}


DuEffectSetPtr DuEffectSet::fromJson(const QJsonObject &jsonEffectSet)
{
    QJsonValue jsonComprOnOff           = jsonEffectSet[KeyCompressorOnOff];
    QJsonValue jsonDelayOnOff           = jsonEffectSet[KeyDelayOnOff];
    QJsonValue jsonDistoOnOff           = jsonEffectSet[KeyDistortionOnOff];
    QJsonValue jsonEqualOnOff           = jsonEffectSet[KeyEqualizerOnOff];
    QJsonValue jsonChorusOnOff          = jsonEffectSet[KeyChorusOnOff];

    QJsonValue jsonMultinoteAct         = jsonEffectSet[KeyMultinoteAct];
    QJsonValue jsonMultinote            = jsonEffectSet[KeyMultinote];

    QJsonValue jsonPitch                = jsonEffectSet[KeyPitch];

    QJsonValue jsonAutopitchRate        = jsonEffectSet[KeyAutopitchRate];
    QJsonValue jsonAutopitchRange       = jsonEffectSet[KeyAutopitchRange];
    QJsonValue jsonTremoloRate          = jsonEffectSet[KeyTremoloRate];
    QJsonValue jsonTremoloRange         = jsonEffectSet[KeyTremoloRange];
    QJsonValue jsonAutopanRate          = jsonEffectSet[KeyAutopanRate];
    QJsonValue jsonAutopanRange         = jsonEffectSet[KeyAutopanRange];
    QJsonValue jsonAutowahRate          = jsonEffectSet[KeyAutowahRate];
    QJsonValue jsonAutowahRange         = jsonEffectSet[KeyAutowahRange];

    QJsonValue jsonAdsrAttack           = jsonEffectSet[KeyAdsrAttack];
    QJsonValue jsonAdsrRelease          = jsonEffectSet[KeyAdsrRelease];
    QJsonValue jsonWahType              = jsonEffectSet[KeyWahType];
    QJsonValue jsonWahFrequency         = jsonEffectSet[KeyWahFrequency];
    QJsonValue jsonWahResonance         = jsonEffectSet[KeyWahResonance];

    if (        !jsonComprOnOff.isDouble()
            ||  !jsonDelayOnOff.isDouble()      ||  !jsonDistoOnOff.isDouble()
            ||  !jsonEqualOnOff.isDouble()      ||  !jsonChorusOnOff.isDouble()
            
            ||  !jsonMultinoteAct.isDouble()    ||  !jsonMultinote.isString()
            
            ||  !jsonPitch.isDouble()
            
            ||  !jsonAutopitchRate.isDouble()   ||  !jsonAutopitchRange.isDouble()
            ||  !jsonTremoloRate.isDouble()     ||  !jsonTremoloRange.isDouble()
            ||  !jsonAutopanRate.isDouble()     ||  !jsonAutopanRange.isDouble()
            ||  !jsonAutowahRate.isDouble()     ||  !jsonAutowahRange.isDouble()

            ||  !jsonAdsrAttack.isDouble()      ||  !jsonAdsrRelease.isDouble()
            ||  !jsonWahType.isDouble()         ||  !jsonWahFrequency.isDouble()
            ||  !jsonWahResonance.isDouble())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuEffectSet::fromJson():\n"
                                      << "failed to generate DuEffectSet\n"
                                      << "a json key did not contain the proper type";
        
        return DuEffectSetPtr();
    }


    DuEffectSetPtr effectSet(new DuEffectSet);
    bool verif = true;

    verif = effectSet->setCompressorOnOff(jsonComprOnOff.toInt()) ? verif : false;
    verif = effectSet->setDelayOnOff(jsonDelayOnOff.toInt()) ? verif : false;
    verif = effectSet->setDistortionOnOff(jsonDistoOnOff.toInt()) ? verif : false;
    verif = effectSet->setEqualizerOnOff(jsonEqualOnOff.toInt()) ? verif : false;
    verif = effectSet->setChorusOnOff(jsonChorusOnOff.toInt()) ? verif : false;

    verif = effectSet->setMultinoteAct(jsonMultinoteAct.toInt()) ? verif : false;
    verif = effectSet->setMultinote(jsonMultinote.toString().toUtf8()) ? verif : false;

    verif = effectSet->setPitch(jsonPitch.toInt()) ? verif : false;

    verif = effectSet->setAutopitchRate(jsonAutopitchRate.toInt()) ? verif : false;
    verif = effectSet->setAutopitchRange(jsonAutopitchRange.toInt()) ? verif : false;
    verif = effectSet->setTremoloRate(jsonTremoloRate.toInt()) ? verif : false;
    verif = effectSet->setTremoloRange(jsonTremoloRange.toInt()) ? verif : false;
    verif = effectSet->setAutopanRate(jsonAutopanRate.toInt()) ? verif : false;
    verif = effectSet->setAutopanRange(jsonAutopanRange.toInt()) ? verif : false;
    verif = effectSet->setAutowahRate(jsonAutowahRate.toInt()) ? verif : false;
    verif = effectSet->setAutowahRange(jsonAutowahRange.toInt()) ? verif : false;

    verif = effectSet->setAdsrAttack(jsonAdsrAttack.toInt()) ? verif : false;
    verif = effectSet->setAdsrRelease(jsonAdsrRelease.toInt()) ? verif : false;
    verif = effectSet->setWahType(jsonWahType.toInt()) ? verif : false;
    verif = effectSet->setWahFrequency(jsonWahFrequency.toInt()) ? verif : false;
    verif = effectSet->setWahResonance(jsonWahResonance.toInt()) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuEffectSet::fromJson():\n"
                   << "an attribute was not properly set";
    }

    return effectSet;
}


QByteArray DuEffectSet::toDuMusicBinary() const
{
    preset_instr du_effectset;

    int tmpNum = 0;

    QByteArray tmpClear(size(), (char)0x00);
    std::memcpy((char *)&(du_effectset) + EFFECTSET_PRESET_OFFSET, tmpClear.data(), size());


    tmpNum = getCompressorOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_compressor_onoff = tmpNum;

    tmpNum = getDelayOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_delay_onoff = tmpNum;

    tmpNum = getDistortionOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_distortion_onoff = tmpNum;

    tmpNum = getEqualizerOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_eq_onoff = tmpNum;

    tmpNum = getChorusOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_chorus_onoff = tmpNum;


    tmpNum = getMultinoteAct();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_multinote_act = tmpNum;

    const QByteArray &tmpArray = getMultinote();
    if (tmpArray.isNull())
        return QByteArray();
    std::memcpy(du_effectset.s_multinote, tmpArray.data(), 4);


    tmpNum = getPitch();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_pitch = tmpNum;


    tmpNum = getAutopitchRate();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_autopitch_rate = tmpNum;

    tmpNum = getAutopitchRange();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_autopitch_range = tmpNum;

    tmpNum = getTremoloRate();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_tremolo_rate = tmpNum;

    tmpNum = getTremoloRange();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_tremolo_range = tmpNum;

    tmpNum = getAutopanRate();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_autopan_rate = tmpNum;

    tmpNum = getAutopanRange();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_autopan_range = tmpNum;

    tmpNum = getAutowahRate();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_autowah_rate = tmpNum;

    tmpNum = getAutowahRange();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_autowah_range = tmpNum;


    tmpNum = getAdsrAttack();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_adsr_attack = tmpNum;

    tmpNum = getAdsrRelease();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_adsr_release = tmpNum;

    tmpNum = getWahType();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_wah_type = tmpNum;

    tmpNum = getWahFrequency();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_wah_freq = tmpNum;

    tmpNum = getWahResonance();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_wah_res = tmpNum;


    return QByteArray((char *)&(du_effectset) + EFFECTSET_PRESET_OFFSET, size());
}


int DuEffectSet::size() const
{
    return EFFECTSET_SIZE;
}


DU_KEY_ACCESSORS_IMPL(DuEffectSet, CompressorOnOff,   Numeric,      int,        -1)
DU_KEY_ACCESSORS_IMPL(DuEffectSet, DelayOnOff,        Numeric,      int,        -1)
DU_KEY_ACCESSORS_IMPL(DuEffectSet, DistortionOnOff,   Numeric,      int,        -1)
DU_KEY_ACCESSORS_IMPL(DuEffectSet, EqualizerOnOff,    Numeric,      int,        -1)
DU_KEY_ACCESSORS_IMPL(DuEffectSet, ChorusOnOff,       Numeric,      int,        -1)

DU_KEY_ACCESSORS_IMPL(DuEffectSet, MultinoteAct,      Numeric,      int,        -1)
DU_KEY_ACCESSORS_IMPL(DuEffectSet, Multinote,         BinaryData,   QByteArray, QByteArray())

DU_KEY_ACCESSORS_IMPL(DuEffectSet, Pitch,             Numeric,      int,        -1)

DU_KEY_ACCESSORS_IMPL(DuEffectSet, AutopitchRate,     Numeric,      int,        -1)
DU_KEY_ACCESSORS_IMPL(DuEffectSet, AutopitchRange,    Numeric,      int,        -1)
DU_KEY_ACCESSORS_IMPL(DuEffectSet, TremoloRate,       Numeric,      int,        -1)
DU_KEY_ACCESSORS_IMPL(DuEffectSet, TremoloRange,      Numeric,      int,        -1)
DU_KEY_ACCESSORS_IMPL(DuEffectSet, AutopanRate,       Numeric,      int,        -1)
DU_KEY_ACCESSORS_IMPL(DuEffectSet, AutopanRange,      Numeric,      int,        -1)
DU_KEY_ACCESSORS_IMPL(DuEffectSet, AutowahRate,       Numeric,      int,        -1)
DU_KEY_ACCESSORS_IMPL(DuEffectSet, AutowahRange,      Numeric,      int,        -1)

DU_KEY_ACCESSORS_IMPL(DuEffectSet, AdsrAttack,        Numeric,      int,        -1)
DU_KEY_ACCESSORS_IMPL(DuEffectSet, AdsrRelease,       Numeric,      int,        -1)
DU_KEY_ACCESSORS_IMPL(DuEffectSet, WahType,           Numeric,      int,        -1)
DU_KEY_ACCESSORS_IMPL(DuEffectSet, WahFrequency,      Numeric,      int,        -1)
DU_KEY_ACCESSORS_IMPL(DuEffectSet, WahResonance,      Numeric,      int,        -1)
