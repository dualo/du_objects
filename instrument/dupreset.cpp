#include "dupreset.h"

#include <cstring>

#include <QJsonObject>

#include "../du-touch/dualo_structs/controler_structs.h"

#include "../general/duarray.h"
#include "../general/dunumeric.h"
#include "../general/dustring.h"

#include "../instrument/effects/duchorus.h"
#include "../instrument/effects/ducompressor.h"
#include "../instrument/effects/dudelay.h"
#include "../instrument/effects/dudistortion.h"
#include "../instrument/effects/duequalizer.h"
#include "../instrument/effects/dumixer.h"


DU_OBJECT_IMPL(DuPreset)

DuPreset::DuPreset() :
    DuContainer()
{
    // preset name
    addChild(KeyName, new DuString(PRESET_NAME_MAXSIZE));


    // general
    addChild(KeyVolume,
             new DuNumeric(MAIN_VOLUME_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_VOLUME_MAX, MAIN_VOLUME_MIN));

    addChild(KeyPanning,
             new DuNumeric(MAIN_PANNING_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_PANNING_MAX, MAIN_PANNING_MIN));

    addChild(KeySendToReverb,
             new DuNumeric(MAIN_SENDTOREV_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_SENDTOREV_MAX, MAIN_SENDTOREV_MIN));

    addChild(KeyOctave,
             new DuNumeric(MAIN_OCTAVE_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_OCTAVE_MAX, MAIN_OCTAVE_MIN));

    addChild(KeyKeyCurve,
             new DuNumeric(KEYS_CURVES_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           KEYS_CURVES_MAXVALUE, KEYS_CURVES_MINVALUE));

    addChild(KeyExpression,
             new DuNumeric(MAIN_EXPRESSION_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_EXPRESSION_MAX, MAIN_EXPRESSION_MIN));


    // controleurs
    addChild(KeyActiveAftertouch,   new DuNumeric(0x0000, 2, 0xFFFF, 0x0000));
    addChild(KeyActiveSliderL,      new DuNumeric(0x0000, 2, 0xFFFF, 0x0000));
    addChild(KeyActiveSliderR,      new DuNumeric(0x0000, 2, 0xFFFF, 0x0000));
    addChild(KeyActiveGyroP,        new DuNumeric(0x0000, 2, 0xFFFF, 0x0000));
    addChild(KeyActiveGyroR,        new DuNumeric(0x0000, 2, 0xFFFF, 0x0000));
    addChild(KeyActiveGyroY,        new DuNumeric(0x0000, 2, 0xFFFF, 0x0000));

    const DuNumericPtr& directionAftertouch = DuNumericPtr(new DuNumeric(DIRECTIONCONTROLER_MINVALUE, NUMERIC_DEFAULT_SIZE,
                                                                         DIRECTIONCONTROLER_MAXVALUE, DIRECTIONCONTROLER_MINVALUE));
    directionAftertouch->setForbiddenValues(QVector<int>() << 0);
    addChild(KeyDirectionAftertouch, directionAftertouch);


    const DuNumericPtr& directionSliderL = DuNumericPtr(new DuNumeric(DIRECTIONCONTROLER_MINVALUE, NUMERIC_DEFAULT_SIZE,
                                                                      DIRECTIONCONTROLER_MAXVALUE, DIRECTIONCONTROLER_MINVALUE));
    directionSliderL->setForbiddenValues(QVector<int>() << 0);
    addChild(KeyDirectionSliderL, directionSliderL);


    const DuNumericPtr& directionSliderR = DuNumericPtr(new DuNumeric(DIRECTIONCONTROLER_MINVALUE, NUMERIC_DEFAULT_SIZE,
                                                                      DIRECTIONCONTROLER_MAXVALUE, DIRECTIONCONTROLER_MINVALUE));
    directionSliderR->setForbiddenValues(QVector<int>() << 0);
    addChild(KeyDirectionSliderR, directionSliderR);


    const DuNumericPtr& directionGyroP = DuNumericPtr(new DuNumeric(DIRECTIONCONTROLER_MINVALUE, NUMERIC_DEFAULT_SIZE,
                                                                    DIRECTIONCONTROLER_MAXVALUE, DIRECTIONCONTROLER_MINVALUE));
    directionGyroP->setForbiddenValues(QVector<int>() << 0);
    addChild(KeyDirectionGyroP, directionGyroP);


    const DuNumericPtr& directionGyroR = DuNumericPtr(new DuNumeric(DIRECTIONCONTROLER_MINVALUE, NUMERIC_DEFAULT_SIZE,
                                                                    DIRECTIONCONTROLER_MAXVALUE, DIRECTIONCONTROLER_MINVALUE));
    directionGyroR->setForbiddenValues(QVector<int>() << 0);
    addChild(KeyDirectionGyroR, directionGyroR);


    const DuNumericPtr& directionGyroY = DuNumericPtr(new DuNumeric(DIRECTIONCONTROLER_MINVALUE, NUMERIC_DEFAULT_SIZE,
                                                                    DIRECTIONCONTROLER_MAXVALUE, DIRECTIONCONTROLER_MINVALUE));
    directionGyroY->setForbiddenValues(QVector<int>() << 0);
    addChild(KeyDirectionGyroY, directionGyroY);


    // portamento
    addChild(KeyPortamentoOnOff,
             new DuNumeric(MAIN_PORT_ONOFF_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_PORT_ONOFF_MAX, MAIN_PORT_ONOFF_MIN));

    addChild(KeyPortamentoControl,
             new DuNumeric(MAIN_PORT_CTRL_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_PORT_CTRL_MAX, MAIN_PORT_CTRL_MIN));

    addChild(KeyPortamentoTime,
             new DuNumeric(MAIN_PORT_TIME_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_PORT_TIME_MAX, MAIN_PORT_TIME_MIN));


    // led marks
    addChild(KeyDisplayLed, new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));

    DuArrayPtr<DuNumeric> ledArray(new DuArray<DuNumeric>(NUM_LED_VALUE));
    for (int i = 0; i < NUM_LED_VALUE; ++i)
        ledArray->append(new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));
    addChild(KeyLedArray, ledArray);


    // pitch
    addChild(KeyPitch, new DuNumeric(0x40, NUMERIC_DEFAULT_SIZE, 0x7F, 0x00));

    addChild(KeyPitchBendSensitivity,
             new DuNumeric(MAIN_PB_SENS_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_PB_SENS_MAX, MAIN_PB_SENS_MIN));


    // adsr
    addChild(KeyAdsrAttack,
             new DuNumeric(FX_ADSR_ATTACK_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_ADSR_ATTACK_MAXVALUE, FX_ADSR_ATTACK_MINVALUE));

    addChild(KeyAdsrRelease,
             new DuNumeric(FX_ADSR_RELEAS_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_ADSR_RELEAS_MAXVALUE, FX_ADSR_RELEAS_MINVALUE));


    // wah
    addChild(KeyWahType,
             new DuNumeric(FX_WAH_FILTERTYPE_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_WAH_FILTERTYPE_MAXVALUE, FX_WAH_FILTERTYPE_MINVALUE));

    addChild(KeyWahFrequency,
             new DuNumeric(FX_WAH_FILTERFREQ_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_WAH_FILTERFREQ_MAXVALUE, FX_WAH_FILTERFREQ_MINVALUE));

    addChild(KeyWahResonance,
             new DuNumeric(FX_WAH_FILTERRES_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_WAH_FILTERRES_MAXVALUE, FX_WAH_FILTERRES_MINVALUE));


    // multi-note
    addChild(KeyMultinoteAct, new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));

    DuArrayPtr<DuNumeric> multinoteArray(new DuArray<DuNumeric>(4));
    for (int i = 0; i < 4; ++i)
        multinoteArray->append(new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                                             127, -128));
    addChild(KeyMultinote, multinoteArray);


    // arpeggiator
    addChild(KeyArpeggiatorType, new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0x08, 0x00));
    addChild(KeyArpeggiatorBeat, new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0x07, 0x00));


    // auto pitch
    addChild(KeyAutopitchRate,  new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0x07, 0x00));
    addChild(KeyAutopitchRange, new DuNumeric(0x7F, NUMERIC_DEFAULT_SIZE, 0x7F, 0x00));


    // tremolo
    addChild(KeyTremoloRate,  new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0x07, 0x00));
    addChild(KeyTremoloRange, new DuNumeric(0x7F, NUMERIC_DEFAULT_SIZE, 0x7F, 0x00));


    // autopan
    addChild(KeyAutopanRate,  new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0x07, 0x00));
    addChild(KeyAutopanRange, new DuNumeric(0x7F, NUMERIC_DEFAULT_SIZE, 0x7F, 0x00));


    // auto wah
    addChild(KeyAutowahRate,  new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0x07, 0x00));
    addChild(KeyAutowahRange, new DuNumeric(0x7F, NUMERIC_DEFAULT_SIZE, 0x7F, 0x00));


    // FX structs
    addChild(KeyChorusPreset,
             new DuNumeric(FX_CHORUS_PRESET_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_NUM_FX_INTR - 1, 0x00));

    addChild(KeyCompressorOnOff, new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));
    addChild(KeyDelayOnOff,      new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));
    addChild(KeyDistortionOnOff, new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));
    addChild(KeyEqualizerOnOff,  new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));
    addChild(KeyChorusOnOff,     new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));
    addChild(KeyReverbOnOff,     new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0x01, 0x00));

    addChild(KeyMixer,      new DuMixer);
    addChild(KeyDistortion, new DuDistortion);
    addChild(KeyCompressor, new DuCompressor);
    addChild(KeyEqualizer,  new DuEqualizer);
    addChild(KeyDelay,      new DuDelay);

    DuChorusPtr chorus(new DuChorus);
    chorus->setMode(0);
    DuChorusPtr flanger(new DuChorus);
    flanger->setMode(1);
    DuChorusPtr phaser(new DuChorus);
    phaser->setMode(3);

    DuArrayPtr<DuChorus> chorusArray(new DuArray<DuChorus>(3));
    chorusArray->append(chorus);
    chorusArray->append(flanger);
    chorusArray->append(phaser);
    addChild(KeyChorusArray, chorusArray);
}

DuObjectPtr DuPreset::clone() const
{
    return DuPresetPtr(new DuPreset(*this));
}


DuPresetPtr DuPreset::fromDuMusicBinary(const preset_instr &du_preset)
{
    DuPresetPtr preset(new DuPreset);

    bool verif = true;

    // preset name
    verif = preset->setName(DuString::fromStruct(du_preset.s_name, PRESET_NAME_MAXSIZE)) ? verif : false;

    // general
    verif = preset->setVolume(du_preset.s_volume) ? verif : false;
    verif = preset->setPanning(du_preset.s_panning) ? verif : false;
    verif = preset->setSendToReverb(du_preset.s_sendtorev) ? verif : false;
    verif = preset->setOctave(du_preset.s_instr_octave) ? verif : false;
    verif = preset->setKeyCurve(du_preset.s_key_curve) ? verif : false;
    verif = preset->setExpression(du_preset.s_expression) ? verif : false;

    // controleurs
    verif = preset->setActiveAftertouch(du_preset.s_activ_aftertouch) ? verif : false;
    verif = preset->setActiveSliderL(du_preset.s_activ_slider_L) ? verif : false;
    verif = preset->setActiveSliderR(du_preset.s_activ_slider_R) ? verif : false;
    verif = preset->setActiveGyroP(du_preset.s_activ_gyro_P) ? verif : false;
    verif = preset->setActiveGyroR(du_preset.s_activ_gyro_R) ? verif : false;
    verif = preset->setActiveGyroY(du_preset.s_activ_gyro_Y) ? verif : false;

    verif = preset->setDirectionAftertouch(du_preset.s_direction_aftertouch) ? verif : false;
    verif = preset->setDirectionSliderL(du_preset.s_direction_slider_L) ? verif : false;
    verif = preset->setDirectionSliderR(du_preset.s_direction_slider_R) ? verif : false;
    verif = preset->setDirectionGyroP(du_preset.s_direction_gyro_P) ? verif : false;
    verif = preset->setDirectionGyroR(du_preset.s_direction_gyro_R) ? verif : false;
    verif = preset->setDirectionGyroY(du_preset.s_direction_gyro_Y) ? verif : false;

    // portamento
    verif = preset->setPortamentoOnOff(du_preset.s_portamento_on_off) ? verif : false;
    verif = preset->setPortamentoControl(du_preset.s_portamento_ctrl) ? verif : false;
    verif = preset->setPortamentoTime(du_preset.s_portamento_time) ? verif : false;

    // led marks
    verif = preset->setDisplayLed(du_preset.s_displayled) ? verif : false;

    DuArrayPtr<DuNumeric> ledsArray(new DuArray<DuNumeric>(NUM_LED_VALUE));
    for (int i = 0; i < NUM_LED_VALUE; ++i)
    {
        DuNumericPtr leds(new DuNumeric(du_preset.s_leds[i], NUMERIC_DEFAULT_SIZE, 0xFF, 0x00, 0));
        if (leds == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate DuPreset\n"
                                          << "Leds" << i << "was not properly generated";

            return DuPresetPtr();
        }
        ledsArray->append(leds);
    }
    preset->setLedArray(ledsArray);

    // pitch
    verif = preset->setPitch(du_preset.s_pitch) ? verif : false;
    verif = preset->setPitchBendSensitivity(du_preset.s_pitch_bend_sensitivity) ? verif : false;

    // adsr
    verif = preset->setAdsrAttack(du_preset.s_adsr_attack) ? verif : false;
    verif = preset->setAdsrRelease(du_preset.s_adsr_release) ? verif : false;

    // wah
    verif = preset->setWahType(du_preset.s_wah_type) ? verif : false;
    verif = preset->setWahFrequency(du_preset.s_wah_freq) ? verif : false;
    verif = preset->setWahResonance(du_preset.s_wah_res) ? verif : false;

    // multi-note
    verif = preset->setMultinoteAct(du_preset.s_multinote_act) ? verif : false;

    DuArrayPtr<DuNumeric> multinoteArray(new DuArray<DuNumeric>(4));
    for (int i = 0; i < 4; ++i)
    {
        multinoteArray->append(new DuNumeric(du_preset.s_multinote[i], NUMERIC_DEFAULT_SIZE, 127, -128, 0x00));
    }
    preset->setMultinote(multinoteArray);

    // arpeggiator
    verif = preset->setArpeggiatorType(du_preset.s_arpegiator_type) ? verif : false;
    verif = preset->setArpeggiatorBeat(du_preset.s_arpegiator_beat) ? verif : false;

    // auto pitch
    verif = preset->setAutopitchRate(du_preset.s_autopitch_rate) ? verif : false;
    verif = preset->setAutopitchRange(du_preset.s_autopitch_range) ? verif : false;

    // tremolo
    verif = preset->setTremoloRate(du_preset.s_tremolo_rate) ? verif : false;
    verif = preset->setTremoloRange(du_preset.s_tremolo_range) ? verif : false;

    // autopan
    verif = preset->setAutopanRate(du_preset.s_autopan_rate) ? verif : false;
    verif = preset->setAutopanRange(du_preset.s_autopan_range) ? verif : false;

    // auto wah
    verif = preset->setAutowahRate(du_preset.s_autowah_rate) ? verif : false;
    verif = preset->setAutowahRange(du_preset.s_autowah_range) ? verif : false;

    // FX structs
    verif = preset->setChorusPreset(du_preset.s_chorus_preset) ? verif : false;
    verif = preset->setCompressorOnOff(du_preset.s_compressor_onoff) ? verif : false;
    verif = preset->setDelayOnOff(du_preset.s_delay_onoff) ? verif : false;
    verif = preset->setDistortionOnOff(du_preset.s_distortion_onoff) ? verif : false;
    verif = preset->setEqualizerOnOff(du_preset.s_eq_onoff) ? verif : false;
    verif = preset->setChorusOnOff(du_preset.s_chorus_onoff) ? verif : false;
    verif = preset->setReverbOnOff(du_preset.s_reverb_onoff) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "An attribute was not properly set";
    }

    // fx
    const DuMixerPtr &mixer = DuMixer::fromDuMusicBinary(du_preset.s_mix);
    if (mixer == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate DuPreset\n"
                                      << "the DuMixer was not properly generated";

        return DuPresetPtr();
    }
    preset->setMixer(mixer);

    const DuDistortionPtr &distortion = DuDistortion::fromDuMusicBinary(du_preset.s_distortion);
    if (distortion == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate DuPreset\n"
                                      << "the DuDistortion was not properly generated";

        return DuPresetPtr();
    }
    preset->setDistortion(distortion);

    const DuCompressorPtr &compressor = DuCompressor::fromDuMusicBinary(du_preset.s_compressor);
    if (compressor == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate DuPreset\n"
                                      << "the DuCompressor was not properly generated";

        return DuPresetPtr();
    }
    preset->setCompressor(compressor);

    const DuEqualizerPtr &equalizer = DuEqualizer::fromDuMusicBinary(du_preset.s_equalizer);
    if (equalizer == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate DuPreset\n"
                                      << "the DuEqualizer was not properly generated";

        return DuPresetPtr();
    }
    preset->setEqualizer(equalizer);

    const DuDelayPtr &delay = DuDelay::fromDuMusicBinary(du_preset.s_delay);
    if (delay == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate DuPreset\n"
                                      << "the DuDelay was not properly generated";

        return DuPresetPtr();
    }
    preset->setDelay(delay);

    DuArrayPtr<DuChorus> chorusArray(new DuArray<DuChorus>(3));
    for (int i = 0; i < 3; ++i)
    {
        const DuChorusPtr &chorus = DuChorus::fromDuMusicBinary(du_preset.s_chorus[i]);
        if (chorus == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate DuPreset\n"
                                          << "Chorus" << i << "was not properly generated";

            return DuPresetPtr();
        }
        chorusArray->append(chorus);
    }
    preset->setChorusArray(chorusArray);

    return preset;
}


DuPresetPtr DuPreset::fromJson(const QJsonObject &jsonPreset)
{
    DuPresetPtr preset(new DuPreset);

    if (!preset->parseJson(jsonPreset))
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to parse json:" << jsonPreset;
        return DuPresetPtr();
    }

    return preset;
}


QByteArray DuPreset::toDuMusicBinary() const
{
    preset_instr du_preset;

    QString tmpStr;
    int tmpNum = 0;

    std::memset(&du_preset, 0, static_cast<size_t>(size()));


    // preset name
    QByteArray tmpName(PRESET_NAME_MAXSIZE, 0x00);
    tmpStr = getName();
    if (tmpStr.isNull())
        return QByteArray();
    tmpName.prepend(tmpStr.toLatin1());

    std::memcpy(du_preset.s_name, tmpName.constData(), PRESET_NAME_MAXSIZE);


    // general
    tmpNum = getVolume();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_volume = static_cast<uint8_t>(tmpNum);

    tmpNum = getPanning();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_panning = static_cast<uint8_t>(tmpNum);

    tmpNum = getSendToReverb();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_sendtorev = static_cast<uint8_t>(tmpNum);

    tmpNum = getOctave();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_instr_octave = static_cast<uint8_t>(tmpNum);

    tmpNum = getKeyCurve();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_key_curve = static_cast<uint8_t>(tmpNum);

    tmpNum = getExpression();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_expression = static_cast<uint8_t>(tmpNum);


    // controleurs
    tmpNum = getActiveAftertouch();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_activ_aftertouch = static_cast<uint16_t>(tmpNum);

    tmpNum = getActiveSliderL();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_activ_slider_L = static_cast<uint16_t>(tmpNum);

    tmpNum = getActiveSliderR();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_activ_slider_R = static_cast<uint16_t>(tmpNum);

    tmpNum = getActiveGyroP();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_activ_gyro_P = static_cast<uint16_t>(tmpNum);

    tmpNum = getActiveGyroR();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_activ_gyro_R = static_cast<uint16_t>(tmpNum);

    tmpNum = getActiveGyroY();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_activ_gyro_Y = static_cast<uint16_t>(tmpNum);


    tmpNum = getDirectionAftertouch();
    if (tmpNum == 0)
        return QByteArray();
    du_preset.s_direction_aftertouch = static_cast<int8_t>(tmpNum);

    tmpNum = getDirectionSliderL();
    if (tmpNum == 0)
        return QByteArray();
    du_preset.s_direction_slider_L = static_cast<int8_t>(tmpNum);

    tmpNum = getDirectionSliderR();
    if (tmpNum == 0)
        return QByteArray();
    du_preset.s_direction_slider_R = static_cast<int8_t>(tmpNum);

    tmpNum = getDirectionGyroP();
    if (tmpNum == 0)
        return QByteArray();
    du_preset.s_direction_gyro_P = static_cast<int8_t>(tmpNum);

    tmpNum = getDirectionGyroR();
    if (tmpNum == 0)
        return QByteArray();
    du_preset.s_direction_gyro_R = static_cast<int8_t>(tmpNum);

    tmpNum = getDirectionGyroY();
    if (tmpNum == 0)
        return QByteArray();
    du_preset.s_direction_gyro_Y = static_cast<int8_t>(tmpNum);


    // portamento
    tmpNum = getPortamentoOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_portamento_on_off = static_cast<uint8_t>(tmpNum);

    tmpNum = getPortamentoControl();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_portamento_ctrl = static_cast<uint8_t>(tmpNum);

    tmpNum = getPortamentoTime();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_portamento_time = static_cast<uint8_t>(tmpNum);


    // led marks
    tmpNum = getDisplayLed();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_displayled = static_cast<uint8_t>(tmpNum);

    const DuArrayConstPtr<DuNumeric> &ledsArray = getLedArray();
    if (ledsArray == NULL)
        return QByteArray();
    std::memcpy(du_preset.s_leds, ledsArray->toDuMusicBinary().constData(), static_cast<size_t>(ledsArray->size()));


    // pitch
    tmpNum = getPitch();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_pitch = static_cast<uint8_t>(tmpNum);

    tmpNum = getPitchBendSensitivity();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_pitch_bend_sensitivity = static_cast<uint8_t>(tmpNum);


    // adsr
    tmpNum = getAdsrAttack();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_adsr_attack = static_cast<uint8_t>(tmpNum);

    tmpNum = getAdsrRelease();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_adsr_release = static_cast<uint8_t>(tmpNum);


    // wah
    tmpNum = getWahType();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_wah_type = static_cast<uint8_t>(tmpNum);

    tmpNum = getWahFrequency();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_wah_freq = static_cast<uint8_t>(tmpNum);

    tmpNum = getWahResonance();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_wah_res = static_cast<uint8_t>(tmpNum);


    // multi-note
    tmpNum = getMultinoteAct();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_multinote_act = static_cast<uint8_t>(tmpNum);

    const DuArrayConstPtr<DuNumeric> &multinoteArray = getMultinote();
    if (multinoteArray == NULL)
        return QByteArray();
    std::memcpy(du_preset.s_multinote, multinoteArray->toDuMusicBinary().constData(), static_cast<size_t>(multinoteArray->size()));


    // arpeggiator
    tmpNum = getArpeggiatorType();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_arpegiator_type = static_cast<uint8_t>(tmpNum);

    tmpNum = getArpeggiatorBeat();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_arpegiator_beat = static_cast<uint8_t>(tmpNum);


    // auto pitch
    tmpNum = getAutopitchRate();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_autopitch_rate = static_cast<uint8_t>(tmpNum);

    tmpNum = getAutopitchRange();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_autopitch_range = static_cast<uint8_t>(tmpNum);


    // tremolo
    tmpNum = getTremoloRate();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_tremolo_rate = static_cast<uint8_t>(tmpNum);

    tmpNum = getTremoloRange();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_tremolo_range = static_cast<uint8_t>(tmpNum);


    // autopan
    tmpNum = getAutopanRate();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_autopan_rate = static_cast<uint8_t>(tmpNum);

    tmpNum = getAutopanRange();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_autopan_range = static_cast<uint8_t>(tmpNum);


    // auto wah
    tmpNum = getAutowahRate();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_autowah_rate = static_cast<uint8_t>(tmpNum);

    tmpNum = getAutowahRange();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_autowah_range = static_cast<uint8_t>(tmpNum);


    // FX structs
    tmpNum = getChorusPreset();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_chorus_preset = static_cast<uint8_t>(tmpNum);

    tmpNum = getCompressorOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_compressor_onoff = static_cast<uint8_t>(tmpNum);

    tmpNum = getDelayOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_delay_onoff = static_cast<uint8_t>(tmpNum);

    tmpNum = getDistortionOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_distortion_onoff = static_cast<uint8_t>(tmpNum);

    tmpNum = getEqualizerOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_eq_onoff = static_cast<uint8_t>(tmpNum);

    tmpNum = getChorusOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_chorus_onoff = static_cast<uint8_t>(tmpNum);

    tmpNum = getReverbOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_reverb_onoff = static_cast<uint8_t>(tmpNum);


    // fx
    const DuMixerConstPtr& mixer = getMixer();
    if (mixer == NULL)
        return QByteArray();
    std::memcpy(&(du_preset.s_mix), mixer->toDuMusicBinary().constData(), static_cast<size_t>(mixer->size()));

    const DuDistortionConstPtr& distortion = getDistortion();
    if (distortion == NULL)
        return QByteArray();
    std::memcpy(&(du_preset.s_distortion), distortion->toDuMusicBinary().constData(), static_cast<size_t>(distortion->size()));

    const DuCompressorConstPtr& compressor = getCompressor();
    if (compressor == NULL)
        return QByteArray();
    std::memcpy(&(du_preset.s_compressor), compressor->toDuMusicBinary().constData(), static_cast<size_t>(compressor->size()));

    const DuEqualizerConstPtr& equalizer = getEqualizer();
    if (equalizer == NULL)
        return QByteArray();
    std::memcpy(&(du_preset.s_equalizer), equalizer->toDuMusicBinary().constData(), static_cast<size_t>(equalizer->size()));

    const DuDelayConstPtr& delay = getDelay();
    if (delay == NULL)
        return QByteArray();
    std::memcpy(&(du_preset.s_delay), delay->toDuMusicBinary().constData(), static_cast<size_t>(delay->size()));

    const DuArrayConstPtr<DuChorus> &chorusArray = getChorusArray();
    if (chorusArray == NULL)
        return QByteArray();
    std::memcpy(&(du_preset.s_chorus), chorusArray->toDuMusicBinary().constData(), static_cast<size_t>(chorusArray->size()));


    return QByteArray(reinterpret_cast<char*>(&du_preset), size());
}


int DuPreset::size() const
{
    return PRESET_STRUCT_SIZE;
}


#define X(key, dutype, type, defaultReturn) DU_KEY_ACCESSORS_IMPL(DuPreset, key, dutype, type, defaultReturn)
#define X_OBJECT(key, dutype) DU_KEY_ACCESSORS_OBJECT_IMPL(DuPreset, key, dutype)
#define X_OBJECT_TEMPLATE(key, dutype, tpltype) DU_KEY_ACCESSORS_OBJECT_TEMPLATE_IMPL(DuPreset, key, dutype, tpltype)
DuPreset_Children
#undef X_OBJECT_TEMPLATE
#undef X_OBJECT
#undef X
