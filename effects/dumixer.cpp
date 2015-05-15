#include "dumixer.h"

DuMixer::DuMixer() :
    DuEffectSettings()
{
    addChild(KEY_MIXER_INPUTGAIN,
             new DuNumeric(FX_MIX_INGAIN_MAXVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_MIX_INGAIN_MAXVALUE, FX_MIX_INGAIN_MINVALUE));

    addChild(KEY_MIXER_LOWCUTFILTERFREQUENCY,
             new DuNumeric(FX_MIX_LCFREQ_MINVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_MIX_LCFREQ_MAXVALUE, FX_MIX_LCFREQ_MINVALUE));

    addChild(KEY_MIXER_HIGHCUTFILTERFREQUENCY,
             new DuNumeric(FX_MIX_HCFREQ_MAXVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_MIX_HCFREQ_MAXVALUE, FX_MIX_HCFREQ_MINVALUE));

    addChild(KEY_MIXER_OUTPUTLEVEL,
             new DuNumeric(FX_MIX_OUTLEVEL_MAXVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_MIX_OUTLEVEL_MAXVALUE, FX_MIX_OUTLEVEL_MINVALUE));

    addChild(KEY_MIXER_OUTPUTPANNING,
             new DuNumeric(FX_MIX_OUTPAN_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_MIX_OUTPAN_MAXVALUE, FX_MIX_OUTPAN_MINVALUE));

    addChild(KEY_MIXER_OUTPUTFRONTREAR,
             new DuNumeric(FX_MIX_OUTFR_MINVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_MIX_OUTFR_MAXVALUE, FX_MIX_OUTFR_MINVALUE));

    addChild(KEY_MIXER_SENDTOREVERB,
             new DuNumeric(FX_MIX_REVERB_MINVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_MIX_REVERB_MAXVALUE, FX_MIX_REVERB_MINVALUE));

    addChild(KEY_MIXER_SENDTOCHORUS,
             new DuNumeric(FX_MIX_CHORUS_MINVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_MIX_CHORUS_MAXVALUE, FX_MIX_CHORUS_MINVALUE));
}

DuMixer::~DuMixer()
{
}


DuMixer *DuMixer::fromDuMusicFile(const FX_mix &du_mixer)
{
    DuMixer *mixer = new DuMixer;
    bool verif = true;

    verif = verif && mixer->setInputGain(du_mixer.m_inputgain);
    verif = verif && mixer->setLowCutFilterFrequency(du_mixer.m_locutfilterfrequency);
    verif = verif && mixer->setHighCutFilterFrequency(du_mixer.m_hicutfilterfrequency);

    verif = verif && mixer->setOutputLevel(du_mixer.m_ouputlevel);
    verif = verif && mixer->setOutputPanning(du_mixer.m_outputpanning);
    verif = verif && mixer->setOutputFrontRear(du_mixer.m_ouputfrontrear);

    verif = verif && mixer->setSendToReverb(du_mixer.m_sendtoreverb);
    verif = verif && mixer->setSendToChorus(du_mixer.m_sendtochorus);

    if (!verif)
    {
        delete mixer;
        return NULL;
    }

    return mixer;
}

DuMixer *DuMixer::fromJson(const QJsonObject &jsonMixer)
{
    QJsonValue jsonInputGain    = jsonMixer[KEY_MIXER_INPUTGAIN];
    QJsonValue jsonLoCutFreq    = jsonMixer[KEY_MIXER_LOWCUTFILTERFREQUENCY];
    QJsonValue jsonHiCutFreq    = jsonMixer[KEY_MIXER_HIGHCUTFILTERFREQUENCY];
    QJsonValue jsonOutputLvl    = jsonMixer[KEY_MIXER_OUTPUTLEVEL];
    QJsonValue jsonPanning      = jsonMixer[KEY_MIXER_OUTPUTPANNING];
    QJsonValue jsonFrontRear    = jsonMixer[KEY_MIXER_OUTPUTFRONTREAR];
    QJsonValue jsonToReverb     = jsonMixer[KEY_MIXER_SENDTOREVERB];
    QJsonValue jsonToChorus     = jsonMixer[KEY_MIXER_SENDTOCHORUS];

    if (        !jsonInputGain.isDouble()   ||  !jsonLoCutFreq.isDouble()
            ||  !jsonHiCutFreq.isDouble()   ||  !jsonOutputLvl.isDouble()
            ||  !jsonPanning.isDouble()     ||  !jsonFrontRear.isDouble()
            ||  !jsonToReverb.isDouble()    ||  !jsonToChorus.isDouble())

        return NULL;


    DuMixer *mixer = new DuMixer;
    bool verif = true;

    verif = verif && mixer->setInputGain(jsonInputGain.toInt());
    verif = verif && mixer->setLowCutFilterFrequency(jsonLoCutFreq.toInt());
    verif = verif && mixer->setHighCutFilterFrequency(jsonHiCutFreq.toInt());

    verif = verif && mixer->setOutputLevel(jsonOutputLvl.toInt());
    verif = verif && mixer->setOutputPanning(jsonPanning.toInt());
    verif = verif && mixer->setOutputFrontRear(jsonFrontRear.toInt());

    verif = verif && mixer->setSendToReverb(jsonToReverb.toInt());
    verif = verif && mixer->setSendToChorus(jsonToChorus.toInt());

    if (!verif)
    {
        delete mixer;
        return NULL;
    }

    return mixer;
}


int DuMixer::size() const
{
    return FX_MIX_SIZE;
}


int DuMixer::getInputGain() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_INPUTGAIN));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuMixer::setInputGain(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_INPUTGAIN));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuMixer::getLowCutFilterFrequency() const
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_LOWCUTFILTERFREQUENCY));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuMixer::setLowCutFilterFrequency(int value)
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_LOWCUTFILTERFREQUENCY));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuMixer::getHighCutFilterFrequency() const
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_HIGHCUTFILTERFREQUENCY));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuMixer::setHighCutFilterFrequency(int value)
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_HIGHCUTFILTERFREQUENCY));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuMixer::getOutputLevel() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_OUTPUTLEVEL));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuMixer::setOutputLevel(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_OUTPUTLEVEL));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuMixer::getOutputPanning() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_OUTPUTPANNING));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuMixer::setOutputPanning(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_OUTPUTPANNING));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuMixer::getOutputFrontRear() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_OUTPUTFRONTREAR));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuMixer::setOutputFrontRear(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_OUTPUTFRONTREAR));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuMixer::getSendToReverb() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_SENDTOREVERB));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuMixer::setSendToReverb(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_SENDTOREVERB));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuMixer::getSendToChorus() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_SENDTOCHORUS));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuMixer::setSendToChorus(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_SENDTOCHORUS));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}
