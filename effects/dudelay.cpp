#include "dudelay.h"

#include <QJsonObject>
#include <QDebug>

DU_OBJECT_IMPL(DuDelay)

DuDelay::DuDelay() :
    DuEffectSettings()
{
    //TODO: define correct maxima

    addChild(KEY_DELAY_ONOFF,
             new DuNumeric(FX_DELAY_ONOFF_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           0x7F, FX_DELAY_ONOFF_MINVALUE));

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
                           0x7F, FX_DELAY_FEEDBACK_MINVALUE));

    addChild(KEY_DELAY_HDAMP,
             new DuNumeric(FX_DELAY_HDAMP_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DELAY_HDAMP_MAXVALUE, FX_DELAY_HDAMP_MINVALUE));

    addChild(KEY_DELAY_EFFECTNAME,
             new DuString(QString(DEFAULT_EFFECTNAME), NAME_CARACT));
}

DuDelay::~DuDelay()
{
}

DuObjectPtr DuDelay::clone() const
{
    return DuDelayPtr(new DuDelay(*this));
}


DuDelayPtr DuDelay::fromDuMusicFile(const FX_delay &du_delay)
{
    DuDelayPtr delay(new DuDelay);
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
        return DuDelayPtr();
    }

    return delay;
}


DuDelayPtr DuDelay::fromJson(const QJsonObject &jsonDelay)
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

        return DuDelayPtr();


    DuDelayPtr delay(new DuDelay);
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
        return DuDelayPtr();
    }

    return delay;
}


QByteArray DuDelay::toDuMusicFile() const
{
    FX_delay du_delay;

    QString tmpStr;
    int tmpNum = 0;


    tmpNum = getOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_delay.d_on_off = tmpNum;

    tmpNum = getMode();
    if (tmpNum == -1)
        return QByteArray();
    du_delay.d_mode = tmpNum;

    tmpNum = getPreLowPassFilter();
    if (tmpNum == -1)
        return QByteArray();
    du_delay.d_prelp = tmpNum;

    tmpNum = getEffectLevel();
    if (tmpNum == -1)
        return QByteArray();
    du_delay.d_level = tmpNum;

    tmpNum = getEffectTime();
    if (tmpNum == -1)
        return QByteArray();
    du_delay.d_time = tmpNum;

    tmpNum = getFeedback();
    if (tmpNum == -1)
        return QByteArray();
    du_delay.d_feedback = tmpNum;

    tmpNum = getHDAmp();
    if (tmpNum == -1)
        return QByteArray();
    du_delay.d_hdamp = tmpNum;


    QByteArray tmpName(NAME_CARACT, (char)0x00);
    tmpStr = getEffectName();
    if (tmpStr.isNull())
        return QByteArray();
    tmpName.prepend(tmpStr.toUtf8());

    std::memcpy(du_delay.d_name, tmpName.data(), NAME_CARACT);


    return QByteArray((char *)&(du_delay), size());
}


int DuDelay::size() const
{
    return FX_DELAY_SIZE;
}


int DuDelay::getOnOff() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_DELAY_ONOFF);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDelay::setOnOff(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_DELAY_ONOFF);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuDelay::getMode() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_DELAY_MODE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDelay::setMode(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_DELAY_MODE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuDelay::getPreLowPassFilter() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_DELAY_PRELOWPASSFILTER);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDelay::setPreLowPassFilter(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_DELAY_PRELOWPASSFILTER);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuDelay::getEffectLevel() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_DELAY_EFFECTLEVEL);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDelay::setEffectLevel(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_DELAY_EFFECTLEVEL);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuDelay::getEffectTime() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_DELAY_EFFECTTIME);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDelay::setEffectTime(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_DELAY_EFFECTTIME);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuDelay::getFeedback() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_DELAY_FEEDBACK);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDelay::setFeedback(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_DELAY_FEEDBACK);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuDelay::getHDAmp() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_DELAY_HDAMP);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDelay::setHDAmp(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_DELAY_HDAMP);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


QString DuDelay::getEffectName() const
{
    const DuStringConstPtr& tmp = getChildAs<DuString>(KEY_DELAY_EFFECTNAME);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuDelay::setEffectName(const QString &value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_DELAY_EFFECTNAME);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}
