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
    DuMixer *mixer = new DuMixer();

    mixer->setInputGain(du_mixer.m_inputgain);
    mixer->setLowCutFilterFrequency(du_mixer.m_locutfilterfrequency);
    mixer->setHighCutFilterFrequency(du_mixer.m_hicutfilterfrequency);

    mixer->setOutputLevel(du_mixer.m_ouputlevel);
    mixer->setOutputPanning(du_mixer.m_outputpanning);
    mixer->setOutputFrontRear(du_mixer.m_ouputfrontrear);

    mixer->setSendToReverb(du_mixer.m_sendtoreverb);
    mixer->setSendToChorus(du_mixer.m_sendtochorus);

    return mixer;
}

DuMixer *DuMixer::fromJson(const QJsonObject &jsonMixer)
{
    DuMixer *mixer = new DuMixer();
    const QStringList &keyList = mixer->keys();

    bool test = true;
    for (int i = 0; i < keyList.count(); i++)
    {
        test = test && jsonMixer.contains(keyList[i]);
    }

    if (!test)
        return NULL;

    mixer->setInputGain(jsonMixer[KEY_MIXER_INPUTGAIN].toInt());
    mixer->setLowCutFilterFrequency(
                jsonMixer[KEY_MIXER_LOWCUTFILTERFREQUENCY].toInt());
    mixer->setHighCutFilterFrequency(
                jsonMixer[KEY_MIXER_HIGHCUTFILTERFREQUENCY].toInt());

    mixer->setOutputLevel(jsonMixer[KEY_MIXER_OUTPUTLEVEL].toInt());
    mixer->setOutputPanning(jsonMixer[KEY_MIXER_OUTPUTPANNING].toInt());
    mixer->setOutputFrontRear(jsonMixer[KEY_MIXER_OUTPUTFRONTREAR].toInt());

    mixer->setSendToReverb(jsonMixer[KEY_MIXER_SENDTOREVERB].toInt());
    mixer->setSendToChorus(jsonMixer[KEY_MIXER_SENDTOCHORUS].toInt());

    return mixer;
}


int DuMixer::getInputGain() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_INPUTGAIN));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuMixer::setInputGain(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_INPUTGAIN));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuMixer::getLowCutFilterFrequency() const
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_LOWCUTFILTERFREQUENCY));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuMixer::setLowCutFilterFrequency(int value)
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_LOWCUTFILTERFREQUENCY));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuMixer::getHighCutFilterFrequency() const
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_HIGHCUTFILTERFREQUENCY));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuMixer::setHighCutFilterFrequency(int value)
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_HIGHCUTFILTERFREQUENCY));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


int DuMixer::getOutputLevel() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_OUTPUTLEVEL));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuMixer::setOutputLevel(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_OUTPUTLEVEL));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuMixer::getOutputPanning() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_OUTPUTPANNING));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuMixer::setOutputPanning(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_OUTPUTPANNING));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuMixer::getOutputFrontRear() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_OUTPUTFRONTREAR));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuMixer::setOutputFrontRear(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_OUTPUTFRONTREAR));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


int DuMixer::getSendToReverb() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_SENDTOREVERB));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuMixer::setSendToReverb(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_SENDTOREVERB));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuMixer::getSendToChorus() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_SENDTOCHORUS));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuMixer::setSendToChorus(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_MIXER_SENDTOCHORUS));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}
