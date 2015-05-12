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
    DuAdsr *adsr = new DuAdsr();

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
    DuAdsr *adsr = new DuAdsr();
    QStringList &keyList = adsr->keys();

    bool test = true;
    for (int i = 0; i < keyList.count(); i++)
    {
        test = test && jsonAdsr.contains(keyList[i]);
    }

    if (!test)
        return NULL;

    adsr->setEnvelopeAttackTime(jsonAdsr[KEY_ADSR_ENVELOPEATTACKTIME].toInt());
    adsr->setEnvelopeDecayTime(jsonAdsr[KEY_ADSR_ENVELOPEDECAYTIME].toInt());
    adsr->setTimeVariantFilterCutoffResonance(
                jsonAdsr[KEY_ADSR_TIMEVARIANTFILTERCUTOFFRESONANCE].toInt());
    adsr->setTimeVariantFilterCutoffFrequency(
                jsonAdsr[KEY_ADSR_TIMEVARIANTFILTERCUTOFFFREQUENCY].toInt());
    adsr->setEnvelopeReleaseTime(jsonAdsr[KEY_ADSR_ENVELOPERELEASETIME].toInt());

    adsr->setEffectName(jsonAdsr[KEY_ADSR_EFFECTNAME].toString());

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

    QByteArray array((char *)&(du_adsr), FX_ADSR_SIZE);

    return array;
}


int DuAdsr::getEnvelopeAttackTime() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_ADSR_ENVELOPEATTACKTIME));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuAdsr::setEnvelopeAttackTime(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_ADSR_ENVELOPEATTACKTIME));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuAdsr::getEnvelopeDecayTime() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_ADSR_ENVELOPEDECAYTIME));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuAdsr::setEnvelopeDecayTime(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_ADSR_ENVELOPEDECAYTIME));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuAdsr::getTimeVariantFilterCutoffResonance() const
{
    DuNumeric *tmp =
        dynamic_cast<DuNumeric *>(getChild(KEY_ADSR_TIMEVARIANTFILTERCUTOFFRESONANCE));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuAdsr::setTimeVariantFilterCutoffResonance(int value)
{
    DuNumeric *tmp =
        dynamic_cast<DuNumeric *>(getChild(KEY_ADSR_TIMEVARIANTFILTERCUTOFFRESONANCE));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuAdsr::getTimeVariantFilterCutoffFrequency() const
{
    DuNumeric *tmp =
        dynamic_cast<DuNumeric *>(getChild(KEY_ADSR_TIMEVARIANTFILTERCUTOFFFREQUENCY));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuAdsr::setTimeVariantFilterCutoffFrequency(int value)
{
    DuNumeric *tmp =
        dynamic_cast<DuNumeric *>(getChild(KEY_ADSR_TIMEVARIANTFILTERCUTOFFFREQUENCY));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuAdsr::getEnvelopeReleaseTime() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_ADSR_ENVELOPERELEASETIME));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuAdsr::setEnvelopeReleaseTime(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_ADSR_ENVELOPERELEASETIME));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

QString DuAdsr::getEffectName() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_ADSR_EFFECTNAME));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

void DuAdsr::setEffectName(QString &value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_ADSR_EFFECTNAME));

    if (tmp == NULL)
        return;

    tmp->setString(value);
}
