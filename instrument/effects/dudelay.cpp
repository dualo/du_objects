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

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuDelay::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    return delay;
}


QByteArray DuDelay::toDuMusicBinary() const
{
    FX_delay du_delay;
    std::memset(&du_delay, 0, static_cast<size_t>(size()));

    int tmpNum = 0;


    tmpNum = getOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_delay.d_on_off = static_cast<quint8>(tmpNum);

    tmpNum = getMode();
    if (tmpNum == -1)
        return QByteArray();
    du_delay.d_mode = static_cast<quint8>(tmpNum);

    tmpNum = getPreLowPassFilter();
    if (tmpNum == -1)
        return QByteArray();
    du_delay.d_prelp = static_cast<quint8>(tmpNum);

    tmpNum = getEffectLevel();
    if (tmpNum == -1)
        return QByteArray();
    du_delay.d_level = static_cast<quint8>(tmpNum);

    tmpNum = getEffectTime();
    if (tmpNum == -1)
        return QByteArray();
    du_delay.d_time = static_cast<quint8>(tmpNum);

    tmpNum = getFeedback();
    if (tmpNum == -1)
        return QByteArray();
    du_delay.d_feedback = static_cast<quint8>(tmpNum);

    tmpNum = getHDAmp();
    if (tmpNum == -1)
        return QByteArray();
    du_delay.d_hdamp = static_cast<quint8>(tmpNum);


    return QByteArray(reinterpret_cast<char*>(&du_delay), size());
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
