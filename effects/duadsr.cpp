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
    bool verif = true;

    verif = verif && adsr->setEnvelopeAttackTime(du_adsr.a_env_attack_time);
    verif = verif && adsr->setEnvelopeDecayTime(du_adsr.a_env_decay_time);
    verif = verif && adsr->setTimeVariantFilterCutoffResonance(
                du_adsr.a_tvf_cutoff_res);
    verif = verif && adsr->setTimeVariantFilterCutoffFrequency(
                du_adsr.a_tvf_cutoff_freq);
    verif = verif && adsr->setEnvelopeReleaseTime(du_adsr.a_env_release_time);

    verif = verif && adsr->setEffectName(
                QString(QByteArray((char *)du_adsr.a_name, NAME_CARACT)));

    if (!verif)
    {
        delete adsr;
        return NULL;
    }

    return adsr;
}


DuAdsr *DuAdsr::fromJson(const QJsonObject &jsonAdsr)
{
    QJsonValue jsonEnvAttTime   = jsonAdsr[KEY_ADSR_ENVELOPEATTACKTIME];
    QJsonValue jsonEnvDecTime   = jsonAdsr[KEY_ADSR_ENVELOPEDECAYTIME];
    QJsonValue jsonTvfCutRes    = jsonAdsr[KEY_ADSR_TIMEVARIANTFILTERCUTOFFRESONANCE];
    QJsonValue jsonTvfCutFreq   = jsonAdsr[KEY_ADSR_TIMEVARIANTFILTERCUTOFFFREQUENCY];
    QJsonValue jsonEnvRelTime   = jsonAdsr[KEY_ADSR_ENVELOPERELEASETIME];
    QJsonValue jsonEffectName   = jsonAdsr[KEY_ADSR_EFFECTNAME];

    if (        !jsonEnvAttTime.isDouble()  ||  !jsonEnvDecTime.isDouble()
            ||  !jsonTvfCutRes.isDouble()   ||  !jsonTvfCutFreq.isDouble()
            ||  !jsonEnvRelTime.isDouble()  ||  !jsonEffectName.isString())

        return NULL;


    DuAdsr *adsr = new DuAdsr;
    bool verif = true;

    verif = verif && adsr->setEnvelopeAttackTime(jsonEnvAttTime.toInt());
    verif = verif && adsr->setEnvelopeDecayTime(jsonEnvDecTime.toInt());
    verif = verif && adsr->setTimeVariantFilterCutoffResonance(jsonTvfCutRes.toInt());
    verif = verif && adsr->setTimeVariantFilterCutoffFrequency(jsonTvfCutFreq.toInt());
    verif = verif && adsr->setEnvelopeReleaseTime(jsonEnvRelTime.toInt());

    verif = verif && adsr->setEffectName(jsonEffectName.toString());

    if (!verif)
    {
        delete adsr;
        return NULL;
    }

    return adsr;
}


QByteArray DuAdsr::toDuMusicFile() const
{
    FX_adsr du_adsr;

    QString tmpStr;
    int tmpNum = 0;


    tmpNum = getEnvelopeAttackTime();
    if (tmpNum == -1)
        return QByteArray();
    du_adsr.a_env_attack_time = tmpNum;

    tmpNum = getEnvelopeDecayTime();
    if (tmpNum == -1)
        return QByteArray();
    du_adsr.a_env_decay_time = tmpNum;

    tmpNum = getTimeVariantFilterCutoffResonance();
    if (tmpNum == -1)
        return QByteArray();
    du_adsr.a_tvf_cutoff_res = tmpNum;

    tmpNum = getTimeVariantFilterCutoffFrequency();
    if (tmpNum == -1)
        return QByteArray();
    du_adsr.a_tvf_cutoff_freq = tmpNum;

    tmpNum = getEnvelopeReleaseTime();
    if (tmpNum == -1)
        return QByteArray();
    du_adsr.a_env_release_time = tmpNum;


    QByteArray tmpName(NAME_CARACT, (char)0x00);
    tmpStr = getEffectName();
    if (tmpStr.isNull())
        return QByteArray();
    tmpName.prepend(tmpStr.toUtf8());

#ifdef Q_OS_WIN
    memcpy_s(du_adsr.a_name, NAME_CARACT, tmpName.data(), NAME_CARACT);
#else
    memcpy(du_adsr.a_name, tmpName.data(), NAME_CARACT);
#endif


    return QByteArray((char *)&(du_adsr), size());
}


int DuAdsr::size() const
{
    return FX_ADSR_SIZE;
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
        return QString();

    return tmp->getString();
}

bool DuAdsr::setEffectName(const QString &value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_ADSR_EFFECTNAME));

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}
