#include "dudelay.h"

DuDelay::DuDelay() :
    DuEffectSettings()
{
    addChild(KEY_DELAY_ONOFF,
             new DuNumeric(FX_DELAY_ONOFF_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DELAY_ONOFF_MAXVALUE, FX_DELAY_ONOFF_MINVALUE));

    addChild(KEY_DELAY_MODE,
             new DuNumeric(FX_DELAY_MODE_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DELAY_MODE_MAXVALUE, FX_DELAY_MODE_MINVALUE));

    addChild(KEY_DELAY_PRELOWPASSFILTER,
             new DuNumeric(FX_DELAY_PRELP_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DELAY_PRELP_MAXVALUE, FX_DELAY_PRELP_MINVALUE));

    addChild(KEY_DELAY_EFFECTLEVEL,
             new DuNumeric(FX_DELAY_LEVEL_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DELAY_LEVEL_MAXVALUE, FX_DELAY_LEVEL_MINVALUE));

    addChild(KEY_DELAY_EFFECTTIME,
             new DuNumeric(FX_DELAY_TIME_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DELAY_TIME_MAXVALUE, FX_DELAY_TIME_MINVALUE));

    addChild(KEY_DELAY_FEEDBACK,
             new DuNumeric(FX_DELAY_FEEDBACK_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DELAY_FEEDBACK_MAXVALUE, FX_DELAY_FEEDBACK_MINVALUE));

    addChild(KEY_DELAY_HDAMP,
             new DuNumeric(FX_DELAY_HDAMP_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DELAY_HDAMP_MAXVALUE, FX_DELAY_HDAMP_MINVALUE));

    addChild(KEY_DELAY_EFFECTNAME,
             new DuString(QString(DEFAULT_EFFECTNAME), NAME_CARACT));
}

DuDelay::~DuDelay()
{
}


DuDelay *DuDelay::fromDuMusicFile(const FX_delay &du_delay)
{
    DuDelay *delay = new DuDelay;
    bool verif = true;

    verif = verif && delay->setOnOff(du_delay.d_on_off);

    verif = verif && delay->setMode(du_delay.d_mode);
    verif = verif && delay->setPreLowPassFilter(du_delay.d_prelp);

    verif = verif && delay->setEffectLevel(du_delay.d_level);
    verif = verif && delay->setEffectTime(du_delay.d_time);
    verif = verif && delay->setFeedback(du_delay.d_feedback);
    verif = verif && delay->setHDAmp(du_delay.d_hdamp);

    verif = verif && delay->setEffectName(
                QString(QByteArray((char *)du_delay.d_name, NAME_CARACT)));

    if (!verif)
    {
        delete delay;
        return NULL;
    }

    return delay;
}


DuDelay *DuDelay::fromJson(const QJsonObject &jsonDelay)
{
    QJsonValue jsonOnOff        = jsonDelay[KEY_DELAY_ONOFF];
    QJsonValue jsonMode         = jsonDelay[KEY_DELAY_MODE];
    QJsonValue jsonLoPassFilt   = jsonDelay[KEY_DELAY_PRELOWPASSFILTER];
    QJsonValue jsonEffectLvl    = jsonDelay[KEY_DELAY_EFFECTLEVEL];
    QJsonValue jsonEffectTime   = jsonDelay[KEY_DELAY_EFFECTTIME];
    QJsonValue jsonFeedback     = jsonDelay[KEY_DELAY_FEEDBACK];
    QJsonValue jsonHDAmp        = jsonDelay[KEY_DELAY_HDAMP];
    QJsonValue jsonEffectName   = jsonDelay[KEY_DELAY_EFFECTNAME];

    if (        !jsonOnOff.isDouble()       ||  !jsonMode.isDouble()
            ||  !jsonLoPassFilt.isDouble()  ||  !jsonEffectLvl.isDouble()
            ||  !jsonEffectTime.isDouble()  ||  !jsonFeedback.isDouble()
            ||  !jsonHDAmp.isDouble()       ||  !jsonEffectName.isString())

        return NULL;


    DuDelay *delay = new DuDelay;
    bool verif = true;

    verif = verif && delay->setOnOff(jsonOnOff.toInt());

    verif = verif && delay->setMode(jsonMode.toInt());
    verif = verif && delay->setPreLowPassFilter(jsonLoPassFilt.toInt());

    verif = verif && delay->setEffectLevel(jsonEffectLvl.toInt());
    verif = verif && delay->setEffectTime(jsonEffectTime.toInt());

    verif = verif && delay->setFeedback(jsonFeedback.toInt());
    verif = verif && delay->setHDAmp(jsonHDAmp.toInt());

    verif = verif && delay->setEffectName(jsonEffectName.toString());

    if (!verif)
    {
        delete delay;
        return NULL;
    }

    return delay;
}


int DuDelay::size() const
{
    return FX_DELAY_SIZE;
}


int DuDelay::getOnOff() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_ONOFF));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDelay::setOnOff(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_ONOFF));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuDelay::getMode() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_MODE));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDelay::setMode(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_MODE));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuDelay::getPreLowPassFilter() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_PRELOWPASSFILTER));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDelay::setPreLowPassFilter(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_PRELOWPASSFILTER));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuDelay::getEffectLevel() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_EFFECTLEVEL));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDelay::setEffectLevel(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_EFFECTLEVEL));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuDelay::getEffectTime() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_EFFECTTIME));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDelay::setEffectTime(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_EFFECTTIME));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuDelay::getFeedback() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_FEEDBACK));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDelay::setFeedback(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_FEEDBACK));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuDelay::getHDAmp() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_HDAMP));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDelay::setHDAmp(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_HDAMP));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


QString DuDelay::getEffectName() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_DELAY_EFFECTNAME));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

bool DuDelay::setEffectName(const QString &value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_DELAY_EFFECTNAME));

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}
