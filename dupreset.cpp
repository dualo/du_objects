#include "dupreset.h"

DuPreset::DuPreset() :
    DuContainer()
{
    addChild(KEY_PRESET_VOLUME,
             new DuNumeric(MAIN_VOLUME_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_VOLUME_MAX, MAIN_VOLUME_MIN));

    addChild(KEY_PRESET_PANNING,
             new DuNumeric(MAIN_PANNING_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_PANNING_MAX, MAIN_PANNING_MIN));

    addChild(KEY_PRESET_EXPRESSION,
             new DuNumeric(MAIN_EXPRESSION_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_EXPRESSION_MAX, MAIN_EXPRESSION_MIN));

    addChild(KEY_PRESET_OCTAVE,
             new DuNumeric(MAIN_OCTAVE_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_OCTAVE_MAX, MAIN_OCTAVE_MIN));

    addChild(KEY_PRESET_SENDTOREVERB,
             new DuNumeric(MAIN_SENDTOREV_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_SENDTOREV_MAX, MAIN_SENDTOREV_MIN));

    addChild(KEY_PRESET_PORTAMENTOONOFF,
             new DuNumeric(MAIN_PORT_ONOFF_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_PORT_ONOFF_MAX, MAIN_PORT_ONOFF_MIN));

    addChild(KEY_PRESET_PORTAMENTOCONTROL,
             new DuNumeric(MAIN_PORT_CTRL_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_PORT_CTRL_MAX, MAIN_PORT_CTRL_MIN));

    addChild(KEY_PRESET_PORTAMENTOTIME,
             new DuNumeric(MAIN_PORT_TIME_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_PORT_TIME_MAX, MAIN_PORT_TIME_MIN));

    addChild(KEY_PRESET_PITCHBENDSENSITIVITY,
             new DuNumeric(MAIN_PB_SENS_DEFAULT, NUMERIC_DEFAULT_SIZE,
                           MAIN_PB_SENS_MAX, MAIN_PB_SENS_MIN));

    addChild(KEY_PRESET_DISPOSITION,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));
}

DuPreset::~DuPreset()
{
}


DuPreset *DuPreset::fromDuMusicFile(const preset_instr &du_preset)
{
    DuPreset *preset = new DuPreset;
    bool verif = true;

    verif = verif && preset->setVolume(du_preset.s_volume);
    verif = verif && preset->setPanning(du_preset.s_panning);
    verif = verif && preset->setExpression(du_preset.s_expression);
    verif = verif && preset->setOctave(du_preset.s_instr_octave);
    verif = verif && preset->setSendToReverb(du_preset.s_sendtorev);

    verif = verif && preset->setPortamentoOnOff(du_preset.s_portamento_on_off);
    verif = verif && preset->setPortamentoControl(du_preset.s_portamento_ctrl);
    verif = verif && preset->setPortamentoTime(du_preset.s_portamento_time);

    verif = verif && preset->setPitchBendSensitivity(du_preset.s_pitch_bend_sensitivity);
    verif = verif && preset->setDisposition(du_preset.s_disposition);

    if (!verif)
    {
        delete preset;
        return NULL;
    }

    return preset;
}


DuPreset *DuPreset::fromJson(const QJsonObject &jsonPreset)
{
    QJsonValue jsonVolume       = jsonPreset[KEY_PRESET_VOLUME];
    QJsonValue jsonPanning      = jsonPreset[KEY_PRESET_PANNING];
    QJsonValue jsonExpression   = jsonPreset[KEY_PRESET_EXPRESSION];
    QJsonValue jsonOctave       = jsonPreset[KEY_PRESET_OCTAVE];
    QJsonValue jsonToReverb     = jsonPreset[KEY_PRESET_SENDTOREVERB];
    QJsonValue jsonPortaOnOff   = jsonPreset[KEY_PRESET_PORTAMENTOONOFF];
    QJsonValue jsonPortaCtrl    = jsonPreset[KEY_PRESET_PORTAMENTOCONTROL];
    QJsonValue jsonPortaTime    = jsonPreset[KEY_PRESET_PORTAMENTOTIME];
    QJsonValue jsonPitchBend    = jsonPreset[KEY_PRESET_PITCHBENDSENSITIVITY];
    QJsonValue jsonDisposition  = jsonPreset[KEY_PRESET_DISPOSITION];

    if (        !jsonVolume.isDouble()      ||  !jsonPanning.isDouble()
            ||  !jsonExpression.isDouble()  ||  !jsonOctave.isDouble()
            ||  !jsonToReverb.isDouble()    ||  !jsonPortaOnOff.isDouble()
            ||  !jsonPortaCtrl.isDouble()   ||  !jsonPortaTime.isDouble()
            ||  !jsonPitchBend.isDouble()   ||  !jsonDisposition.isDouble())

        return NULL;


    DuPreset *preset = new DuPreset;
    bool verif = true;

    verif = verif && preset->setVolume(jsonVolume.toInt());
    verif = verif && preset->setPanning(jsonPanning.toInt());
    verif = verif && preset->setExpression(jsonExpression.toInt());
    verif = verif && preset->setOctave(jsonOctave.toInt());
    verif = verif && preset->setSendToReverb(jsonToReverb.toInt());

    verif = verif && preset->setPortamentoOnOff(jsonPortaOnOff.toInt());
    verif = verif && preset->setPortamentoControl(jsonPortaCtrl.toInt());
    verif = verif && preset->setPortamentoTime(jsonPortaTime.toInt());

    verif = verif && preset->setPitchBendSensitivity(jsonPitchBend.toInt());
    verif = verif && preset->setDisposition(jsonDisposition.toInt());

    if (!verif)
    {
        delete preset;
        return NULL;
    }

    return preset;
}


int DuPreset::getVolume() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_VOLUME));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setVolume(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_VOLUME));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuPreset::getPanning() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_PANNING));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setPanning(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_PANNING));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuPreset::getExpression() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_EXPRESSION));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setExpression(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_EXPRESSION));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuPreset::getOctave() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_OCTAVE));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setOctave(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_OCTAVE));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuPreset::getSendToReverb() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_SENDTOREVERB));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setSendToReverb(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_SENDTOREVERB));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuPreset::getPortamentoOnOff() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_PORTAMENTOONOFF));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setPortamentoOnOff(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_PORTAMENTOONOFF));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuPreset::getPortamentoControl() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_PORTAMENTOCONTROL));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setPortamentoControl(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_PORTAMENTOCONTROL));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuPreset::getPortamentoTime() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_PORTAMENTOTIME));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setPortamentoTime(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_PORTAMENTOTIME));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuPreset::getPitchBendSensitivity() const
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_PITCHBENDSENSITIVITY));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setPitchBendSensitivity(int value)
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_PITCHBENDSENSITIVITY));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuPreset::getDisposition() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_DISPOSITION));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuPreset::setDisposition(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_DISPOSITION));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}
