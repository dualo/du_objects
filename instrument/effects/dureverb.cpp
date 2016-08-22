#include "dureverb.h"

#include <cstring>

#include <QJsonObject>

#include "../../general/dunumeric.h"
#include "../../general/dustring.h"


DuReverb::DuReverb()
{
    addChild(KeyLevel,
             new DuNumeric(FX_REVERB_LEVEL_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_REVERB_LEVEL_MAXVALUE, FX_REVERB_LEVEL_MINVALUE));

    addChild(KeyDirectLevel,
             new DuNumeric(FX_REVERB_DIRECTLEVEL_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_REVERB_DIRECTLEVEL_MAXVALUE, FX_REVERB_DIRECTLEVEL_MINVALUE));

    addChild(KeyReverbSend,
             new DuNumeric(FX_REVERB_REVSEND_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_REVERB_REVSEND_MAXVALUE, FX_REVERB_REVSEND_MINVALUE));

    addChild(KeyToneGain,
             new DuNumeric(FX_REVERB_TONEGAIN_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_REVERB_TONEGAIN_MAXVALUE, FX_REVERB_TONEGAIN_MINVALUE));

    addChild(KeyToneFrequency,
             new DuNumeric(FX_REVERB_TONEFREQ_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_REVERB_TONEFREQ_MAXVALUE, FX_REVERB_TONEFREQ_MINVALUE));

    addChild(KeyPreHighPassFilter,
             new DuNumeric(FX_REVERB_PREHP_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_REVERB_PREHP_MAXVALUE, FX_REVERB_PREHP_MINVALUE));

    addChild(KeyTime,
             new DuNumeric(FX_REVERB_TIME_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_REVERB_TIME_MAXVALUE, FX_REVERB_TIME_MINVALUE));

    addChild(KeyEchoFeedback,
             new DuNumeric(FX_REVERB_ECHOFEED_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_REVERB_ECHOFEED_MAXVALUE, FX_REVERB_ECHOFEED_MINVALUE));

    addChild(KeyHDAmp,
             new DuNumeric(FX_REVERB_HDAMP_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_REVERB_HDAMP_MAXVALUE, FX_REVERB_HDAMP_MINVALUE));

    addChild(KeyThresholdGate,
             new DuNumeric(FX_REVERB_THRESGATE_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_REVERB_THRESGATE_MAXVALUE, FX_REVERB_THRESGATE_MINVALUE));

    addChild(KeyPreDelayTime,
             new DuNumeric(FX_REVERB_PREDELAYTIME_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_REVERB_PREDELAYTIME_MAXVALUE, FX_REVERB_PREDELAYTIME_MINVALUE));

    addChild(KeyEffectName,
             new DuString(QStringLiteral(DEFAULT_EFFECTNAME), NAME_CARACT));
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
    verif = reverb->setPreHighPassFilter(du_reverb.r_prehp) ? verif : false;

    verif = reverb->setTime(du_reverb.r_time) ? verif : false;
    verif = reverb->setEchoFeedback(du_reverb.r_echofeedback) ? verif : false;
    verif = reverb->setHDAmp(du_reverb.r_hdamp) ? verif : false;
    verif = reverb->setThresholdGate(du_reverb.r_thresgate) ? verif : false;
    verif = reverb->setPreDelayTime(du_reverb.r_predelaytime) ? verif : false;

    verif = reverb->setEffectName(QString::fromLatin1(reinterpret_cast<const char*>(du_reverb.r_name), NAME_CARACT)) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuReverb::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    return reverb;
}


QByteArray DuReverb::toDuMusicBinary() const
{
    FX_reverb du_reverb;
    std::memset((char*)&du_reverb, 0, size());

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

    tmpNum = getPreHighPassFilter();
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
    tmpName.prepend(tmpStr.toLatin1());

    std::memcpy(du_reverb.r_name, tmpName.data(), NAME_CARACT);


    return QByteArray((char *)&(du_reverb), size());
}


int DuReverb::size() const
{
    return FX_REVERB_SIZE;
}


DU_KEY_ACCESSORS_IMPL(DuReverb, Level,             Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuReverb, DirectLevel,       Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuReverb, ReverbSend,        Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuReverb, ToneGain,          Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuReverb, ToneFrequency,     Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuReverb, PreHighPassFilter, Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuReverb, Time,              Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuReverb, EchoFeedback,      Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuReverb, HDAmp,             Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuReverb, ThresholdGate,     Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuReverb, PreDelayTime,      Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuReverb, EffectName,        String, QString, QString())
