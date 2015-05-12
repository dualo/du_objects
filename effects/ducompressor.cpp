#include "ducompressor.h"

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


DuCompressor *DuCompressor::fromDuMusicFile(const FX_compressor &du_compressor)
{
    DuCompressor *compressor = new DuCompressor();

    compressor->setOnOff(du_compressor.c_on_off);

    compressor->setAttackTime(du_compressor.c_attacktime);
    compressor->setReleaseTime(du_compressor.c_releasetime);

    compressor->setThreshold(du_compressor.c_threshold);
    compressor->setRatio(du_compressor.c_ratio);
    compressor->setBoost(du_compressor.c_boost);
    compressor->setKneeType(du_compressor.c_kneetype);

    compressor->setEffectName(
            QString(QByteArray((char *)du_compressor.c_name, NAME_CARACT)));

    return compressor;
}


DuCompressor *DuCompressor::fromJson(const QJsonObject &jsonCompressor)
{
    DuCompressor *compressor = new DuCompressor();
    QStringList &keyList = compressor->keys();

    bool test = true;
    for (int i = 0; i < keyList.count(); i++)
    {
        test = test && jsonCompressor.contains(keyList[i]);
    }

    if (!test)
        return NULL;

    compressor->setOnOff(jsonCompressor[KEY_COMP_ONOFF].toInt());

    compressor->setAttackTime(jsonCompressor[KEY_COMP_ATTACKTIME].toInt());
    compressor->setReleaseTime(jsonCompressor[KEY_COMP_RELEASETIME].toInt());
    compressor->setThreshold(jsonCompressor[KEY_COMP_THRESHOLD].toInt());

    compressor->setRatio(jsonCompressor[KEY_COMP_RATIO].toInt());
    compressor->setBoost(jsonCompressor[KEY_COMP_BOOST].toInt());
    compressor->setKneeType(jsonCompressor[KEY_COMP_KNEETYPE].toInt());

    compressor->setEffectName(jsonCompressor[KEY_COMP_EFFECTNAME].toString());

    return compressor;
}


int DuCompressor::getOnOff() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_COMP_ONOFF));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuCompressor::setOnOff(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_COMP_ONOFF));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


int DuCompressor::getAttackTime() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_COMP_ATTACKTIME));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuCompressor::setAttackTime(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_COMP_ATTACKTIME));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuCompressor::getReleaseTime() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_COMP_RELEASETIME));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuCompressor::setReleaseTime(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_COMP_RELEASETIME));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuCompressor::getThreshold() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_COMP_THRESHOLD));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuCompressor::setThreshold(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_COMP_THRESHOLD));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuCompressor::getRatio() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_COMP_RATIO));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuCompressor::setRatio(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_COMP_RATIO));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuCompressor::getBoost() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_COMP_BOOST));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuCompressor::setBoost(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_COMP_BOOST));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuCompressor::getKneeType() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_COMP_KNEETYPE));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuCompressor::setKneeType(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_COMP_KNEETYPE));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


QString DuCompressor::getEffectName() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_COMP_EFFECTNAME));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

void DuCompressor::setEffectName(QString &value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_COMP_EFFECTNAME));

    if (tmp == NULL)
        return;

    tmp->setString(value);
}
