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
    DuPreset *preset = new DuPreset();

    preset->setVolume(du_preset.s_volume);
    preset->setPanning(du_preset.s_panning);
    preset->setExpression(du_preset.s_expression);
    preset->setOctave(du_preset.s_instr_octave);
    preset->setSendToReverb(du_preset.s_sendtorev);

    preset->setPortamentoOnOff(du_preset.s_portamento_on_off);
    preset->setPortamentoControl(du_preset.s_portamento_ctrl);
    preset->setPortamentoTime(du_preset.s_portamento_time);

    preset->setPitchBendSensitivity(du_preset.s_pitch_bend_sensitivity);
    preset->setDisposition(du_preset.s_disposition);

    return preset;
}


DuPreset *DuPreset::fromJson(const QJsonObject &jsonPreset)
{
    DuPreset *preset = new DuPreset();
    QStringList &keyList = preset->keys();

    bool test = true;
    for (int i = 0; i < keyList.count(); i++)
    {
        test = test && jsonPreset.contains(keyList[i]);
    }

    if (!test)
        return NULL;

    preset->setVolume(jsonPreset[KEY_PRESET_VOLUME].toInt());
    preset->setPanning(jsonPreset[KEY_PRESET_PANNING].toInt());
    preset->setExpression(jsonPreset[KEY_PRESET_EXPRESSION].toInt());
    preset->setOctave(jsonPreset[KEY_PRESET_OCTAVE].toInt());
    preset->setSendToReverb(jsonPreset[KEY_PRESET_SENDTOREVERB].toInt());

    preset->setPortamentoOnOff(jsonPreset[KEY_PRESET_PORTAMENTOONOFF].toInt());
    preset->setPortamentoControl(jsonPreset[KEY_PRESET_PORTAMENTOCONTROL].toInt());
    preset->setPortamentoTime(jsonPreset[KEY_PRESET_PORTAMENTOTIME].toInt());

    preset->setPitchBendSensitivity(jsonPreset[KEY_PRESET_PITCHBENDSENSITIVITY].toInt());
    preset->setDisposition(jsonPreset[KEY_PRESET_DISPOSITION].toInt());

    return preset;
}


int DuPreset::getVolume() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_VOLUME));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuPreset::setVolume(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_VOLUME));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuPreset::getPanning() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_PANNING));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuPreset::setPanning(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_PANNING));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuPreset::getExpression() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_EXPRESSION));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuPreset::setExpression(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_EXPRESSION));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuPreset::getOctave() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_OCTAVE));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuPreset::setOctave(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_OCTAVE));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuPreset::getSendToReverb() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_SENDTOREVERB));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuPreset::setSendToReverb(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_SENDTOREVERB));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


int DuPreset::getPortamentoOnOff() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_PORTAMENTOONOFF));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuPreset::setPortamentoOnOff(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_PORTAMENTOONOFF));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuPreset::getPortamentoControl() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_PORTAMENTOCONTROL));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuPreset::setPortamentoControl(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_PORTAMENTOCONTROL));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuPreset::getPortamentoTime() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_PORTAMENTOTIME));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuPreset::setPortamentoTime(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_PORTAMENTOTIME));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


int DuPreset::getPitchBendSensitivity() const
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_PITCHBENDSENSITIVITY));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuPreset::setPitchBendSensitivity(int value)
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_PITCHBENDSENSITIVITY));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuPreset::getDisposition() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_DISPOSITION));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuPreset::setDisposition(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_PRESET_DISPOSITION));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}
