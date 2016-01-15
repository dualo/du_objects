#include "dudelay.h"

#include <cstring>

#include <QJsonObject>

#include "../../general/dunumeric.h"
#include "../../general/dustring.h"

DU_OBJECT_IMPL(DuDelay)

DuDelay::DuDelay() :
    DuEffectSettings()
{
    addChild(KeyOnOff,
             new DuNumeric(FX_DELAY_ONOFF_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DELAY_ONOFF_MAXVALUE, FX_DELAY_ONOFF_MINVALUE));

    addChild(KeyMode,
             new DuNumeric(FX_DELAY_MODE_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DELAY_MODE_MAXVALUE, FX_DELAY_MODE_MINVALUE));

    addChild(KeyPreLowPassFilter,
             new DuNumeric(FX_DELAY_PRELP_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DELAY_PRELP_MAXVALUE, FX_DELAY_PRELP_MINVALUE));

    addChild(KeyEffectLevel,
             new DuNumeric(FX_DELAY_LEVEL_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DELAY_LEVEL_MAXVALUE, FX_DELAY_LEVEL_MINVALUE));

    addChild(KeyEffectTime,
             new DuNumeric(FX_DELAY_TIME_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DELAY_TIME_MAXVALUE, FX_DELAY_TIME_MINVALUE));

    addChild(KeyFeedback,
             new DuNumeric(FX_DELAY_FEEDBACK_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DELAY_FEEDBACK_MAXVALUE, FX_DELAY_FEEDBACK_MINVALUE));

    addChild(KeyHDAmp,
             new DuNumeric(FX_DELAY_HDAMP_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DELAY_HDAMP_MAXVALUE, FX_DELAY_HDAMP_MINVALUE));

    addChild(KeyEffectName,
             new DuString(QStringLiteral(DEFAULT_EFFECTNAME), NAME_CARACT));
}

DuDelay::~DuDelay()
{
}

DuObjectPtr DuDelay::clone() const
{
    return DuDelayPtr(new DuDelay(*this));
}


DuDelayPtr DuDelay::fromDuMusicBinary(const FX_delay &du_delay)
{
    DuDelayPtr delay(new DuDelay);
    bool verif = true;

    verif = delay->setOnOff(du_delay.d_on_off) ? verif : false;

    verif = delay->setMode(du_delay.d_mode) ? verif : false;
    verif = delay->setPreLowPassFilter(du_delay.d_prelp) ? verif : false;

    verif = delay->setEffectLevel(du_delay.d_level) ? verif : false;
    verif = delay->setEffectTime(du_delay.d_time) ? verif : false;
    verif = delay->setFeedback(du_delay.d_feedback) ? verif : false;
    verif = delay->setHDAmp(du_delay.d_hdamp) ? verif : false;

    verif = delay->setEffectName(QString(QByteArray((char *)du_delay.d_name, NAME_CARACT))) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuDelay::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    return delay;
}


DuDelayPtr DuDelay::fromJson(const QJsonObject &jsonDelay)
{
    QJsonValue jsonOnOff        = jsonDelay[KeyOnOff];
    QJsonValue jsonMode         = jsonDelay[KeyMode];
    QJsonValue jsonLoPassFilt   = jsonDelay[KeyPreLowPassFilter];
    QJsonValue jsonEffectLvl    = jsonDelay[KeyEffectLevel];
    QJsonValue jsonEffectTime   = jsonDelay[KeyEffectTime];
    QJsonValue jsonFeedback     = jsonDelay[KeyFeedback];
    QJsonValue jsonHDAmp        = jsonDelay[KeyHDAmp];
    QJsonValue jsonEffectName   = jsonDelay[KeyEffectName];

    if (        !jsonOnOff.isDouble()       ||  !jsonMode.isDouble()
            ||  !jsonLoPassFilt.isDouble()  ||  !jsonEffectLvl.isDouble()
            ||  !jsonEffectTime.isDouble()  ||  !jsonFeedback.isDouble()
            ||  !jsonHDAmp.isDouble()       ||  !jsonEffectName.isString())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuDelay::fromJson():\n"
                    << "failed to generate DuDelay\n"
                    << "a json key did not contain the proper type";

        return DuDelayPtr();
    }


    DuDelayPtr delay(new DuDelay);
    bool verif = true;

    verif = delay->setOnOff(jsonOnOff.toInt()) ? verif : false;

    verif = delay->setMode(jsonMode.toInt()) ? verif : false;
    verif = delay->setPreLowPassFilter(jsonLoPassFilt.toInt()) ? verif : false;

    verif = delay->setEffectLevel(jsonEffectLvl.toInt()) ? verif : false;
    verif = delay->setEffectTime(jsonEffectTime.toInt()) ? verif : false;

    verif = delay->setFeedback(jsonFeedback.toInt()) ? verif : false;
    verif = delay->setHDAmp(jsonHDAmp.toInt()) ? verif : false;

    verif = delay->setEffectName(jsonEffectName.toString()) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuDelay::fromJson():\n"
                   << "an attribute was not properly set";
    }

    return delay;
}


QByteArray DuDelay::toDuMusicBinary() const
{
    FX_delay du_delay;
    std::memset((char*)&du_delay, 0, size());

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


DU_KEY_ACCESSORS_IMPL(DuDelay, OnOff,            Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuDelay, Mode,             Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuDelay, PreLowPassFilter, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuDelay, EffectLevel,      Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuDelay, EffectTime,       Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuDelay, Feedback,         Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuDelay, HDAmp,            Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuDelay, EffectName,       String, QString, QString())
