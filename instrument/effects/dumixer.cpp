#include "dumixer.h"

#include <QJsonObject>

#include "../../general/dunumeric.h"

DU_OBJECT_IMPL(DuMixer)

DuMixer::DuMixer() :
    DuEffectSettings()
{
    addChild(KeyInputGain,
             new DuNumeric(FX_MIX_INGAIN_MAXVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_MIX_INGAIN_MAXVALUE, FX_MIX_INGAIN_MINVALUE));

    addChild(KeyLowCutFilterFrequency,
             new DuNumeric(FX_MIX_LCFREQ_MINVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_MIX_LCFREQ_MAXVALUE, FX_MIX_LCFREQ_MINVALUE));

    addChild(KeyHighCutFilterFrequency,
             new DuNumeric(FX_MIX_HCFREQ_MAXVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_MIX_HCFREQ_MAXVALUE, FX_MIX_HCFREQ_MINVALUE));

    addChild(KeyOutputLevel,
             new DuNumeric(FX_MIX_OUTLEVEL_MAXVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_MIX_OUTLEVEL_MAXVALUE, FX_MIX_OUTLEVEL_MINVALUE));

    addChild(KeyOutputPanning,
             new DuNumeric(FX_MIX_OUTPAN_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_MIX_OUTPAN_MAXVALUE, FX_MIX_OUTPAN_MINVALUE));

    addChild(KeyOutputFrontRear,
             new DuNumeric(FX_MIX_OUTFR_MINVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_MIX_OUTFR_MAXVALUE, FX_MIX_OUTFR_MINVALUE));

    addChild(KeySendToReverb,
             new DuNumeric(FX_MIX_REVERB_MINVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_MIX_REVERB_MAXVALUE, FX_MIX_REVERB_MINVALUE));

    addChild(KeySendToChorus,
             new DuNumeric(FX_MIX_CHORUS_MINVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_MIX_CHORUS_MAXVALUE, FX_MIX_CHORUS_MINVALUE));
}

DuMixer::~DuMixer()
{
}

DuObjectPtr DuMixer::clone() const
{
    return DuMixerPtr(new DuMixer(*this));
}


DuMixerPtr DuMixer::fromDuMusicBinary(const FX_mix &du_mixer)
{
    DuMixerPtr mixer(new DuMixer);
    bool verif = true;

    verif = mixer->setInputGain(du_mixer.m_inputgain) ? verif : false;
    verif = mixer->setLowCutFilterFrequency(du_mixer.m_locutfilterfrequency) ? verif : false;
    verif = mixer->setHighCutFilterFrequency(du_mixer.m_hicutfilterfrequency) ? verif : false;

    verif = mixer->setOutputLevel(du_mixer.m_ouputlevel) ? verif : false;
    verif = mixer->setOutputPanning(du_mixer.m_outputpanning) ? verif : false;
    verif = mixer->setOutputFrontRear(du_mixer.m_ouputfrontrear) ? verif : false;

    verif = mixer->setSendToReverb(du_mixer.m_sendtoreverb) ? verif : false;
    verif = mixer->setSendToChorus(du_mixer.m_sendtochorus) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuMixer::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    return mixer;
}

DuMixerPtr DuMixer::fromJson(const QJsonObject &jsonMixer)
{
    QJsonValue jsonInputGain    = jsonMixer[KeyInputGain];
    QJsonValue jsonLoCutFreq    = jsonMixer[KeyLowCutFilterFrequency];
    QJsonValue jsonHiCutFreq    = jsonMixer[KeyHighCutFilterFrequency];
    QJsonValue jsonOutputLvl    = jsonMixer[KeyOutputLevel];
    QJsonValue jsonPanning      = jsonMixer[KeyOutputPanning];
    QJsonValue jsonFrontRear    = jsonMixer[KeyOutputFrontRear];
    QJsonValue jsonToReverb     = jsonMixer[KeySendToReverb];
    QJsonValue jsonToChorus     = jsonMixer[KeySendToChorus];

    if (        !jsonInputGain.isDouble()   ||  !jsonLoCutFreq.isDouble()
            ||  !jsonHiCutFreq.isDouble()   ||  !jsonOutputLvl.isDouble()
            ||  !jsonPanning.isDouble()     ||  !jsonFrontRear.isDouble()
            ||  !jsonToReverb.isDouble()    ||  !jsonToChorus.isDouble())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMixer::fromJson():\n"
                    << "failed to generate DuMixer\n"
                    << "a json key did not contain the proper type";

        return DuMixerPtr();
    }


    DuMixerPtr mixer(new DuMixer);
    bool verif = true;

    verif = mixer->setInputGain(jsonInputGain.toInt()) ? verif : false;
    verif = mixer->setLowCutFilterFrequency(jsonLoCutFreq.toInt()) ? verif : false;
    verif = mixer->setHighCutFilterFrequency(jsonHiCutFreq.toInt()) ? verif : false;

    verif = mixer->setOutputLevel(jsonOutputLvl.toInt()) ? verif : false;
    verif = mixer->setOutputPanning(jsonPanning.toInt()) ? verif : false;
    verif = mixer->setOutputFrontRear(jsonFrontRear.toInt()) ? verif : false;

    verif = mixer->setSendToReverb(jsonToReverb.toInt()) ? verif : false;
    verif = mixer->setSendToChorus(jsonToChorus.toInt()) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuMixer::fromJson():\n"
                   << "an attribute was not properly set";
    }

    return mixer;
}


QByteArray DuMixer::toDuMusicBinary() const
{
    FX_mix du_mixer;
    std::memset((char*)&du_mixer, 0, size());
    int tmp = 0;


    tmp = getInputGain();
    if (tmp == -1)
        return QByteArray();
    du_mixer.m_inputgain = tmp;

    tmp = getLowCutFilterFrequency();
    if (tmp == -1)
        return QByteArray();
    du_mixer.m_locutfilterfrequency = tmp;

    tmp = getHighCutFilterFrequency();
    if (tmp == -1)
        return QByteArray();
    du_mixer.m_hicutfilterfrequency = tmp;

    tmp = getOutputLevel();
    if (tmp == -1)
        return QByteArray();
    du_mixer.m_ouputlevel = tmp;

    tmp = getOutputPanning();
    if (tmp == -1)
        return QByteArray();
    du_mixer.m_outputpanning = tmp;

    tmp = getOutputFrontRear();
    if (tmp == -1)
        return QByteArray();
    du_mixer.m_ouputfrontrear = tmp;

    tmp = getSendToReverb();
    if (tmp == -1)
        return QByteArray();
    du_mixer.m_sendtoreverb = tmp;

    tmp = getSendToChorus();
    if (tmp == -1)
        return QByteArray();
    du_mixer.m_sendtochorus = tmp;


    return QByteArray((char *)&(du_mixer), size());
}


int DuMixer::size() const
{
    return FX_MIX_SIZE;
}


DU_KEY_ACCESSORS_IMPL(DuMixer, InputGain,              Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuMixer, LowCutFilterFrequency,  Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuMixer, HighCutFilterFrequency, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuMixer, OutputLevel,            Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuMixer, OutputPanning,          Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuMixer, OutputFrontRear,        Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuMixer, SendToReverb,           Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuMixer, SendToChorus,           Numeric, int, -1)
