#include "dureverb.h"

#include <cstring>

#include <QJsonObject>


DuReverb::DuReverb()
{
    addChild(KEY_REVERB_LEVEL,
             new DuNumeric(FX_REVERB_LEVEL_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_REVERB_LEVEL_MAXVALUE, FX_REVERB_LEVEL_MINVALUE));

    addChild(KEY_REVERB_DIRECTLEVEL,
             new DuNumeric(FX_REVERB_DIRECTLEVEL_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_REVERB_DIRECTLEVEL_MAXVALUE, FX_REVERB_DIRECTLEVEL_MINVALUE));

    addChild(KEY_REVERB_REVERBSEND,
             new DuNumeric(FX_REVERB_REVSEND_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_REVERB_REVSEND_MAXVALUE, FX_REVERB_REVSEND_MINVALUE));

    addChild(KEY_REVERB_TONEGAIN,
             new DuNumeric(FX_REVERB_TONEGAIN_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_REVERB_TONEGAIN_MAXVALUE, FX_REVERB_TONEGAIN_MINVALUE));

    addChild(KEY_REVERB_TONEFREQUENCY,
             new DuNumeric(FX_REVERB_TONEFREQ_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_REVERB_TONEFREQ_MAXVALUE, FX_REVERB_TONEFREQ_MINVALUE));

    addChild(KEY_REVERB_PREHPFILTER,
             new DuNumeric(FX_REVERB_PREHP_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_REVERB_PREHP_MAXVALUE, FX_REVERB_PREHP_MINVALUE));

    addChild(KEY_REVERB_TIME,
             new DuNumeric(FX_REVERB_TIME_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_REVERB_TIME_MAXVALUE, FX_REVERB_TIME_MINVALUE));

    addChild(KEY_REVERB_ECHOFEEDBACK,
             new DuNumeric(FX_REVERB_ECHOFEED_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_REVERB_ECHOFEED_MAXVALUE, FX_REVERB_ECHOFEED_MINVALUE));

    addChild(KEY_REVERB_HDAMP,
             new DuNumeric(FX_REVERB_HDAMP_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_REVERB_HDAMP_MAXVALUE, FX_REVERB_HDAMP_MINVALUE));

    addChild(KEY_REVERB_THRESHOLDGATE,
             new DuNumeric(FX_REVERB_THRESGATE_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_REVERB_THRESGATE_MAXVALUE, FX_REVERB_THRESGATE_MINVALUE));

    addChild(KEY_REVERB_PREDELAYTIME,
             new DuNumeric(FX_REVERB_PREDELAYTIME_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_REVERB_PREDELAYTIME_MAXVALUE, FX_REVERB_PREDELAYTIME_MINVALUE));

    addChild(KEY_REVERB_EFFECTNAME,
             new DuString(DEFAULT_EFFECTNAME, NAME_CARACT));
}

DuReverb::~DuReverb()
{
}


DuObjectPtr DuReverb::clone() const
{
    return DuReverbPtr(new DuReverb(*this));
}


DuReverbPtr DuReverb::fromDuMusicBinary(const FX_reverb &du_reverb)
{
    DuReverbPtr reverb(new DuReverb);
    bool verif = true;

    verif = reverb->setLevel(du_reverb.r_level) ? verif : false;
    verif = reverb->setDirectLevel(du_reverb.r_directlevel) ? verif : false;
    verif = reverb->setReverbSend(du_reverb.r_revsend) ? verif : false;

    verif = reverb->setToneGain(du_reverb.r_tonegain) ? verif : false;
    verif = reverb->setToneFrequency(du_reverb.r_tonefreq) ? verif : false;
    verif = reverb->setPreHPFilter(du_reverb.r_prehp) ? verif : false;

    verif = reverb->setTime(du_reverb.r_time) ? verif : false;
    verif = reverb->setEchoFeedback(du_reverb.r_echofeedback) ? verif : false;
    verif = reverb->setHDAmp(du_reverb.r_hdamp) ? verif : false;
    verif = reverb->setThresholdGate(du_reverb.r_thresgate) ? verif : false;
    verif = reverb->setPreDelayTime(du_reverb.r_predelaytime) ? verif : false;

    verif = verif && reverb->setEffectName(
                QString(QByteArray((char *)du_reverb.r_name, NAME_CARACT)));

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuReverb::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    return reverb;
}


DuReverbPtr DuReverb::fromJson(const QJsonObject &jsonReverb)
{
    QJsonValue jsonLevel                    = jsonReverb[KEY_REVERB_LEVEL];
    QJsonValue jsonDirectLevel              = jsonReverb[KEY_REVERB_DIRECTLEVEL];
    QJsonValue jsonRevSend                  = jsonReverb[KEY_REVERB_REVERBSEND];
    QJsonValue jsonToneGain                 = jsonReverb[KEY_REVERB_TONEGAIN];
    QJsonValue jsonToneFreq                 = jsonReverb[KEY_REVERB_TONEFREQUENCY];
    QJsonValue jsonPreHPFilter              = jsonReverb[KEY_REVERB_PREHPFILTER];
    QJsonValue jsonTime                     = jsonReverb[KEY_REVERB_TIME];
    QJsonValue jsonEchoFeedback             = jsonReverb[KEY_REVERB_ECHOFEEDBACK];
    QJsonValue jsonHDAmp                    = jsonReverb[KEY_REVERB_HDAMP];
    QJsonValue jsonThreshGate               = jsonReverb[KEY_REVERB_THRESHOLDGATE];
    QJsonValue jsonPreDelayTime             = jsonReverb[KEY_REVERB_PREDELAYTIME];
    QJsonValue jsonEffectName               = jsonReverb[KEY_REVERB_EFFECTNAME];

    if (        !jsonLevel.isDouble()           ||  !jsonDirectLevel.isDouble()
            ||  !jsonRevSend.isDouble()         ||  !jsonToneGain.isDouble()
            ||  !jsonToneFreq.isDouble()        ||  !jsonPreHPFilter.isDouble()
            ||  !jsonTime.isDouble()            ||  !jsonEchoFeedback.isDouble()
            ||  !jsonHDAmp.isDouble()           ||  !jsonThreshGate.isDouble()
            ||  !jsonPreDelayTime.isDouble()    ||  !jsonEffectName.isString())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuReverb::fromJson():\n"
                    << "failed to generate DuReverb\n"
                    << "a json key did not contain the proper type";

        return DuReverbPtr();
    }


    DuReverbPtr reverb(new DuReverb);
    bool verif = true;

    verif = reverb->setLevel(jsonLevel.toInt()) ? verif : false;
    verif = reverb->setDirectLevel(jsonDirectLevel.toInt()) ? verif : false;
    verif = reverb->setReverbSend(jsonRevSend.toInt()) ? verif : false;

    verif = reverb->setToneGain(jsonToneGain.toInt()) ? verif : false;
    verif = reverb->setToneFrequency(jsonToneFreq.toInt()) ? verif : false;
    verif = reverb->setPreHPFilter(jsonPreHPFilter.toInt()) ? verif : false;

    verif = reverb->setTime(jsonTime.toInt()) ? verif : false;
    verif = reverb->setEchoFeedback(jsonEchoFeedback.toInt()) ? verif : false;
    verif = reverb->setHDAmp(jsonHDAmp.toInt()) ? verif : false;
    verif = reverb->setThresholdGate(jsonThreshGate.toInt()) ? verif : false;
    verif = reverb->setPreDelayTime(jsonPreDelayTime.toInt()) ? verif : false;

    verif = reverb->setEffectName(jsonEffectName.toString()) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuReverb::fromJson():\n"
                   << "an attribute was not properly set";
    }

    return reverb;
}


QByteArray DuReverb::toDuMusicBinary() const
{
    FX_reverb du_reverb;

    QString tmpStr;
    int tmpNum = 0;


    tmpNum = getLevel();
    if (tmpNum == -1)
        return QByteArray();
    du_reverb.r_level = tmpNum;

    tmpNum = getDirectLevel();
    if (tmpNum == -1)
        return QByteArray();
    du_reverb.r_directlevel = tmpNum;

    tmpNum = getReverbSend();
    if (tmpNum == -1)
        return QByteArray();
    du_reverb.r_revsend = tmpNum;


    tmpNum = getToneGain();
    if (tmpNum == -1)
        return QByteArray();
    du_reverb.r_tonegain = tmpNum;

    tmpNum = getToneFrequency();
    if (tmpNum == -1)
        return QByteArray();
    du_reverb.r_tonefreq = tmpNum;

    tmpNum = getPreHPFilter();
    if (tmpNum == -1)
        return QByteArray();
    du_reverb.r_prehp = tmpNum;


    tmpNum = getTime();
    if (tmpNum == -1)
        return QByteArray();
    du_reverb.r_time = tmpNum;

    tmpNum = getEchoFeedback();
    if (tmpNum == -1)
        return QByteArray();
    du_reverb.r_echofeedback = tmpNum;

    tmpNum = getHDAmp();
    if (tmpNum == -1)
        return QByteArray();
    du_reverb.r_hdamp = tmpNum;

    tmpNum = getThresholdGate();
    if (tmpNum == -1)
        return QByteArray();
    du_reverb.r_thresgate = tmpNum;

    tmpNum = getPreDelayTime();
    if (tmpNum == -1)
        return QByteArray();
    du_reverb.r_predelaytime = tmpNum;


    QByteArray tmpName(NAME_CARACT, (char)0x00);
    tmpStr = getEffectName();
    if (tmpStr.isNull())
        return QByteArray();
    tmpName.prepend(tmpStr.toUtf8());

    std::memcpy(du_reverb.r_name, tmpName.data(), NAME_CARACT);


    return QByteArray((char *)&(du_reverb), size());
}


int DuReverb::size() const
{
    return FX_REVERB_SIZE;
}


int DuReverb::getLevel() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_REVERB_LEVEL);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuReverb::setLevel(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_REVERB_LEVEL);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuReverb::getDirectLevel() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_REVERB_DIRECTLEVEL);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuReverb::setDirectLevel(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_REVERB_DIRECTLEVEL);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuReverb::getReverbSend() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_REVERB_REVERBSEND);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuReverb::setReverbSend(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_REVERB_REVERBSEND);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuReverb::getToneGain() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_REVERB_TONEGAIN);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuReverb::setToneGain(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_REVERB_TONEGAIN);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuReverb::getToneFrequency() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_REVERB_TONEFREQUENCY);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuReverb::setToneFrequency(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_REVERB_TONEFREQUENCY);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuReverb::getPreHPFilter() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_REVERB_PREHPFILTER);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuReverb::setPreHPFilter(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_REVERB_PREHPFILTER);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuReverb::getTime() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_REVERB_TIME);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuReverb::setTime(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_REVERB_TIME);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuReverb::getEchoFeedback() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_REVERB_ECHOFEEDBACK);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuReverb::setEchoFeedback(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_REVERB_ECHOFEEDBACK);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuReverb::getHDAmp() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_REVERB_HDAMP);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuReverb::setHDAmp(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_REVERB_HDAMP);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuReverb::getThresholdGate() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_REVERB_THRESHOLDGATE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuReverb::setThresholdGate(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_REVERB_THRESHOLDGATE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuReverb::getPreDelayTime() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_REVERB_PREDELAYTIME);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuReverb::setPreDelayTime(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_REVERB_PREDELAYTIME);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


QString DuReverb::getEffectName() const
{
    const DuStringConstPtr &tmp = getChildAs<DuString>(KEY_REVERB_EFFECTNAME);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuReverb::setEffectName(const QString &value)
{
    const DuStringPtr &tmp = getChildAs<DuString>(KEY_REVERB_EFFECTNAME);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}
