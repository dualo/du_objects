#include "duexpression.h"

#include <cstring>

#include <QJsonObject>


DU_OBJECT_IMPL(DuExpression)

DuExpression::DuExpression() :
    DuContainer()
{
    addChild(KEY_EXPR_VOLUME,
             new DuNumeric(MAIN_VOLUME_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_VOLUME_MAX, MAIN_VOLUME_MIN));

    addChild(KEY_EXPR_PANNING,
             new DuNumeric(MAIN_PANNING_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_PANNING_MAX, MAIN_PANNING_MIN));

    addChild(KEY_EXPR_SENDTOREVERB,
             new DuNumeric(MAIN_SENDTOREV_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_SENDTOREV_MAX, MAIN_SENDTOREV_MIN));

    addChild(KEY_EXPR_OCTAVE,
             new DuNumeric(MAIN_OCTAVE_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_OCTAVE_MAX, MAIN_OCTAVE_MIN));

    addChild(KEY_EXPR_PORTAMENTOONOFF,
             new DuNumeric(MAIN_PORT_ONOFF_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_PORT_ONOFF_MAX, MAIN_PORT_ONOFF_MIN));

    addChild(KEY_EXPR_PORTAMENTOCONTROL,
             new DuNumeric(MAIN_PORT_CTRL_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_PORT_CTRL_MAX, MAIN_PORT_CTRL_MIN));

    addChild(KEY_EXPR_PORTAMENTOTIME,
             new DuNumeric(MAIN_PORT_TIME_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_PORT_TIME_MAX, MAIN_PORT_TIME_MIN));

    addChild(KEY_EXPR_EXPRESSION,
             new DuNumeric(MAIN_EXPRESSION_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_EXPRESSION_MAX, MAIN_EXPRESSION_MIN));

    addChild(KEY_EXPR_PITCHBENDSENSITIVITY,
             new DuNumeric(MAIN_PB_SENS_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_PB_SENS_MAX, MAIN_PB_SENS_MIN));

    addChild(KEY_EXPR_DISPOSITION,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));


    addChild(KEY_EXPR_ARPEGIATORTYPE,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KEY_EXPR_ARPEGIATORBEAT,
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
    QJsonValue jsonVolume       = jsonExpression[KEY_EXPR_VOLUME];
    QJsonValue jsonPanning      = jsonExpression[KEY_EXPR_PANNING];
    QJsonValue jsonToReverb     = jsonExpression[KEY_EXPR_SENDTOREVERB];
    QJsonValue jsonOctave       = jsonExpression[KEY_EXPR_OCTAVE];
    QJsonValue jsonPortaOnOff   = jsonExpression[KEY_EXPR_PORTAMENTOONOFF];
    QJsonValue jsonPortaCtrl    = jsonExpression[KEY_EXPR_PORTAMENTOCONTROL];
    QJsonValue jsonPortaTime    = jsonExpression[KEY_EXPR_PORTAMENTOTIME];
    QJsonValue jsonExpr         = jsonExpression[KEY_EXPR_EXPRESSION];
    QJsonValue jsonPitchBend    = jsonExpression[KEY_EXPR_PITCHBENDSENSITIVITY];
    QJsonValue jsonDisposition  = jsonExpression[KEY_EXPR_DISPOSITION];

    QJsonValue jsonArpeggiatorType      = jsonExpression[KEY_EXPR_ARPEGIATORTYPE];
    QJsonValue jsonArpeggiatorBeat      = jsonExpression[KEY_EXPR_ARPEGIATORBEAT];

    if (        !jsonVolume.isDouble()      ||  !jsonPanning.isDouble()
            ||  !jsonExpr.isDouble()  ||  !jsonOctave.isDouble()
            ||  !jsonToReverb.isDouble()    ||  !jsonPortaOnOff.isDouble()
            ||  !jsonPortaCtrl.isDouble()   ||  !jsonPortaTime.isDouble()
            ||  !jsonPitchBend.isDouble()   ||  !jsonDisposition.isDouble()

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


int DuExpression::getVolume() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EXPR_VOLUME);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuExpression::setVolume(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EXPR_VOLUME);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuExpression::getPanning() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EXPR_PANNING);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuExpression::setPanning(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EXPR_PANNING);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuExpression::getSendToReverb() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EXPR_SENDTOREVERB);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuExpression::setSendToReverb(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EXPR_SENDTOREVERB);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuExpression::getOctave() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EXPR_OCTAVE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuExpression::setOctave(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EXPR_OCTAVE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuExpression::getPortamentoOnOff() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EXPR_PORTAMENTOONOFF);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuExpression::setPortamentoOnOff(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EXPR_PORTAMENTOONOFF);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuExpression::getPortamentoControl() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EXPR_PORTAMENTOCONTROL);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuExpression::setPortamentoControl(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EXPR_PORTAMENTOCONTROL);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuExpression::getPortamentoTime() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EXPR_PORTAMENTOTIME);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuExpression::setPortamentoTime(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EXPR_PORTAMENTOTIME);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuExpression::getExpression() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EXPR_EXPRESSION);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuExpression::setExpression(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EXPR_EXPRESSION);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuExpression::getPitchBendSensitivity() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EXPR_PITCHBENDSENSITIVITY);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuExpression::setPitchBendSensitivity(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EXPR_PITCHBENDSENSITIVITY);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuExpression::getDisposition() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EXPR_DISPOSITION);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuExpression::setDisposition(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EXPR_DISPOSITION);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuExpression::getArpeggiatorType() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EXPR_ARPEGIATORTYPE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuExpression::setArpeggiatorType(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EXPR_ARPEGIATORTYPE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuExpression::getArpeggiatorBeat() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EXPR_ARPEGIATORBEAT);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuExpression::setArpeggiatorBeat(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EXPR_ARPEGIATORBEAT);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}
