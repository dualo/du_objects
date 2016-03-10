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

    addChild(KeyEffectName,
             new DuString(QStringLiteral(DEFAULT_EFFECTNAME), NAME_CARACT));
}

DuEqualizer::~DuEqualizer()
{
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

    equalizer->setEffectName(
            QString(QByteArray((char *)du_equalizer.e_name, NAME_CARACT)));

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuEqualizer::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    return equalizer;
}


DuEqualizerPtr DuEqualizer::fromJson(const QJsonObject &jsonEqualizer)
{
    QJsonValue jsonOnOff        = jsonEqualizer[KeyOnOff];
    QJsonValue jsonLowGain      = jsonEqualizer[KeyLowBandGain];
    QJsonValue jsonLoMidGain    = jsonEqualizer[KeyLowMidBandGain];
    QJsonValue jsonHiMidGain    = jsonEqualizer[KeyHighMidBandGain];
    QJsonValue jsonHighGain     = jsonEqualizer[KeyHighBandGain];
    QJsonValue jsonLowFreq      = jsonEqualizer[KeyLowBandFrequency];
    QJsonValue jsonLoMidFreq    = jsonEqualizer[KeyLowMidBandFrequency];
    QJsonValue jsonHiMidFreq    = jsonEqualizer[KeyHighMidBandFrequency];
    QJsonValue jsonHighFreq     = jsonEqualizer[KeyHighBandFrequency];
    QJsonValue jsonLoMidQual    = jsonEqualizer[KeyLowMidBandQualityFactor];
    QJsonValue jsonHiMidQual    = jsonEqualizer[KeyHighMidBandQualityFactor];
    QJsonValue jsonEffectName   = jsonEqualizer[KeyEffectName];

    if (        !jsonOnOff.isDouble()       ||  !jsonLowGain.isDouble()
            ||  !jsonLoMidGain.isDouble()   ||  !jsonHiMidGain.isDouble()
            ||  !jsonHighGain.isDouble()    ||  !jsonLowFreq.isDouble()
            ||  !jsonLoMidFreq.isDouble()   ||  !jsonHiMidFreq.isDouble()
            ||  !jsonHighFreq.isDouble()    ||  !jsonLoMidQual.isDouble()
            ||  !jsonHiMidQual.isDouble()   ||  !jsonEffectName.isString())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuEqualizer::fromJson():\n"
                    << "failed to generate DuEqualizer\n"
                    << "a json key did not contain the proper type";

        return DuEqualizerPtr();
    }


    DuEqualizerPtr equalizer(new DuEqualizer);
    bool verif = true;

    verif = equalizer->setOnOff(jsonOnOff.toInt()) ? verif : false;

    verif = equalizer->setLowBandGain(jsonLowGain.toInt()) ? verif : false;
    verif = equalizer->setLowMidBandGain(jsonLoMidGain.toInt()) ? verif : false;
    verif = equalizer->setHighMidBandGain(jsonHiMidGain.toInt()) ? verif : false;
    verif = equalizer->setHighBandGain(jsonHighGain.toInt()) ? verif : false;

    verif = equalizer->setLowBandFrequency(jsonLowFreq.toInt()) ? verif : false;
    verif = equalizer->setLowMidBandFrequency(jsonLoMidFreq.toInt()) ? verif : false;
    verif = equalizer->setHighMidBandFrequency(jsonHiMidFreq.toInt()) ? verif : false;
    verif = equalizer->setHighBandFrequency(jsonHighFreq.toInt()) ? verif : false;

    verif = equalizer->setLowMidBandQualityFactor(jsonLoMidQual.toInt()) ? verif : false;
    verif = equalizer->setHighMidBandQualityFactor(jsonHiMidQual.toInt()) ? verif : false;

    verif = equalizer->setEffectName(jsonEffectName.toString()) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuEqualizer::fromJson():\n"
                   << "an attribute was not properly set";
    }

    return equalizer;
}


QByteArray DuEqualizer::toDuMusicBinary() const
{
    FX_equalizer du_equalizer;
    std::memset((char*)&du_equalizer, 0, size());

    QString tmpStr;
    int tmpNum = 0;


    tmpNum = getOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_equalizer.e_on_off = tmpNum;

    tmpNum = getLowBandGain();
    if (tmpNum == -1)
        return QByteArray();
    du_equalizer.e_lowbandgain = tmpNum;

    tmpNum = getLowMidBandGain();
    if (tmpNum == -1)
        return QByteArray();
    du_equalizer.e_lowmidbandgain = tmpNum;

    tmpNum = getHighMidBandGain();
    if (tmpNum == -1)
        return QByteArray();
    du_equalizer.e_highmidbandgain = tmpNum;

    tmpNum = getHighBandGain();
    if (tmpNum == -1)
        return QByteArray();
    du_equalizer.e_highbandgain = tmpNum;

    tmpNum = getLowBandFrequency();
    if (tmpNum == -1)
        return QByteArray();
    du_equalizer.e_lowbandfrequency = tmpNum;

    tmpNum = getLowMidBandFrequency();
    if (tmpNum == -1)
        return QByteArray();
    du_equalizer.e_lowmidbandfrequency = tmpNum;

    tmpNum = getHighMidBandFrequency();
    if (tmpNum == -1)
        return QByteArray();
    du_equalizer.e_highmidbandfrequency = tmpNum;

    tmpNum = getHighBandFrequency();
    if (tmpNum == -1)
        return QByteArray();
    du_equalizer.e_highbandfrequency = tmpNum;

    tmpNum = getLowMidBandQualityFactor();
    if (tmpNum == -1)
        return QByteArray();
    du_equalizer.e_lowmidbandQ = tmpNum;

    tmpNum = getHighMidBandQualityFactor();
    if (tmpNum == -1)
        return QByteArray();
    du_equalizer.e_highmidbandQ = tmpNum;


    QByteArray tmpName(NAME_CARACT, (char)0x00);
    tmpStr = getEffectName();
    if (tmpStr.isNull())
        return QByteArray();
    tmpName.prepend(tmpStr.toUtf8());

    std::memcpy(du_equalizer.e_name, tmpName.data(), NAME_CARACT);


    return QByteArray((char *)&(du_equalizer), size());
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
DU_KEY_ACCESSORS_IMPL(DuEqualizer, EffectName,               String, QString, QString())
