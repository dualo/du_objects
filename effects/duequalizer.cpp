#include "duequalizer.h"

DuEqualizer::DuEqualizer() :
    DuEffectSettings()
{
    addChild(KEY_EQ_ONOFF,
             new DuNumeric(FX_PEQ_ONOFF_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_PEQ_ONOFF_MAXVALUE, FX_PEQ_ONOFF_MINVALUE));

    addChild(KEY_EQ_LOWBANDGAIN,
             new DuNumeric(FX_PEQ_LBGAIN_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_PEQ_LBGAIN_MAXVALUE, FX_PEQ_LBGAIN_MINVALUE));

    addChild(KEY_EQ_LOWMIDBANDGAIN,
             new DuNumeric(FX_PEQ_LMBGAIN_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_PEQ_LMBGAIN_MAXVALUE, FX_PEQ_LMBGAIN_MINVALUE));

    addChild(KEY_EQ_HIGHMIDBANDGAIN,
             new DuNumeric(FX_PEQ_HMBGAIN_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_PEQ_HMBGAIN_MAXVALUE, FX_PEQ_HMBGAIN_MINVALUE));

    addChild(KEY_EQ_HIGHBANDGAIN,
             new DuNumeric(FX_PEQ_HBGAIN_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_PEQ_HBGAIN_MAXVALUE, FX_PEQ_HBGAIN_MINVALUE));

    addChild(KEY_EQ_LOWBANDFREQUENCY,
             new DuNumeric(FX_PEQ_LBFREQ_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_PEQ_LBFREQ_MAXVALUE, FX_PEQ_LBFREQ_MINVALUE));

    addChild(KEY_EQ_LOWMIDBANDFREQUENCY,
             new DuNumeric(FX_PEQ_LMBFREQ_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_PEQ_LMBFREQ_MAXVALUE, FX_PEQ_LMBFREQ_MINVALUE));

    addChild(KEY_EQ_HIGHMIDBANDFREQUENCY,
             new DuNumeric(FX_PEQ_HMBFREQ_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_PEQ_HMBFREQ_MAXVALUE, FX_PEQ_HMBFREQ_MINVALUE));

    addChild(KEY_EQ_HIGHBANDFREQUENCY,
             new DuNumeric(FX_PEQ_HBFREQ_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_PEQ_HBFREQ_MAXVALUE, FX_PEQ_HBFREQ_MINVALUE));

    addChild(KEY_EQ_LOWMIDBANDQUALITYFACTOR,
             new DuNumeric(FX_PEQ_LMBQ_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_PEQ_LMBQ_MAXVALUE, FX_PEQ_LMBQ_MINVALUE));

    addChild(KEY_EQ_HIGHMIDBANDQUALITYFACTOR,
             new DuNumeric(FX_PEQ_HMBQ_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_PEQ_HMBQ_MAXVALUE, FX_PEQ_HMBQ_MINVALUE));

    addChild(KEY_EQ_EFFECTNAME,
             new DuString(QString(DEFAULT_EFFECTNAME), NAME_CARACT));
}

DuEqualizer::~DuEqualizer()
{
}


DuEqualizer *DuEqualizer::fromDuMusicFile(const FX_equalizer du_equalizer)
{
    DuEqualizer *equalizer = new DuEqualizer;
    bool verif = true;

    verif = verif && equalizer->setOnOff(du_equalizer.e_on_off);

    verif = verif && equalizer->setLowBandGain(du_equalizer.e_lowbandgain);
    verif = verif && equalizer->setLowMidBandGain(du_equalizer.e_lowmidbandgain);
    verif = verif && equalizer->setHighMidBandGain(du_equalizer.e_highmidbandgain);
    verif = verif && equalizer->setHighBandGain(du_equalizer.e_highbandgain);

    verif = verif && equalizer->setLowBandFrequency(du_equalizer.e_lowbandfrequency);
    verif = verif && equalizer->setLowMidBandFrequency(du_equalizer.e_lowmidbandfrequency);
    verif = verif && equalizer->setHighMidBandFrequency(du_equalizer.e_highmidbandfrequency);
    verif = verif && equalizer->setHighBandFrequency(du_equalizer.e_highbandfrequency);

    verif = verif && equalizer->setLowMidBandQualityFactor(du_equalizer.e_lowmidbandQ);
    verif = verif && equalizer->setHighMidBandQualityFactor(du_equalizer.e_highmidbandQ);

    equalizer->setEffectName(
            QString(QByteArray((char *)du_equalizer.e_name, NAME_CARACT)));

    if (!verif)
    {
        delete equalizer;
        return NULL;
    }

    return equalizer;
}


DuEqualizer *DuEqualizer::fromJson(const QJsonObject &jsonEqualizer)
{
    QJsonValue jsonOnOff        = jsonEqualizer[KEY_EQ_ONOFF];
    QJsonValue jsonLowGain      = jsonEqualizer[KEY_EQ_LOWBANDGAIN];
    QJsonValue jsonLoMidGain    = jsonEqualizer[KEY_EQ_LOWMIDBANDGAIN];
    QJsonValue jsonHiMidGain    = jsonEqualizer[KEY_EQ_HIGHMIDBANDGAIN];
    QJsonValue jsonHighGain     = jsonEqualizer[KEY_EQ_HIGHBANDGAIN];
    QJsonValue jsonLowFreq      = jsonEqualizer[KEY_EQ_LOWBANDFREQUENCY];
    QJsonValue jsonLoMidFreq    = jsonEqualizer[KEY_EQ_LOWMIDBANDFREQUENCY];
    QJsonValue jsonHiMidFreq    = jsonEqualizer[KEY_EQ_HIGHMIDBANDFREQUENCY];
    QJsonValue jsonHighFreq     = jsonEqualizer[KEY_EQ_HIGHBANDFREQUENCY];
    QJsonValue jsonLoMidQual    = jsonEqualizer[KEY_EQ_LOWMIDBANDQUALITYFACTOR];
    QJsonValue jsonHiMidQual    = jsonEqualizer[KEY_EQ_HIGHMIDBANDQUALITYFACTOR];
    QJsonValue jsonEffectName   = jsonEqualizer[KEY_EQ_EFFECTNAME];

    if (        !jsonOnOff.isDouble()       ||  !jsonLowGain.isDouble()
            ||  !jsonLoMidGain.isDouble()   ||  !jsonHiMidGain.isDouble()
            ||  !jsonHighGain.isDouble()    ||  !jsonLowFreq.isDouble()
            ||  !jsonLoMidFreq.isDouble()   ||  !jsonHiMidFreq.isDouble()
            ||  !jsonHighFreq.isDouble()    ||  !jsonLoMidQual.isDouble()
            ||  !jsonHiMidQual.isDouble()   ||  !jsonEffectName.isString())

        return NULL;


    DuEqualizer *equalizer = new DuEqualizer;
    bool verif = true;

    verif = verif && equalizer->setOnOff(jsonOnOff.toInt());

    verif = verif && equalizer->setLowBandGain(jsonLowGain.toInt());
    verif = verif && equalizer->setLowMidBandGain(jsonLoMidGain.toInt());
    verif = verif && equalizer->setHighMidBandGain(jsonHiMidGain.toInt());
    verif = verif && equalizer->setHighBandGain(jsonHighGain.toInt());

    verif = verif && equalizer->setLowBandFrequency(jsonLowFreq.toInt());
    verif = verif && equalizer->setLowMidBandFrequency(jsonLoMidFreq.toInt());
    verif = verif && equalizer->setHighMidBandFrequency(jsonHiMidFreq.toInt());
    verif = verif && equalizer->setHighBandFrequency(jsonHighFreq.toInt());

    verif = verif && equalizer->setLowMidBandQualityFactor(jsonLoMidQual.toInt());
    verif = verif && equalizer->setHighMidBandQualityFactor(jsonHiMidQual.toInt());

    verif = verif && equalizer->setEffectName(jsonEffectName.toString());

    if (!verif)
    {
        delete equalizer;
        return NULL;
    }

    return equalizer;
}


QByteArray DuEqualizer::toDuMusicFile() const
{
    FX_equalizer du_equalizer;

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


int DuEqualizer::getOnOff() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_ONOFF));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEqualizer::setOnOff(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_ONOFF));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuEqualizer::getLowBandGain() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_LOWBANDGAIN));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEqualizer::setLowBandGain(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_LOWBANDGAIN));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuEqualizer::getLowMidBandGain() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_LOWMIDBANDGAIN));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEqualizer::setLowMidBandGain(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_LOWMIDBANDGAIN));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuEqualizer::getHighMidBandGain() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_HIGHMIDBANDGAIN));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEqualizer::setHighMidBandGain(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_HIGHMIDBANDGAIN));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuEqualizer::getHighBandGain() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_HIGHBANDGAIN));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEqualizer::setHighBandGain(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_HIGHBANDGAIN));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuEqualizer::getLowBandFrequency() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_LOWBANDFREQUENCY));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEqualizer::setLowBandFrequency(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_LOWBANDFREQUENCY));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuEqualizer::getLowMidBandFrequency() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_LOWMIDBANDFREQUENCY));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEqualizer::setLowMidBandFrequency(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_LOWMIDBANDFREQUENCY));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuEqualizer::getHighMidBandFrequency() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_HIGHMIDBANDFREQUENCY));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEqualizer::setHighMidBandFrequency(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_HIGHMIDBANDFREQUENCY));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuEqualizer::getHighBandFrequency() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_HIGHBANDFREQUENCY));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEqualizer::setHighBandFrequency(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_HIGHBANDFREQUENCY));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuEqualizer::getLowMidBandQualityFactor() const
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_EQ_LOWMIDBANDQUALITYFACTOR));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEqualizer::setLowMidBandQualityFactor(int value)
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_EQ_LOWMIDBANDQUALITYFACTOR));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuEqualizer::getHighMidBandQualityFactor() const
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_EQ_HIGHMIDBANDQUALITYFACTOR));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEqualizer::setHighMidBandQualityFactor(int value)
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_EQ_HIGHMIDBANDQUALITYFACTOR));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


QString DuEqualizer::getEffectName() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_EQ_EFFECTNAME));

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuEqualizer::setEffectName(const QString &value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_EQ_EFFECTNAME));

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}
