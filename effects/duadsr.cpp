#include "duadsr.h"

DuAdsr::DuAdsr() :
    DuEffectSettings()
{
    addChild(KEY_ADSR_ENVELOPEATTACKTIME,
             new DuNumeric(FX_ADSR_ATTACK_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_ADSR_ATTACK_MAXVALUE, FX_ADSR_ATTACK_MINVALUE));

    addChild(KEY_ADSR_ENVELOPEDECAYTIME,
             new DuNumeric(FX_ADSR_DECAY_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_ADSR_DECAY_MAXVALUE, FX_ADSR_DECAY_MINVALUE));

    addChild(KEY_ADSR_TIMEVARIANTFILTERCUTOFFRESONANCE,
             new DuNumeric(FX_ADSR_RES_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_ADSR_RES_MAXVALUE, FX_ADSR_RES_MINVALUE));

    addChild(KEY_ADSR_TIMEVARIANTFILTERCUTOFFFREQUENCY,
             new DuNumeric(FX_ADSR_FREQ_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_ADSR_FREQ_MAXVALUE, FX_ADSR_FREQ_MINVALUE));

    addChild(KEY_ADSR_ENVELOPERELEASETIME,
             new DuNumeric(FX_ADSR_RELEAS_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_ADSR_RELEAS_MAXVALUE, FX_ADSR_RELEAS_MINVALUE));

    addChild(KEY_ADSR_EFFECTNAME,
             new DuString(QString(DEFAULT_EFFECTNAME), NAME_CARACT));
}

DuAdsr::~DuAdsr()
{
}


DuAdsr *DuAdsr::fromDuMusicFile(const FX_adsr &du_adsr)
{
    DuAdsr *adsr = new DuAdsr;

    adsr->setEnvelopeAttackTime(du_adsr.a_env_attack_time);
    adsr->setEnvelopeDecayTime(du_adsr.a_env_decay_time);
    adsr->setTimeVariantFilterCutoffResonance(du_adsr.a_tvf_cutoff_res);
    adsr->setTimeVariantFilterCutoffFrequency(du_adsr.a_tvf_cutoff_freq);
    adsr->setEnvelopeReleaseTime(du_adsr.a_env_release_time);

    adsr->setEffectName(QString(QByteArray((char *)du_adsr.a_name, NAME_CARACT)));

    return adsr;
}


DuAdsr *DuAdsr::fromJson(const QJsonObject &jsonAdsr)
{
    QJsonValue jsonEnvAttTime = jsonAdsr[KEY_ADSR_ENVELOPEATTACKTIME];
    QJsonValue jsonEnvDecTime = jsonAdsr[KEY_ADSR_ENVELOPEDECAYTIME];
    QJsonValue jsonTvfCutRes  = jsonAdsr[KEY_ADSR_TIMEVARIANTFILTERCUTOFFRESONANCE];
    QJsonValue jsonTvfCutFreq = jsonAdsr[KEY_ADSR_TIMEVARIANTFILTERCUTOFFFREQUENCY];
    QJsonValue jsonEnvRelTime = jsonAdsr[KEY_ADSR_ENVELOPERELEASETIME];
    QJsonValue jsonEffectName = jsonAdsr[KEY_ADSR_EFFECTNAME];

    if (!jsonEnvAttTime.isDouble() || !jsonEnvDecTime.isDouble()
            || !jsonTvfCutRes.isDouble() || !jsonTvfCutFreq.isDouble()
            || !jsonEnvRelTime.isDouble() || !jsonEffectName.isString())

        return NULL;


    DuAdsr *adsr = new DuAdsr;

    adsr->setEnvelopeAttackTime(jsonEnvAttTime.toInt());
    adsr->setEnvelopeDecayTime(jsonEnvDecTime.toInt());
    adsr->setTimeVariantFilterCutoffResonance(jsonTvfCutRes.toInt());
    adsr->setTimeVariantFilterCutoffFrequency(jsonTvfCutFreq.toInt());
    adsr->setEnvelopeReleaseTime(jsonEnvRelTime.toInt());

    adsr->setEffectName(jsonEffectName.toString());

    return adsr;
}


QByteArray DuAdsr::toDuMusicFile()
{
    FX_adsr du_adsr;
    du_adsr.a_env_attack_time = getEnvelopeAttackTime();
    du_adsr.a_env_decay_time = getEnvelopeDecayTime();
    du_adsr.a_tvf_cutoff_res = getTimeVariantFilterCutoffResonance();
    du_adsr.a_tvf_cutoff_freq = getTimeVariantFilterCutoffFrequency();
    du_adsr.a_env_release_time = getEnvelopeReleaseTime();

    QByteArray tmpName(NAME_CARACT, (char)0x00);
    tmpName.prepend(getEffectName().toUtf8());

#ifdef Q_OS_WIN
    memcpy_s(du_adsr.a_name, NAME_CARACT, tmpName.data(), NAME_CARACT);
#else
    memcpy(du_adsr.a_name, tmpName.data(), NAME_CARACT);
#endif

    return QByteArray((char *)&(du_adsr), FX_ADSR_SIZE);
}


int DuAdsr::getEnvelopeAttackTime() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_ADSR_ENVELOPEATTACKTIME));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuAdsr::setEnvelopeAttackTime(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_ADSR_ENVELOPEATTACKTIME));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuAdsr::getEnvelopeDecayTime() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_ADSR_ENVELOPEDECAYTIME));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuAdsr::setEnvelopeDecayTime(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_ADSR_ENVELOPEDECAYTIME));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuAdsr::getTimeVariantFilterCutoffResonance() const
{
    DuNumeric *tmp =
        dynamic_cast<DuNumeric *>(getChild(KEY_ADSR_TIMEVARIANTFILTERCUTOFFRESONANCE));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuAdsr::setTimeVariantFilterCutoffResonance(int value)
{
    DuNumeric *tmp =
        dynamic_cast<DuNumeric *>(getChild(KEY_ADSR_TIMEVARIANTFILTERCUTOFFRESONANCE));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuAdsr::getTimeVariantFilterCutoffFrequency() const
{
    DuNumeric *tmp =
        dynamic_cast<DuNumeric *>(getChild(KEY_ADSR_TIMEVARIANTFILTERCUTOFFFREQUENCY));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuAdsr::setTimeVariantFilterCutoffFrequency(int value)
{
    DuNumeric *tmp =
        dynamic_cast<DuNumeric *>(getChild(KEY_ADSR_TIMEVARIANTFILTERCUTOFFFREQUENCY));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuAdsr::getEnvelopeReleaseTime() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_ADSR_ENVELOPERELEASETIME));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuAdsr::setEnvelopeReleaseTime(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_ADSR_ENVELOPERELEASETIME));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

QString DuAdsr::getEffectName() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_ADSR_EFFECTNAME));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

bool DuAdsr::setEffectName(const QString &value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_ADSR_EFFECTNAME));

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}
