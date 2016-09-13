#include "duequalizer.h"

#include <cstring>

#include <QJsonObject>

#include "../../general/dunumeric.h"
#include "../../general/dustring.h"

DU_OBJECT_IMPL(DuEqualizer)

DuEqualizer::DuEqualizer() :
    DuEffectSettings()
{
    addChild(KeyOnOff,
             new DuNumeric(FX_PEQ_ONOFF_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_PEQ_ONOFF_MAXVALUE, FX_PEQ_ONOFF_MINVALUE));

    addChild(KeyLowBandGain,
             new DuNumeric(FX_PEQ_LBGAIN_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_PEQ_LBGAIN_MAXVALUE, FX_PEQ_LBGAIN_MINVALUE));

    addChild(KeyLowMidBandGain,
             new DuNumeric(FX_PEQ_LMBGAIN_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_PEQ_LMBGAIN_MAXVALUE, FX_PEQ_LMBGAIN_MINVALUE));

    addChild(KeyHighMidBandGain,
             new DuNumeric(FX_PEQ_HMBGAIN_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_PEQ_HMBGAIN_MAXVALUE, FX_PEQ_HMBGAIN_MINVALUE));

    addChild(KeyHighBandGain,
             new DuNumeric(FX_PEQ_HBGAIN_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_PEQ_HBGAIN_MAXVALUE, FX_PEQ_HBGAIN_MINVALUE));

    addChild(KeyLowBandFrequency,
             new DuNumeric(FX_PEQ_LBFREQ_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_PEQ_LBFREQ_MAXVALUE, FX_PEQ_LBFREQ_MINVALUE));

    addChild(KeyLowMidBandFrequency,
             new DuNumeric(FX_PEQ_LMBFREQ_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_PEQ_LMBFREQ_MAXVALUE, FX_PEQ_LMBFREQ_MINVALUE));

    addChild(KeyHighMidBandFrequency,
             new DuNumeric(FX_PEQ_HMBFREQ_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_PEQ_HMBFREQ_MAXVALUE, FX_PEQ_HMBFREQ_MINVALUE));

    addChild(KeyHighBandFrequency,
             new DuNumeric(FX_PEQ_HBFREQ_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_PEQ_HBFREQ_MAXVALUE, FX_PEQ_HBFREQ_MINVALUE));

    addChild(KeyLowMidBandQualityFactor,
             new DuNumeric(FX_PEQ_LMBQ_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_PEQ_LMBQ_MAXVALUE, FX_PEQ_LMBQ_MINVALUE));

    addChild(KeyHighMidBandQualityFactor,
             new DuNumeric(FX_PEQ_HMBQ_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_PEQ_HMBQ_MAXVALUE, FX_PEQ_HMBQ_MINVALUE));
}

DuObjectPtr DuEqualizer::clone() const
{
    return DuEqualizerPtr(new DuEqualizer(*this));
}


DuEqualizerPtr DuEqualizer::fromDuMusicBinary(const FX_equalizer& du_equalizer)
{
    DuEqualizerPtr equalizer(new DuEqualizer);
    bool verif = true;

    verif = equalizer->setOnOff(du_equalizer.e_on_off) ? verif : false;

    verif = equalizer->setLowBandGain(du_equalizer.e_lowbandgain) ? verif : false;
    verif = equalizer->setLowMidBandGain(du_equalizer.e_lowmidbandgain) ? verif : false;
    verif = equalizer->setHighMidBandGain(du_equalizer.e_highmidbandgain) ? verif : false;
    verif = equalizer->setHighBandGain(du_equalizer.e_highbandgain) ? verif : false;

    verif = equalizer->setLowBandFrequency(du_equalizer.e_lowbandfrequency) ? verif : false;
    verif = equalizer->setLowMidBandFrequency(du_equalizer.e_lowmidbandfrequency) ? verif : false;
    verif = equalizer->setHighMidBandFrequency(du_equalizer.e_highmidbandfrequency) ? verif : false;
    verif = equalizer->setHighBandFrequency(du_equalizer.e_highbandfrequency) ? verif : false;

    verif = equalizer->setLowMidBandQualityFactor(du_equalizer.e_lowmidbandQ) ? verif : false;
    verif = equalizer->setHighMidBandQualityFactor(du_equalizer.e_highmidbandQ) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuEqualizer::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    return equalizer;
}


QByteArray DuEqualizer::toDuMusicBinary() const
{
    FX_equalizer du_equalizer;
    std::memset(&du_equalizer, 0, static_cast<size_t>(size()));

    int tmpNum = 0;


    tmpNum = getOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_equalizer.e_on_off = static_cast<quint8>(tmpNum);

    tmpNum = getLowBandGain();
    if (tmpNum == -1)
        return QByteArray();
    du_equalizer.e_lowbandgain = static_cast<quint8>(tmpNum);

    tmpNum = getLowMidBandGain();
    if (tmpNum == -1)
        return QByteArray();
    du_equalizer.e_lowmidbandgain = static_cast<quint8>(tmpNum);

    tmpNum = getHighMidBandGain();
    if (tmpNum == -1)
        return QByteArray();
    du_equalizer.e_highmidbandgain = static_cast<quint8>(tmpNum);

    tmpNum = getHighBandGain();
    if (tmpNum == -1)
        return QByteArray();
    du_equalizer.e_highbandgain = static_cast<quint8>(tmpNum);

    tmpNum = getLowBandFrequency();
    if (tmpNum == -1)
        return QByteArray();
    du_equalizer.e_lowbandfrequency = static_cast<quint8>(tmpNum);

    tmpNum = getLowMidBandFrequency();
    if (tmpNum == -1)
        return QByteArray();
    du_equalizer.e_lowmidbandfrequency = static_cast<quint8>(tmpNum);

    tmpNum = getHighMidBandFrequency();
    if (tmpNum == -1)
        return QByteArray();
    du_equalizer.e_highmidbandfrequency = static_cast<quint8>(tmpNum);

    tmpNum = getHighBandFrequency();
    if (tmpNum == -1)
        return QByteArray();
    du_equalizer.e_highbandfrequency = static_cast<quint8>(tmpNum);

    tmpNum = getLowMidBandQualityFactor();
    if (tmpNum == -1)
        return QByteArray();
    du_equalizer.e_lowmidbandQ = static_cast<quint8>(tmpNum);

    tmpNum = getHighMidBandQualityFactor();
    if (tmpNum == -1)
        return QByteArray();
    du_equalizer.e_highmidbandQ = static_cast<quint8>(tmpNum);


    return QByteArray(reinterpret_cast<char*>(&du_equalizer), size());
}


int DuEqualizer::size() const
{
    return FX_EQ_SIZE;
}


DU_KEY_ACCESSORS_IMPL(DuEqualizer, OnOff,                    Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuEqualizer, LowBandGain,              Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuEqualizer, LowMidBandGain,           Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuEqualizer, HighMidBandGain,          Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuEqualizer, HighBandGain,             Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuEqualizer, LowBandFrequency,         Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuEqualizer, LowMidBandFrequency,      Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuEqualizer, HighMidBandFrequency,     Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuEqualizer, HighBandFrequency,        Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuEqualizer, LowMidBandQualityFactor,  Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuEqualizer, HighMidBandQualityFactor, Numeric, int, -1)
