#include "dupreset.h"

#include <cstring>

#include <QJsonObject>

DU_OBJECT_IMPL(DuPreset)

DuPreset::DuPreset() :
    DuContainer()
{
    addChild(KEY_PRESET_VOLUME,
             new DuNumeric(MAIN_VOLUME_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_VOLUME_MAX, MAIN_VOLUME_MIN));

    addChild(KEY_PRESET_PANNING,
             new DuNumeric(MAIN_PANNING_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_PANNING_MAX, MAIN_PANNING_MIN));

    addChild(KEY_PRESET_SENDTOREVERB,
             new DuNumeric(MAIN_SENDTOREV_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_SENDTOREV_MAX, MAIN_SENDTOREV_MIN));

    addChild(KEY_PRESET_OCTAVE,
             new DuNumeric(MAIN_OCTAVE_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_OCTAVE_MAX, MAIN_OCTAVE_MIN));

    addChild(KEY_PRESET_PORTAMENTOONOFF,
             new DuNumeric(MAIN_PORT_ONOFF_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_PORT_ONOFF_MAX, MAIN_PORT_ONOFF_MIN));

    addChild(KEY_PRESET_PORTAMENTOCONTROL,
             new DuNumeric(MAIN_PORT_CTRL_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_PORT_CTRL_MAX, MAIN_PORT_CTRL_MIN));

    addChild(KEY_PRESET_PORTAMENTOTIME,
             new DuNumeric(MAIN_PORT_TIME_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_PORT_TIME_MAX, MAIN_PORT_TIME_MIN));

    addChild(KEY_PRESET_EXPRESSION,
             new DuNumeric(MAIN_EXPRESSION_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_EXPRESSION_MAX, MAIN_EXPRESSION_MIN));

    addChild(KEY_PRESET_PITCHBENDSENSITIVITY,
             new DuNumeric(MAIN_PB_SENS_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_PB_SENS_MAX, MAIN_PB_SENS_MIN));

    addChild(KEY_PRESET_DISPOSITION,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));


    addChild(KEY_PRESET_ARPEGIATORTYPE,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KEY_PRESET_ARPEGIATORBEAT,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));


    addChild(KEY_PRESET_DIRECTIONGYROP,
             new DuNumeric(DIRECTIONGYRO_MINVALUE, NUMERIC_DEFAULT_SIZE,
                           DIRECTIONGYRO_MAXVALUE, DIRECTIONGYRO_MINVALUE));

    addChild(KEY_PRESET_DIRECTIONGYROR,
             new DuNumeric(DIRECTIONGYRO_MINVALUE, NUMERIC_DEFAULT_SIZE,
                           DIRECTIONGYRO_MAXVALUE, DIRECTIONGYRO_MINVALUE));

    addChild(KEY_PRESET_DIRECTIONGYROY,
             new DuNumeric(DIRECTIONGYRO_MINVALUE, NUMERIC_DEFAULT_SIZE,
                           DIRECTIONGYRO_MAXVALUE, DIRECTIONGYRO_MINVALUE));

    addChild(KEY_PRESET_ACTIVEAFTERTOUCH,
             new DuNumeric(0x00, PARAMS_NUMERIC_SIZE,
                           0x7F, 0x00));

    addChild(KEY_PRESET_ACTIVESLIDERL,
             new DuNumeric(0x00, PARAMS_NUMERIC_SIZE,
                           0x7F, 0x00));

    addChild(KEY_PRESET_ACTIVESLIDERR,
             new DuNumeric(0x00, PARAMS_NUMERIC_SIZE,
                           0x7F, 0x00));

    addChild(KEY_PRESET_ACTIVEGYROP,
             new DuNumeric(0x00, PARAMS_NUMERIC_SIZE,
                           0x7F, 0x00));

    addChild(KEY_PRESET_ACTIVEGYROR,
             new DuNumeric(0x00, PARAMS_NUMERIC_SIZE,
                           0x7F, 0x00));

    addChild(KEY_PRESET_ACTIVEGYROY,
             new DuNumeric(0x00, PARAMS_NUMERIC_SIZE,
                           0x7F, 0x00));
}

DuPreset::~DuPreset()
{
}

DuObjectPtr DuPreset::clone() const
{
    return DuPresetPtr(new DuPreset(*this));
}


DuPresetPtr DuPreset::fromDuMusicBinary(const preset_instr &du_preset)
{
    DuPresetPtr preset(new DuPreset);
    bool verif = true;

    verif = verif && preset->setVolume(du_preset.s_volume);
    verif = verif && preset->setPanning(du_preset.s_panning);
    verif = verif && preset->setSendToReverb(du_preset.s_sendtorev);
    verif = verif && preset->setOctave(du_preset.s_instr_octave);

    verif = verif && preset->setPortamentoOnOff(du_preset.s_portamento_on_off);
    verif = verif && preset->setPortamentoControl(du_preset.s_portamento_ctrl);
    verif = verif && preset->setPortamentoTime(du_preset.s_portamento_time);
    verif = verif && preset->setExpression(du_preset.s_expression);
    verif = verif && preset->setPitchBendSensitivity(du_preset.s_pitch_bend_sensitivity);
    verif = verif && preset->setDisposition(du_preset.s_disposition);


    verif = verif && preset->setArpeggiatorType(du_preset.s_arpegiator_type);
    verif = verif && preset->setArpeggiatorBeat(du_preset.s_arpegiator_beat);

    verif = verif && preset->setDirectionGyroP(du_preset.s_direction_gyro_P);
    verif = verif && preset->setDirectionGyroR(du_preset.s_direction_gyro_R);
    verif = verif && preset->setDirectionGyroY(du_preset.s_direction_gyro_Y);

    verif = verif && preset->setActiveAftertouch(du_preset.s_activ_aftertouch);
    verif = verif && preset->setActiveSliderL(du_preset.s_activ_slider_L);
    verif = verif && preset->setActiveSliderR(du_preset.s_activ_slider_R);

    verif = verif && preset->setActiveGyroP(du_preset.s_activ_gyro_P);
    verif = verif && preset->setActiveGyroR(du_preset.s_activ_gyro_R);
    verif = verif && preset->setActiveGyroY(du_preset.s_activ_gyro_Y);

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuPreset::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    return preset;
}


DuPresetPtr DuPreset::fromJson(const QJsonObject &jsonPreset)
{
    QJsonValue jsonVolume       = jsonPreset[KEY_PRESET_VOLUME];
    QJsonValue jsonPanning      = jsonPreset[KEY_PRESET_PANNING];
    QJsonValue jsonToReverb     = jsonPreset[KEY_PRESET_SENDTOREVERB];
    QJsonValue jsonOctave       = jsonPreset[KEY_PRESET_OCTAVE];
    QJsonValue jsonPortaOnOff   = jsonPreset[KEY_PRESET_PORTAMENTOONOFF];
    QJsonValue jsonPortaCtrl    = jsonPreset[KEY_PRESET_PORTAMENTOCONTROL];
    QJsonValue jsonPortaTime    = jsonPreset[KEY_PRESET_PORTAMENTOTIME];
    QJsonValue jsonExpression   = jsonPreset[KEY_PRESET_EXPRESSION];
    QJsonValue jsonPitchBend    = jsonPreset[KEY_PRESET_PITCHBENDSENSITIVITY];
    QJsonValue jsonDisposition  = jsonPreset[KEY_PRESET_DISPOSITION];

    QJsonValue jsonArpeggiatorType      = jsonPreset[KEY_PRESET_ARPEGIATORTYPE];
    QJsonValue jsonArpeggiatorBeat      = jsonPreset[KEY_PRESET_ARPEGIATORBEAT];

    QJsonValue jsonDirectionGyroP       = jsonPreset[KEY_PRESET_DIRECTIONGYROP];
    QJsonValue jsonDirectionGyroR       = jsonPreset[KEY_PRESET_DIRECTIONGYROR];
    QJsonValue jsonDirectionGyroY       = jsonPreset[KEY_PRESET_DIRECTIONGYROY];
    QJsonValue jsonActiveAftertouch     = jsonPreset[KEY_PRESET_ACTIVEAFTERTOUCH];
    QJsonValue jsonActiveSliderL        = jsonPreset[KEY_PRESET_ACTIVESLIDERL];
    QJsonValue jsonActiveSliderR        = jsonPreset[KEY_PRESET_ACTIVESLIDERR];
    QJsonValue jsonActiveGyroP          = jsonPreset[KEY_PRESET_ACTIVEGYROP];
    QJsonValue jsonActiveGyroR          = jsonPreset[KEY_PRESET_ACTIVEGYROR];
    QJsonValue jsonActiveGyroY          = jsonPreset[KEY_PRESET_ACTIVEGYROY];

    if (        !jsonVolume.isDouble()      ||  !jsonPanning.isDouble()
            ||  !jsonExpression.isDouble()  ||  !jsonOctave.isDouble()
            ||  !jsonToReverb.isDouble()    ||  !jsonPortaOnOff.isDouble()
            ||  !jsonPortaCtrl.isDouble()   ||  !jsonPortaTime.isDouble()
            ||  !jsonPitchBend.isDouble()   ||  !jsonDisposition.isDouble()

            || !jsonArpeggiatorType.isDouble()  || !jsonArpeggiatorBeat.isDouble()

            ||  !jsonDirectionGyroP.isDouble()  ||  !jsonDirectionGyroR.isDouble()
            ||  !jsonDirectionGyroY.isDouble()  ||  !jsonActiveAftertouch.isDouble()
            ||  !jsonActiveSliderL.isDouble()   ||  !jsonActiveSliderR.isDouble()
            ||  !jsonActiveGyroP.isDouble()     ||  !jsonActiveGyroR.isDouble()
            ||  !jsonActiveGyroY.isDouble())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuPreset::fromJson():\n"
                    << "failed to generate DuPreset\n"
                    << "a json key did not contain the proper type";

        return DuPresetPtr();
    }


    DuPresetPtr preset(new DuPreset);
    bool verif = true;

    verif = verif && preset->setVolume(jsonVolume.toInt());
    verif = verif && preset->setPanning(jsonPanning.toInt());
    verif = verif && preset->setSendToReverb(jsonToReverb.toInt());
    verif = verif && preset->setOctave(jsonOctave.toInt());

    verif = verif && preset->setPortamentoOnOff(jsonPortaOnOff.toInt());
    verif = verif && preset->setPortamentoControl(jsonPortaCtrl.toInt());
    verif = verif && preset->setPortamentoTime(jsonPortaTime.toInt());
    verif = verif && preset->setExpression(jsonExpression.toInt());
    verif = verif && preset->setPitchBendSensitivity(jsonPitchBend.toInt());
    verif = verif && preset->setDisposition(jsonDisposition.toInt());


    verif = verif && preset->setArpeggiatorType(jsonArpeggiatorType.toInt());
    verif = verif && preset->setArpeggiatorBeat(jsonArpeggiatorBeat.toInt());


    verif = verif && preset->setDirectionGyroP(jsonDirectionGyroP.toInt());
    verif = verif && preset->setDirectionGyroR(jsonDirectionGyroR.toInt());
    verif = verif && preset->setDirectionGyroY(jsonDirectionGyroY.toInt());

    verif = verif && preset->setActiveAftertouch(jsonActiveAftertouch.toInt());
    verif = verif && preset->setActiveSliderL(jsonActiveSliderL.toInt());
    verif = verif && preset->setActiveSliderR(jsonActiveSliderR.toInt());

    verif = verif && preset->setActiveGyroP(jsonActiveGyroP.toInt());
    verif = verif && preset->setActiveGyroR(jsonActiveGyroR.toInt());
    verif = verif && preset->setActiveGyroY(jsonActiveGyroY.toInt());

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuPreset::fromJson():\n"
                   << "an attribute was not properly set";
    }

    return preset;
}


QByteArray DuPreset::toDuMusicBinary() const
{
    preset_instr du_preset;

    int tmpNum = 0;

    QByteArray tmpClear(size(), (char)0x00);
    std::memcpy((char *)&(du_preset), tmpClear.data(), size());


    tmpNum = getVolume();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_volume = tmpNum;

    tmpNum = getPanning();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_panning = tmpNum;

    tmpNum = getSendToReverb();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_sendtorev = tmpNum;

    tmpNum = getOctave();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_instr_octave = tmpNum;

    tmpNum = getPortamentoOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_portamento_on_off = tmpNum;

    tmpNum = getPortamentoControl();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_portamento_ctrl = tmpNum;

    tmpNum = getPortamentoTime();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_portamento_time = tmpNum;

    tmpNum = getExpression();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_expression = tmpNum;

    tmpNum = getPitchBendSensitivity();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_pitch_bend_sensitivity = tmpNum;

    tmpNum = getDisposition();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_disposition = tmpNum;


    tmpNum = getArpeggiatorType();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_arpegiator_type = tmpNum;

    tmpNum = getArpeggiatorBeat();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_arpegiator_beat = tmpNum;


    tmpNum = getDirectionGyroP();
    if (tmpNum == 0)
        return QByteArray();
    du_preset.s_direction_gyro_P = tmpNum;

    tmpNum = getDirectionGyroR();
    if (tmpNum == 0)
        return QByteArray();
    du_preset.s_direction_gyro_R = tmpNum;

    tmpNum = getDirectionGyroY();
    if (tmpNum == 0)
        return QByteArray();
    du_preset.s_direction_gyro_Y = tmpNum;


    tmpNum = getActiveAftertouch();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_activ_aftertouch = tmpNum;

    tmpNum = getActiveSliderL();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_activ_slider_L = tmpNum;

    tmpNum = getActiveSliderR();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_activ_slider_R = tmpNum;


    tmpNum = getActiveGyroP();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_activ_gyro_P = tmpNum;

    tmpNum = getActiveGyroR();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_activ_gyro_R = tmpNum;

    tmpNum = getActiveGyroY();
    if (tmpNum == -1)
        return QByteArray();
    du_preset.s_activ_gyro_Y = tmpNum;


    return QByteArray((char *)&(du_preset), size());
}


int DuPreset::size() const
{
    return PRESET_STRUCT_SIZE;
}


int DuPreset::getVolume() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_PRESET_VOLUME);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setVolume(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_VOLUME);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuPreset::getPanning() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_PRESET_PANNING);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setPanning(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_PANNING);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuPreset::getSendToReverb() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_PRESET_SENDTOREVERB);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setSendToReverb(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_SENDTOREVERB);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuPreset::getOctave() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_PRESET_OCTAVE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setOctave(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_OCTAVE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuPreset::getPortamentoOnOff() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_PRESET_PORTAMENTOONOFF);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setPortamentoOnOff(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_PORTAMENTOONOFF);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuPreset::getPortamentoControl() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_PRESET_PORTAMENTOCONTROL);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setPortamentoControl(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_PORTAMENTOCONTROL);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuPreset::getPortamentoTime() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_PRESET_PORTAMENTOTIME);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setPortamentoTime(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_PORTAMENTOTIME);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuPreset::getExpression() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_PRESET_EXPRESSION);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setExpression(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_EXPRESSION);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuPreset::getPitchBendSensitivity() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_PRESET_PITCHBENDSENSITIVITY);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setPitchBendSensitivity(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_PITCHBENDSENSITIVITY);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuPreset::getDisposition() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_PRESET_DISPOSITION);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setDisposition(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_DISPOSITION);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuPreset::getArpeggiatorType() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_PRESET_ARPEGIATORTYPE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setArpeggiatorType(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_ARPEGIATORTYPE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuPreset::getArpeggiatorBeat() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_PRESET_ARPEGIATORBEAT);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setArpeggiatorBeat(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_ARPEGIATORBEAT);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


//Warning: the direction gyro parameters should be either 1 or -1 so the error
//return value can't be -1.

int DuPreset::getDirectionGyroP() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_DIRECTIONGYROP);

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

bool DuPreset::setDirectionGyroP(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_DIRECTIONGYROP);

    if (tmp == NULL)
        return false;

    if (value == 0)
    {
        tmp->setNumeric(DIRECTIONGYRO_MINVALUE);
        return false;
    }

    return tmp->setNumeric(value);
}


int DuPreset::getDirectionGyroR() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_DIRECTIONGYROR);

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

bool DuPreset::setDirectionGyroR(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_DIRECTIONGYROR);

    if (tmp == NULL)
        return false;

    if (value == 0)
    {
        tmp->setNumeric(DIRECTIONGYRO_MINVALUE);
        return false;
    }

    return tmp->setNumeric(value);
}


int DuPreset::getDirectionGyroY() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_DIRECTIONGYROY);

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

bool DuPreset::setDirectionGyroY(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_DIRECTIONGYROY);

    if (tmp == NULL)
        return false;

    if (value == 0)
    {
        tmp->setNumeric(DIRECTIONGYRO_MINVALUE);
        return false;
    }

    return tmp->setNumeric(value);
}


int DuPreset::getActiveAftertouch() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_ACTIVEAFTERTOUCH);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setActiveAftertouch(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_ACTIVEAFTERTOUCH);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuPreset::getActiveSliderL() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_ACTIVESLIDERL);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setActiveSliderL(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_ACTIVESLIDERL);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuPreset::getActiveSliderR() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_ACTIVESLIDERR);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setActiveSliderR(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_ACTIVESLIDERR);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuPreset::getActiveGyroP() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_ACTIVEGYROP);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setActiveGyroP(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_ACTIVEGYROP);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuPreset::getActiveGyroR() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_ACTIVEGYROR);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setActiveGyroR(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_ACTIVEGYROR);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuPreset::getActiveGyroY() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_ACTIVEGYROY);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setActiveGyroY(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PRESET_ACTIVEGYROY);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}
