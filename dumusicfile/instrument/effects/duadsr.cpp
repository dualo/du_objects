#include "duadsr.h"

#include <cstring>

#include <QJsonObject>

#include "../../../general/dunumeric.h"
#include "../../../general/dustring.h"

DU_OBJECT_IMPL(DuAdsr)

DuAdsr::DuAdsr() :
    DuEffectSettings()
{
    addChild(KeyEnvelopeAttackTime,
             new DuNumeric(FX_ADSR_ATTACK_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_ADSR_ATTACK_MAXVALUE, FX_ADSR_ATTACK_MINVALUE));

    addChild(KeyEnvelopeDecayTime,
             new DuNumeric(FX_ADSR_DECAY_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_ADSR_DECAY_MAXVALUE, FX_ADSR_DECAY_MINVALUE));

    addChild(KeyTimeVariantFilterCutoffResonance,
             new DuNumeric(FX_ADSR_RES_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_ADSR_RES_MAXVALUE, FX_ADSR_RES_MINVALUE));

    addChild(KeyTimeVariantFilterCutoffFrequency,
             new DuNumeric(FX_ADSR_FREQ_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_ADSR_FREQ_MAXVALUE, FX_ADSR_FREQ_MINVALUE));

    addChild(KeyEnvelopeReleaseTime,
             new DuNumeric(FX_ADSR_RELEAS_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_ADSR_RELEAS_MAXVALUE, FX_ADSR_RELEAS_MINVALUE));

    addChild(KeyEffectName,
             new DuString(QStringLiteral(DEFAULT_EFFECTNAME), NAME_CARACT));
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
    QJsonValue jsonEnvAttTime   = jsonAdsr[KeyEnvelopeAttackTime];
    QJsonValue jsonEnvDecTime   = jsonAdsr[KeyEnvelopeDecayTime];
    QJsonValue jsonTvfCutRes    = jsonAdsr[KeyTimeVariantFilterCutoffResonance];
    QJsonValue jsonTvfCutFreq   = jsonAdsr[KeyTimeVariantFilterCutoffFrequency];
    QJsonValue jsonEnvRelTime   = jsonAdsr[KeyEnvelopeReleaseTime];
    QJsonValue jsonEffectName   = jsonAdsr[KeyEffectName];

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


DU_KEY_ACCESSORS_IMPL(DuAdsr, EnvelopeAttackTime,               Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuAdsr, EnvelopeDecayTime,                Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuAdsr, TimeVariantFilterCutoffResonance, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuAdsr, TimeVariantFilterCutoffFrequency, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuAdsr, EnvelopeReleaseTime,              Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuAdsr, EffectName,                       String, QString, QString())
