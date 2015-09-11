#include "ducompressor.h"

#include <cstring>

#include <QJsonObject>

DU_OBJECT_IMPL(DuCompressor)

DuCompressor::DuCompressor() :
    DuEffectSettings()
{
    addChild(KeyOnOff,
             new DuNumeric(FX_COMP_ONOFF_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_COMP_ONOFF_MAXVALUE, FX_COMP_ONOFF_MINVALUE));

    addChild(KeyAttackTime,
             new DuNumeric(FX_COMP_ATTACK_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_COMP_ATTACK_MAXVALUE, FX_COMP_ATTACK_MINVALUE));

    addChild(KeyReleaseTime,
             new DuNumeric(FX_COMP_RELEASE_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_COMP_RELEASE_MAXVALUE, FX_COMP_RELEASE_MINVALUE));

    addChild(KeyThreshold,
             new DuNumeric(FX_COMP_THRESHOLD_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_COMP_THRESHOLD_MAXVALUE, FX_COMP_THRESHOLD_MINVALUE));

    addChild(KeyRatio,
             new DuNumeric(FX_COMP_RATIO_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_COMP_RATIO_MAXVALUE, FX_COMP_RATIO_MINVALUE));

    addChild(KeyBoost,
             new DuNumeric(FX_COMP_BOOST_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_COMP_BOOST_MAXVALUE, FX_COMP_BOOST_MINVALUE));

    addChild(KeyKneeType,
             new DuNumeric(FX_COMP_KNEE_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_COMP_KNEE_MAXVALUE, FX_COMP_KNEE_MINVALUE));

    addChild(KeyEffectName,
             new DuString(QStringLiteral(DEFAULT_EFFECTNAME), NAME_CARACT));
}

DuCompressor::~DuCompressor()
{
}

DuObjectPtr DuCompressor::clone() const
{
    return DuCompressorPtr(new DuCompressor(*this));
}


DuCompressorPtr DuCompressor::fromDuMusicBinary(const FX_compressor &du_compressor)
{
    DuCompressorPtr compressor(new DuCompressor);
    bool verif = true;

    verif = compressor->setOnOff(du_compressor.c_on_off) ? verif : false;

    verif = compressor->setAttackTime(du_compressor.c_attacktime) ? verif : false;
    verif = compressor->setReleaseTime(du_compressor.c_releasetime) ? verif : false;

    verif = compressor->setThreshold(du_compressor.c_threshold) ? verif : false;
    verif = compressor->setRatio(du_compressor.c_ratio) ? verif : false;
    verif = compressor->setBoost(du_compressor.c_boost) ? verif : false;
    verif = compressor->setKneeType(du_compressor.c_kneetype) ? verif : false;

    verif = compressor->setEffectName(QString(QByteArray((char *)du_compressor.c_name, NAME_CARACT))) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuCompressor::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    return compressor;
}


DuCompressorPtr DuCompressor::fromJson(const QJsonObject &jsonCompressor)
{
    QJsonValue jsonOnOff        = jsonCompressor[KeyOnOff];
    QJsonValue jsonAttTime      = jsonCompressor[KeyAttackTime];
    QJsonValue jsonRelTime      = jsonCompressor[KeyReleaseTime];
    QJsonValue jsonThreshold    = jsonCompressor[KeyThreshold];
    QJsonValue jsonRatio        = jsonCompressor[KeyRatio];
    QJsonValue jsonBoost        = jsonCompressor[KeyBoost];
    QJsonValue jsonKneeType     = jsonCompressor[KeyKneeType];
    QJsonValue jsonEffectName   = jsonCompressor[KeyEffectName];

    if (        !jsonOnOff.isDouble()       ||  !jsonAttTime.isDouble()
            ||  !jsonRelTime.isDouble()     ||  !jsonThreshold.isDouble()
            ||  !jsonRatio.isDouble()       ||  !jsonBoost.isDouble()
            ||  !jsonKneeType.isDouble()    ||  !jsonEffectName.isString())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuCompressor::fromJson():\n"
                    << "failed to generate DuCompressor\n"
                    << "a json key did not contain the proper type";

        return DuCompressorPtr();
    }


    DuCompressorPtr compressor(new DuCompressor);
    bool verif = true;

    verif = compressor->setOnOff(jsonOnOff.toInt()) ? verif : false;

    verif = compressor->setAttackTime(jsonAttTime.toInt()) ? verif : false;
    verif = compressor->setReleaseTime(jsonRelTime.toInt()) ? verif : false;
    verif = compressor->setThreshold(jsonThreshold.toInt()) ? verif : false;

    verif = compressor->setRatio(jsonRatio.toInt()) ? verif : false;
    verif = compressor->setBoost(jsonBoost.toInt()) ? verif : false;
    verif = compressor->setKneeType(jsonKneeType.toInt()) ? verif : false;

    verif = compressor->setEffectName(jsonEffectName.toString()) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuCompressor::fromJson():\n"
                   << "an attribute was not properly set";
    }

    return compressor;
}

QByteArray DuCompressor::toDuMusicBinary() const
{
    FX_compressor du_compressor;

    QString tmpStr;
    int tmpNum = 0;


    tmpNum = getOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_compressor.c_on_off = tmpNum;

    tmpNum = getAttackTime();
    if (tmpNum == -1)
        return QByteArray();
    du_compressor.c_attacktime = tmpNum;

    tmpNum = getReleaseTime();
    if (tmpNum == -1)
        return QByteArray();
    du_compressor.c_releasetime = tmpNum;

    tmpNum = getThreshold();
    if (tmpNum == -1)
        return QByteArray();
    du_compressor.c_threshold = tmpNum;

    tmpNum = getRatio();
    if (tmpNum == -1)
        return QByteArray();
    du_compressor.c_ratio = tmpNum;

    tmpNum = getBoost();
    if (tmpNum == -1)
        return QByteArray();
    du_compressor.c_boost = tmpNum;

    tmpNum = getKneeType();
    if (tmpNum == -1)
        return QByteArray();
    du_compressor.c_kneetype = tmpNum;


    QByteArray tmpName(NAME_CARACT, (char)0x00);
    tmpStr = getEffectName();
    if (tmpStr.isNull())
        return QByteArray();
    tmpName.prepend(tmpStr.toUtf8());

    std::memcpy(du_compressor.c_name, tmpName.data(), NAME_CARACT);


    return QByteArray((char *)&(du_compressor), size());
}


int DuCompressor::size() const
{
    return FX_COMP_SIZE;
}


DU_KEY_ACCESSORS_IMPL(DuCompressor, OnOff,       Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuCompressor, AttackTime,  Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuCompressor, ReleaseTime, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuCompressor, Threshold,   Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuCompressor, Ratio,       Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuCompressor, Boost,       Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuCompressor, KneeType,    Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuCompressor, EffectName,  String, QString, QString())
