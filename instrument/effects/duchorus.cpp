#include "duchorus.h"

#include <cstring>

#include <QJsonObject>

#include "../../general/dunumeric.h"
#include "../../general/dustring.h"

DU_OBJECT_IMPL(DuChorus)

DuChorus::DuChorus() :
    DuEffectSettings()
{
    addChild(KeyMode,
             new DuNumeric(FX_CHORUS_MODE_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_CHORUS_MODE_MAXVALUE, FX_CHORUS_MODE_MINVALUE));

    addChild(KeyEffectLevel,
             new DuNumeric(FX_CHORUS_LEVEL_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_CHORUS_LEVEL_MAXVALUE, FX_CHORUS_LEVEL_MINVALUE));

    addChild(KeyDelayTime,
             new DuNumeric(FX_CHORUS_DELAY_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_CHORUS_DELAY_MAXVALUE, FX_CHORUS_DELAY_MINVALUE));

    addChild(KeyFeedback,
             new DuNumeric(FX_CHORUS_FEEDBACK_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_CHORUS_FEEDBACK_MAXVALUE, FX_CHORUS_FEEDBACK_MINVALUE));

    addChild(KeyInputHighPassFilterFrequency,
             new DuNumeric(FX_CHORUS_INPUTHP_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_CHORUS_INPUTHP_MAXVALUE, FX_CHORUS_INPUTHP_MINVALUE));

    addChild(KeyHDAmp,
             new DuNumeric(FX_CHORUS_HDAMP_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_CHORUS_HDAMP_MAXVALUE, FX_CHORUS_HDAMP_MINVALUE));

    addChild(KeyModulationDepth,
             new DuNumeric(FX_CHORUS_MODDEPTH_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_CHORUS_MODDEPTH_MAXVALUE, FX_CHORUS_MODDEPTH_MINVALUE));

    addChild(KeyModulationRate,
             new DuNumeric(FX_CHORUS_MODRATE_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_CHORUS_MODRATE_MAXVALUE, FX_CHORUS_MODRATE_MINVALUE));

    addChild(KeyTremoloShape,
             new DuNumeric(FX_CHORUS_TREMOLO_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_CHORUS_TREMOLO_MAXVALUE, FX_CHORUS_TREMOLO_MINVALUE));

    addChild(KeyRotarySpeed,
             new DuNumeric(FX_CHORUS_ROTARY_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_CHORUS_ROTARY_MAXVALUE, FX_CHORUS_ROTARY_MINVALUE));
}

DuObjectPtr DuChorus::clone() const
{
    return DuChorusPtr(new DuChorus(*this));
}


DuChorusPtr DuChorus::fromDuMusicBinary(const FX_chorus &du_chorus)
{
    DuChorusPtr chorus(new DuChorus);
    bool verif = true;

    verif = chorus->setMode(du_chorus.c_mode) ? verif : false;

    verif = chorus->setEffectLevel(du_chorus.c_effectlevel) ? verif : false;
    verif = chorus->setDelayTime(du_chorus.c_delaytime) ? verif : false;
    verif = chorus->setFeedback(du_chorus.c_feedback) ? verif : false;
    verif = chorus->setInputHighPassFilterFrequency(du_chorus.c_inputhighpassfilter) ? verif : false;
    verif = chorus->setHDAmp(du_chorus.c_hdamp) ? verif : false;

    verif = chorus->setModulationDepth(du_chorus.c_modulationdepth) ? verif : false;
    verif = chorus->setModulationRate(du_chorus.c_modulationrate) ? verif : false;
    verif = chorus->setTremoloShape(du_chorus.c_tremoloshape) ? verif : false;
    verif = chorus->setRotarySpeed(du_chorus.c_rotaryspeed) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuChorus::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    return chorus;
}


QByteArray DuChorus::toDuMusicBinary() const
{
    FX_chorus du_chorus;
    std::memset(&du_chorus, 0, static_cast<size_t>(size()));

    int tmpNum = 0;


    tmpNum = getMode();
    if (tmpNum == -1)
        return QByteArray();
    du_chorus.c_mode = static_cast<quint8>(tmpNum);

    tmpNum = getEffectLevel();
    if (tmpNum == -1)
        return QByteArray();
    du_chorus.c_effectlevel = static_cast<quint8>(tmpNum);

    tmpNum = getDelayTime();
    if (tmpNum == -1)
        return QByteArray();
    du_chorus.c_delaytime = static_cast<quint8>(tmpNum);

    tmpNum = getFeedback();
    if (tmpNum == -1)
        return QByteArray();
    du_chorus.c_feedback = static_cast<quint8>(tmpNum);

    tmpNum = getInputHighPassFilterFrequency();
    if (tmpNum == -1)
        return QByteArray();
    du_chorus.c_inputhighpassfilter = static_cast<quint8>(tmpNum);

    tmpNum = getHDAmp();
    if (tmpNum == -1)
        return QByteArray();
    du_chorus.c_hdamp = static_cast<quint8>(tmpNum);

    tmpNum = getModulationDepth();
    if (tmpNum == -1)
        return QByteArray();
    du_chorus.c_modulationdepth = static_cast<quint8>(tmpNum);

    tmpNum = getModulationRate();
    if (tmpNum == -1)
        return QByteArray();
    du_chorus.c_modulationrate = static_cast<quint8>(tmpNum);

    tmpNum = getTremoloShape();
    if (tmpNum == -1)
        return QByteArray();
    du_chorus.c_tremoloshape = static_cast<quint8>(tmpNum);

    tmpNum = getRotarySpeed();
    if (tmpNum == -1)
        return QByteArray();
    du_chorus.c_rotaryspeed = static_cast<quint8>(tmpNum);


    return QByteArray(reinterpret_cast<char*>(&du_chorus), size());
}


int DuChorus::size() const
{
    return FX_CHORUS_SIZE;
}


DU_KEY_ACCESSORS_IMPL(DuChorus, Mode,                         Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuChorus, EffectLevel,                  Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuChorus, DelayTime,                    Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuChorus, Feedback,                     Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuChorus, InputHighPassFilterFrequency, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuChorus, HDAmp,                        Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuChorus, ModulationDepth,              Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuChorus, ModulationRate,               Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuChorus, TremoloShape,                 Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuChorus, RotarySpeed,                  Numeric, int, -1)
