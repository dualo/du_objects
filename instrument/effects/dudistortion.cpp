#include "dudistortion.h"

#include <cstring>

#include <QJsonObject>

#include "../../general/dunumeric.h"
#include "../../general/dustring.h"

DU_OBJECT_IMPL(DuDistortion)

DuDistortion::DuDistortion() :
    DuEffectSettings()
{
    addChild(KeyOnOff,
             new DuNumeric(FX_DISTO_ONOFF_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DISTO_ONOFF_MAXVALUE, FX_DISTO_ONOFF_MINVALUE));

    addChild(KeyPreGain,
             new DuNumeric(FX_DISTO_PREGAIN_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DISTO_PREGAIN_MAXVALUE, FX_DISTO_PREGAIN_MINVALUE));

    addChild(KeyEffectType,
             new DuNumeric(FX_DISTO_TYPE_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DISTO_TYPE_MAXVALUE, FX_DISTO_TYPE_MINVALUE));

    addChild(KeyLowPassFilterFrequency,
             new DuNumeric(FX_DISTO_LPFREQ_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DISTO_LPFREQ_MAXVALUE, FX_DISTO_LPFREQ_MINVALUE));

    addChild(KeyLowPassFilterResonance,
             new DuNumeric(FX_DISTO_LPRES_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DISTO_LPRES_MAXVALUE, FX_DISTO_LPRES_MINVALUE));

    addChild(KeyPostGain,
             new DuNumeric(FX_DISTO_POSTGAIN_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DISTO_POSTGAIN_MAXVALUE, FX_DISTO_POSTGAIN_MINVALUE));

    addChild(KeyDrive,
             new DuNumeric(FX_DISTO_DRIVE_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DISTO_DRIVE_MAXVALUE, FX_DISTO_DRIVE_MINVALUE));
}

DuObjectPtr DuDistortion::clone() const
{
    return DuDistortionPtr(new DuDistortion(*this));
}


DuDistortionPtr DuDistortion::fromDuMusicBinary(const FX_distortion &du_distortion)
{
    DuDistortionPtr distortion(new DuDistortion);
    bool verif = true;

    verif = distortion->setOnOff(du_distortion.d_on_off) ? verif : false;

    verif = distortion->setPreGain(du_distortion.d_pre_gain) ? verif : false;
    verif = distortion->setEffectType(du_distortion.d_type) ? verif : false;

    verif = distortion->setLowPassFilterFrequency(du_distortion.d_lowpassfilterfreq) ? verif : false;
    verif = distortion->setLowPassFilterResonance(du_distortion.d_lowpassfilterres) ? verif : false;

    verif = distortion->setPostGain(du_distortion.d_postgain) ? verif : false;
    verif = distortion->setDrive(du_distortion.d_drive) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuDistortion::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    return distortion;
}


QByteArray DuDistortion::toDuMusicBinary() const
{
    FX_distortion du_distortion;
    std::memset(&du_distortion, 0, static_cast<size_t>(size()));

    int tmpNum = 0;


    tmpNum = getOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_distortion.d_on_off = static_cast<quint8>(tmpNum);

    tmpNum = getPreGain();
    if (tmpNum == -1)
        return QByteArray();
    du_distortion.d_pre_gain = static_cast<quint8>(tmpNum);

    tmpNum = getEffectType();
    if (tmpNum == -1)
        return QByteArray();
    du_distortion.d_type = static_cast<quint8>(tmpNum);

    tmpNum = getLowPassFilterFrequency();
    if (tmpNum == -1)
        return QByteArray();
    du_distortion.d_lowpassfilterfreq = static_cast<quint8>(tmpNum);

    tmpNum = getLowPassFilterResonance();
    if (tmpNum == -1)
        return QByteArray();
    du_distortion.d_lowpassfilterres = static_cast<quint8>(tmpNum);

    tmpNum = getPostGain();
    if (tmpNum == -1)
        return QByteArray();
    du_distortion.d_postgain = static_cast<quint8>(tmpNum);

    tmpNum = getDrive();
    if (tmpNum == -1)
        return QByteArray();
    du_distortion.d_drive = static_cast<quint8>(tmpNum);


    return QByteArray(reinterpret_cast<char*>(&du_distortion), size());
}


int DuDistortion::size() const
{
    return FX_DIST_SIZE;
}


DU_KEY_ACCESSORS_IMPL(DuDistortion, OnOff,                  Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuDistortion, PreGain,                Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuDistortion, EffectType,             Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuDistortion, LowPassFilterFrequency, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuDistortion, LowPassFilterResonance, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuDistortion, PostGain,               Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuDistortion, Drive,                  Numeric, int, -1)
