#include "duadsr.h"

#include <cstring>

#include <QJsonObject>

DU_OBJECT_IMPL(DuAdsr)

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

DuObjectPtr DuAdsr::clone() const
{
    return DuAdsrPtr(new DuAdsr(*this));
}


DuAdsrPtr DuAdsr::fromDuMusicBinary(const FX_adsr &du_adsr)
{
    DuAdsrPtr adsr(new DuAdsr);
    bool verif = true;

    verif = adsr->setEnvelopeAttackTime(du_adsr.a_env_attack_time) ? verif : false;
    verif = adsr->setEnvelopeDecayTime(du_adsr.a_env_decay_time) ? verif : false;
    verif = adsr->setTimeVariantFilterCutoffResonance(du_adsr.a_tvf_cutoff_res) ? verif : false;
    verif = adsr->setTimeVariantFilterCutoffFrequency(du_adsr.a_tvf_cutoff_freq) ? verif : false;
    verif = adsr->setEnvelopeReleaseTime(du_adsr.a_env_release_time) ? verif : false;

    verif = adsr->setEffectName( QString(QByteArray((char *)du_adsr.a_name, NAME_CARACT))) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuAdsr::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    return adsr;
}


DuAdsrPtr DuAdsr::fromJson(const QJsonObject &jsonAdsr)
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
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuAdsr::fromJson():\n"
                    << "failed to generate DuAdsr\n"
                    << "a json key did not contain the proper type";

        return DuAdsrPtr();
    }


    DuAdsrPtr adsr(new DuAdsr);
    bool verif = true;

    verif = adsr->setEnvelopeAttackTime(jsonEnvAttTime.toInt()) ? verif : false;
    verif = adsr->setEnvelopeDecayTime(jsonEnvDecTime.toInt()) ? verif : false;
    verif = adsr->setTimeVariantFilterCutoffResonance(jsonTvfCutRes.toInt()) ? verif : false;
    verif = adsr->setTimeVariantFilterCutoffFrequency(jsonTvfCutFreq.toInt()) ? verif : false;
    verif = adsr->setEnvelopeReleaseTime(jsonEnvRelTime.toInt()) ? verif : false;

    verif = adsr->setEffectName(jsonEffectName.toString()) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuAdsr::fromJson():\n"
                   << "an attribute was not properly set";
    }

    return adsr;
}


QByteArray DuAdsr::toDuMusicBinary() const
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

    std::memcpy(du_adsr.a_name, tmpName.data(), NAME_CARACT);


    return QByteArray((char *)&(du_adsr), size());
}


int DuAdsr::size() const
{
    return FX_ADSR_SIZE;
}


int DuAdsr::getEnvelopeAttackTime() const
{
    const DuNumericConstPtr &tmp =
            getChildAs<DuNumeric>(KEY_ADSR_ENVELOPEATTACKTIME);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuAdsr::setEnvelopeAttackTime(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_ADSR_ENVELOPEATTACKTIME);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuAdsr::getEnvelopeDecayTime() const
{
    const DuNumericConstPtr &tmp =
            getChildAs<DuNumeric>(KEY_ADSR_ENVELOPEDECAYTIME);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuAdsr::setEnvelopeDecayTime(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_ADSR_ENVELOPEDECAYTIME);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuAdsr::getTimeVariantFilterCutoffResonance() const
{
    const DuNumericConstPtr &tmp =
            getChildAs<DuNumeric>(KEY_ADSR_TIMEVARIANTFILTERCUTOFFRESONANCE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuAdsr::setTimeVariantFilterCutoffResonance(int value)
{
    const DuNumericPtr &tmp =
            getChildAs<DuNumeric>(KEY_ADSR_TIMEVARIANTFILTERCUTOFFRESONANCE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuAdsr::getTimeVariantFilterCutoffFrequency() const
{
    const DuNumericConstPtr &tmp =
            getChildAs<DuNumeric>(KEY_ADSR_TIMEVARIANTFILTERCUTOFFFREQUENCY);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuAdsr::setTimeVariantFilterCutoffFrequency(int value)
{
    const DuNumericPtr &tmp =
            getChildAs<DuNumeric>(KEY_ADSR_TIMEVARIANTFILTERCUTOFFFREQUENCY);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuAdsr::getEnvelopeReleaseTime() const
{
    const DuNumericConstPtr &tmp =
            getChildAs<DuNumeric>(KEY_ADSR_ENVELOPERELEASETIME);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuAdsr::setEnvelopeReleaseTime(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_ADSR_ENVELOPERELEASETIME);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

QString DuAdsr::getEffectName() const
{
    const DuStringConstPtr &tmp = getChildAs<DuString>(KEY_ADSR_EFFECTNAME);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuAdsr::setEffectName(const QString &value)
{
    const DuStringPtr &tmp = getChildAs<DuString>(KEY_ADSR_EFFECTNAME);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}
