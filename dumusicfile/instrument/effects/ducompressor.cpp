#include "ducompressor.h"

#include <cstring>

#include <QJsonObject>

#include "../../../general/dunumeric.h"
#include "../../../general/dustring.h"

DU_OBJECT_IMPL(DuCompressor)

DuCompressor::DuCompressor() :
    DuEffectSettings()
{
    addChild(KEY_COMP_ONOFF,
             new DuNumeric(FX_COMP_ONOFF_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_COMP_ONOFF_MAXVALUE, FX_COMP_ONOFF_MINVALUE));

    addChild(KEY_COMP_ATTACKTIME,
             new DuNumeric(FX_COMP_ATTACK_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_COMP_ATTACK_MAXVALUE, FX_COMP_ATTACK_MINVALUE));

    addChild(KEY_COMP_RELEASETIME,
             new DuNumeric(FX_COMP_RELEASE_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_COMP_RELEASE_MAXVALUE, FX_COMP_RELEASE_MINVALUE));

    addChild(KEY_COMP_THRESHOLD,
             new DuNumeric(FX_COMP_THRESHOLD_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_COMP_THRESHOLD_MAXVALUE, FX_COMP_THRESHOLD_MINVALUE));

    addChild(KEY_COMP_RATIO,
             new DuNumeric(FX_COMP_RATIO_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_COMP_RATIO_MAXVALUE, FX_COMP_RATIO_MINVALUE));

    addChild(KEY_COMP_BOOST,
             new DuNumeric(FX_COMP_BOOST_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_COMP_BOOST_MAXVALUE, FX_COMP_BOOST_MINVALUE));

    addChild(KEY_COMP_KNEETYPE,
             new DuNumeric(FX_COMP_KNEE_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_COMP_KNEE_MAXVALUE, FX_COMP_KNEE_MINVALUE));

    addChild(KEY_COMP_EFFECTNAME,
             new DuString(QString(DEFAULT_EFFECTNAME), NAME_CARACT));
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
    QJsonValue jsonOnOff        = jsonCompressor[KEY_COMP_ONOFF];
    QJsonValue jsonAttTime      = jsonCompressor[KEY_COMP_ATTACKTIME];
    QJsonValue jsonRelTime      = jsonCompressor[KEY_COMP_RELEASETIME];
    QJsonValue jsonThreshold    = jsonCompressor[KEY_COMP_THRESHOLD];
    QJsonValue jsonRatio        = jsonCompressor[KEY_COMP_RATIO];
    QJsonValue jsonBoost        = jsonCompressor[KEY_COMP_BOOST];
    QJsonValue jsonKneeType     = jsonCompressor[KEY_COMP_KNEETYPE];
    QJsonValue jsonEffectName   = jsonCompressor[KEY_COMP_EFFECTNAME];

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


int DuCompressor::getOnOff() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_COMP_ONOFF);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuCompressor::setOnOff(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_COMP_ONOFF);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuCompressor::getAttackTime() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_COMP_ATTACKTIME);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuCompressor::setAttackTime(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_COMP_ATTACKTIME);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuCompressor::getReleaseTime() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_COMP_RELEASETIME);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuCompressor::setReleaseTime(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_COMP_RELEASETIME);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuCompressor::getThreshold() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_COMP_THRESHOLD);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuCompressor::setThreshold(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_COMP_THRESHOLD);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuCompressor::getRatio() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_COMP_RATIO);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuCompressor::setRatio(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_COMP_RATIO);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuCompressor::getBoost() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_COMP_BOOST);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuCompressor::setBoost(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_COMP_BOOST);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuCompressor::getKneeType() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_COMP_KNEETYPE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuCompressor::setKneeType(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_COMP_KNEETYPE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


QString DuCompressor::getEffectName() const
{
    const DuStringConstPtr &tmp = getChildAs<DuString>(KEY_COMP_EFFECTNAME);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuCompressor::setEffectName(const QString &value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_COMP_EFFECTNAME);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}
