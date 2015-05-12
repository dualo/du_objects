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
    DuDelay *delay = new DuDelay();

    delay->setOnOff(du_delay.d_on_off);

    delay->setMode(du_delay.d_mode);
    delay->setPreLowPassFilter(du_delay.d_prelp);

    delay->setEffectLevel(du_delay.d_level);
    delay->setEffectTime(du_delay.d_time);
    delay->setFeedback(du_delay.d_feedback);
    delay->setHDAmp(du_delay.d_hdamp);

    delay->setEffectName(QString(QByteArray((char *)du_delay.d_name, NAME_CARACT)));

    return delay;
}


DuDelay *DuDelay::fromJson(const QJsonObject &jsonDelay)
{
    DuDelay *delay = new DuDelay();
    const QStringList &keyList = delay->keys();

    bool test = true;
    for (int i = 0; i < keyList.count(); i++)
    {
        test = test && jsonDelay.contains(keyList[i]);
    }

    if (!test)
        return NULL;

    delay->setOnOff(jsonDelay[KEY_DELAY_ONOFF].toInt());

    delay->setMode(jsonDelay[KEY_DELAY_MODE].toInt());
    delay->setPreLowPassFilter(jsonDelay[KEY_DELAY_PRELOWPASSFILTER].toInt());

    delay->setEffectLevel(jsonDelay[KEY_DELAY_EFFECTLEVEL].toInt());
    delay->setEffectTime(jsonDelay[KEY_DELAY_EFFECTTIME].toInt());

    delay->setFeedback(jsonDelay[KEY_DELAY_FEEDBACK].toInt());
    delay->setHDAmp(jsonDelay[KEY_DELAY_HDAMP].toInt());

    delay->setEffectName(jsonDelay[KEY_DELAY_EFFECTNAME].toString());

    return delay;
}


int DuDelay::getOnOff() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_ONOFF));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuDelay::setOnOff(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_ONOFF));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


int DuDelay::getMode() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_MODE));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuDelay::setMode(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_MODE));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuDelay::getPreLowPassFilter() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_PRELOWPASSFILTER));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuDelay::setPreLowPassFilter(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_PRELOWPASSFILTER));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuDelay::getEffectLevel() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_EFFECTLEVEL));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuDelay::setEffectLevel(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_EFFECTLEVEL));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuDelay::getEffectTime() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_EFFECTTIME));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuDelay::setEffectTime(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_EFFECTTIME));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuDelay::getFeedback() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_FEEDBACK));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuDelay::setFeedback(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_FEEDBACK));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuDelay::getHDAmp() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_HDAMP));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuDelay::setHDAmp(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DELAY_HDAMP));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


QString DuDelay::getEffectName() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_DELAY_EFFECTNAME));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

void DuDelay::setEffectName(const QString &value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_DELAY_EFFECTNAME));

    if (tmp == NULL)
        return;

    tmp->setString(value);
}
