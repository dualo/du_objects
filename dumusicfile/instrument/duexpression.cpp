#include "duexpression.h"

#include <cstring>

#include <QJsonObject>


DU_OBJECT_IMPL(DuExpression)

DuExpression::DuExpression() :
    DuContainer()
{
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

    addChild(KeyPortamentoOnOff,
             new DuNumeric(MAIN_PORT_ONOFF_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_PORT_ONOFF_MAX, MAIN_PORT_ONOFF_MIN));

    addChild(KeyPortamentoControl,
             new DuNumeric(MAIN_PORT_CTRL_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_PORT_CTRL_MAX, MAIN_PORT_CTRL_MIN));

    addChild(KeyPortamentoTime,
             new DuNumeric(MAIN_PORT_TIME_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_PORT_TIME_MAX, MAIN_PORT_TIME_MIN));

    addChild(KeyExpression,
             new DuNumeric(MAIN_EXPRESSION_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_EXPRESSION_MAX, MAIN_EXPRESSION_MIN));

    addChild(KeyPitchBendSensitivity,
             new DuNumeric(MAIN_PB_SENS_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_PB_SENS_MAX, MAIN_PB_SENS_MIN));

    addChild(KeyDisposition,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));


    addChild(KeyAdsrPreset,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KeyCompressorPreset,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KeyDelayPreset,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KeyDistortionPreset,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KeyEqualizerPreset,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KeyChorusPreset,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KeyVibratoPreset,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KeyWahPreset,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));


    addChild(KeyArpeggiatorType,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KeyArpeggiatorBeat,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));
}

DuExpression::~DuExpression()
{
}


DuObjectPtr DuExpression::clone() const
{
    return DuExpressionPtr(new DuExpression(*this));
}


DuExpressionPtr DuExpression::fromDuMusicBinary(const preset_instr &du_preset)
{
    DuExpressionPtr expression(new DuExpression);
    bool verif = true;

    verif = expression->setVolume(du_preset.s_volume) ? verif : false;
    verif = expression->setPanning(du_preset.s_panning) ? verif : false;
    verif = expression->setSendToReverb(du_preset.s_sendtorev) ? verif : false;
    verif = expression->setOctave(du_preset.s_instr_octave) ? verif : false;

    verif = expression->setPortamentoOnOff(du_preset.s_portamento_on_off) ? verif : false;
    verif = expression->setPortamentoControl(du_preset.s_portamento_ctrl) ? verif : false;
    verif = expression->setPortamentoTime(du_preset.s_portamento_time) ? verif : false;
    verif = expression->setExpression(du_preset.s_expression) ? verif : false;
    verif = expression->setPitchBendSensitivity(du_preset.s_pitch_bend_sensitivity) ? verif : false;
    verif = expression->setDisposition(du_preset.s_disposition) ? verif : false;

    verif = expression->setAdsrPreset(du_preset.s_adsr_preset) ? verif : false;
    verif = expression->setCompressorPreset(du_preset.s_compressor_preset) ? verif : false;
    verif = expression->setDelayPreset(du_preset.s_delay_preset) ? verif : false;
    verif = expression->setDistortionPreset(du_preset.s_distortion_preset) ? verif : false;
    verif = expression->setEqualizerPreset(du_preset.s_eq_preset) ? verif : false;
    verif = expression->setChorusPreset(du_preset.s_chorus_preset) ? verif : false;
    verif = expression->setVibratoPreset(du_preset.s_vibrato_preset) ? verif : false;
    verif = expression->setWahPreset(du_preset.s_wah_preset) ? verif : false;

    verif = expression->setArpeggiatorType(du_preset.s_arpegiator_type) ? verif : false;
    verif = expression->setArpeggiatorBeat(du_preset.s_arpegiator_beat) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuExpression::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    return expression;
}


DuExpressionPtr DuExpression::fromJson(const QJsonObject &jsonExpression)
{
    QJsonValue jsonVolume       = jsonExpression[KeyVolume];
    QJsonValue jsonPanning      = jsonExpression[KeyPanning];
    QJsonValue jsonToReverb     = jsonExpression[KeySendToReverb];
    QJsonValue jsonOctave       = jsonExpression[KeyOctave];
    QJsonValue jsonPortaOnOff   = jsonExpression[KeyPortamentoOnOff];
    QJsonValue jsonPortaCtrl    = jsonExpression[KeyPortamentoControl];
    QJsonValue jsonPortaTime    = jsonExpression[KeyPortamentoTime];
    QJsonValue jsonExpr         = jsonExpression[KeyExpression];
    QJsonValue jsonPitchBend    = jsonExpression[KeyPitchBendSensitivity];
    QJsonValue jsonDisposition  = jsonExpression[KeyDisposition];

    QJsonValue jsonAdsrPreset   = jsonExpression[KeyAdsrPreset];
    QJsonValue jsonComprPreset  = jsonExpression[KeyCompressorPreset];
    QJsonValue jsonDelayPreset  = jsonExpression[KeyDelayPreset];
    QJsonValue jsonDistoPreset  = jsonExpression[KeyDistortionPreset];
    QJsonValue jsonEqualPreset  = jsonExpression[KeyEqualizerPreset];
    QJsonValue jsonChorusPreset = jsonExpression[KeyChorusPreset];
    QJsonValue jsonVibraPreset  = jsonExpression[KeyVibratoPreset];
    QJsonValue jsonWahPreset    = jsonExpression[KeyWahPreset];

    QJsonValue jsonArpeggiatorType      = jsonExpression[KeyArpeggiatorType];
    QJsonValue jsonArpeggiatorBeat      = jsonExpression[KeyArpeggiatorBeat];

    if (        !jsonVolume.isDouble()      ||  !jsonPanning.isDouble()
            ||  !jsonExpr.isDouble()        ||  !jsonOctave.isDouble()
            ||  !jsonToReverb.isDouble()    ||  !jsonPortaOnOff.isDouble()
            ||  !jsonPortaCtrl.isDouble()   ||  !jsonPortaTime.isDouble()
            ||  !jsonPitchBend.isDouble()   ||  !jsonDisposition.isDouble()

            ||  !jsonAdsrPreset.isDouble()  ||  !jsonComprPreset.isDouble()
            ||  !jsonDelayPreset.isDouble() ||  !jsonDistoPreset.isDouble()
            ||  !jsonEqualPreset.isDouble() ||  !jsonChorusPreset.isDouble()
            ||  !jsonVibraPreset.isDouble() ||  !jsonWahPreset.isDouble()

            || !jsonArpeggiatorType.isDouble()  || !jsonArpeggiatorBeat.isDouble())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuExpression::fromJson():\n"
                    << "failed to generate DuExpression\n"
                    << "a json key did not contain the proper type";

        return DuExpressionPtr();
    }


    DuExpressionPtr expression(new DuExpression);
    bool verif = true;

    verif = expression->setVolume(jsonVolume.toInt()) ? verif : false;
    verif = expression->setPanning(jsonPanning.toInt()) ? verif : false;
    verif = expression->setSendToReverb(jsonToReverb.toInt()) ? verif : false;
    verif = expression->setOctave(jsonOctave.toInt()) ? verif : false;

    verif = expression->setPortamentoOnOff(jsonPortaOnOff.toInt()) ? verif : false;
    verif = expression->setPortamentoControl(jsonPortaCtrl.toInt()) ? verif : false;
    verif = expression->setPortamentoTime(jsonPortaTime.toInt()) ? verif : false;
    verif = expression->setExpression(jsonExpr.toInt()) ? verif : false;
    verif = expression->setPitchBendSensitivity(jsonPitchBend.toInt()) ? verif : false;
    verif = expression->setDisposition(jsonDisposition.toInt()) ? verif : false;

    verif = expression->setAdsrPreset(jsonAdsrPreset.toInt()) ? verif : false;
    verif = expression->setCompressorPreset(jsonComprPreset.toInt()) ? verif : false;
    verif = expression->setDelayPreset(jsonDelayPreset.toInt()) ? verif : false;
    verif = expression->setDistortionPreset(jsonDistoPreset.toInt()) ? verif : false;
    verif = expression->setEqualizerPreset(jsonEqualPreset.toInt()) ? verif : false;
    verif = expression->setChorusPreset(jsonChorusPreset.toInt()) ? verif : false;
    verif = expression->setVibratoPreset(jsonVibraPreset.toInt()) ? verif : false;
    verif = expression->setWahPreset(jsonWahPreset.toInt()) ? verif : false;

    verif = expression->setArpeggiatorType(jsonArpeggiatorType.toInt()) ? verif : false;
    verif = expression->setArpeggiatorBeat(jsonArpeggiatorBeat.toInt()) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuExpression::fromJson():\n"
                   << "an attribute was not properly set";
    }

    return expression;
}


QByteArray DuExpression::toDuMusicBinary() const
{
    preset_instr du_expression;

    int tmpNum = 0;

    QByteArray tmpClear(size(), (char)0x00);
    std::memcpy((char *)&(du_expression) + EXPRESSION_PRESET_OFFSET, tmpClear.data(), size());


    tmpNum = getVolume();
    if (tmpNum == -1)
        return QByteArray();
    du_expression.s_volume = tmpNum;

    tmpNum = getPanning();
    if (tmpNum == -1)
        return QByteArray();
    du_expression.s_panning = tmpNum;

    tmpNum = getSendToReverb();
    if (tmpNum == -1)
        return QByteArray();
    du_expression.s_sendtorev = tmpNum;

    tmpNum = getOctave();
    if (tmpNum == -1)
        return QByteArray();
    du_expression.s_instr_octave = tmpNum;

    tmpNum = getPortamentoOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_expression.s_portamento_on_off = tmpNum;

    tmpNum = getPortamentoControl();
    if (tmpNum == -1)
        return QByteArray();
    du_expression.s_portamento_ctrl = tmpNum;

    tmpNum = getPortamentoTime();
    if (tmpNum == -1)
        return QByteArray();
    du_expression.s_portamento_time = tmpNum;

    tmpNum = getExpression();
    if (tmpNum == -1)
        return QByteArray();
    du_expression.s_expression = tmpNum;

    tmpNum = getPitchBendSensitivity();
    if (tmpNum == -1)
        return QByteArray();
    du_expression.s_pitch_bend_sensitivity = tmpNum;

    tmpNum = getDisposition();
    if (tmpNum == -1)
        return QByteArray();
    du_expression.s_disposition = tmpNum;


    tmpNum = getAdsrPreset();
    if (tmpNum == -1)
        return QByteArray();
    du_expression.s_adsr_preset = tmpNum;

    tmpNum = getCompressorPreset();
    if (tmpNum == -1)
        return QByteArray();
    du_expression.s_compressor_preset = tmpNum;

    tmpNum = getDelayPreset();
    if (tmpNum == -1)
        return QByteArray();
    du_expression.s_delay_preset = tmpNum;

    tmpNum = getDistortionPreset();
    if (tmpNum == -1)
        return QByteArray();
    du_expression.s_distortion_preset = tmpNum;

    tmpNum = getEqualizerPreset();
    if (tmpNum == -1)
        return QByteArray();
    du_expression.s_eq_preset = tmpNum;

    tmpNum = getChorusPreset();
    if (tmpNum == -1)
        return QByteArray();
    du_expression.s_chorus_preset = tmpNum;

    tmpNum = getVibratoPreset();
    if (tmpNum == -1)
        return QByteArray();
    du_expression.s_vibrato_preset = tmpNum;

    tmpNum = getWahPreset();
    if (tmpNum == -1)
        return QByteArray();
    du_expression.s_wah_preset = tmpNum;


    tmpNum = getArpeggiatorType();
    if (tmpNum == -1)
        return QByteArray();
    du_expression.s_arpegiator_type = tmpNum;

    tmpNum = getArpeggiatorBeat();
    if (tmpNum == -1)
        return QByteArray();
    du_expression.s_arpegiator_beat = tmpNum;


    return QByteArray((char *)&(du_expression) + EXPRESSION_PRESET_OFFSET, size());
}


int DuExpression::size() const
{
    return EXPRESSION_SIZE;
}
