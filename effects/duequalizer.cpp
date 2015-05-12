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
    DuEqualizer *equalizer = new DuEqualizer();

    equalizer->setOnOff(du_equalizer.e_on_off);

    equalizer->setLowBandGain(du_equalizer.e_lowbandgain);
    equalizer->setLowMidBandGain(du_equalizer.e_lowmidbandgain);
    equalizer->setHighMidBandGain(du_equalizer.e_highmidbandgain);
    equalizer->setHighBandGain(du_equalizer.e_highbandgain);

    equalizer->setLowBandFrequency(du_equalizer.e_lowbandfrequency);
    equalizer->setLowMidBandFrequency(du_equalizer.e_lowmidbandfrequency);
    equalizer->setHighMidBandFrequency(du_equalizer.e_highmidbandfrequency);
    equalizer->setHighBandFrequency(du_equalizer.e_highbandfrequency);

    equalizer->setLowMidBandQualityFactor(du_equalizer.e_lowmidbandQ);
    equalizer->setHighMidBandQualityFactor(du_equalizer.e_highmidbandQ);

    equalizer->setEffectName(
            QString(QByteArray((char *)du_equalizer.e_name, NAME_CARACT)));

    return equalizer;
}


DuEqualizer *DuEqualizer::fromJson(const QJsonObject &jsonEqualizer)
{
    DuEqualizer *equalizer = new DuEqualizer();
    const QStringList &keyList = equalizer->keys();

    bool test = true;
    for (int i = 0; i < keyList.count(); i++)
    {
        test = test && jsonEqualizer.contains(keyList[i]);
    }

    if (!test)
        return NULL;

    equalizer->setOnOff(jsonEqualizer[KEY_EQ_ONOFF].toInt());

    equalizer->setLowBandGain(jsonEqualizer[KEY_EQ_LOWBANDGAIN].toInt());
    equalizer->setLowMidBandGain(jsonEqualizer[KEY_EQ_LOWMIDBANDGAIN].toInt());
    equalizer->setHighMidBandGain(jsonEqualizer[KEY_EQ_HIGHMIDBANDGAIN].toInt());
    equalizer->setHighBandGain(jsonEqualizer[KEY_EQ_HIGHBANDGAIN].toInt());

    equalizer->setLowBandFrequency(jsonEqualizer[KEY_EQ_LOWBANDFREQUENCY].toInt());
    equalizer->setLowMidBandFrequency(
                jsonEqualizer[KEY_EQ_LOWMIDBANDFREQUENCY].toInt());
    equalizer->setHighMidBandFrequency(
                jsonEqualizer[KEY_EQ_HIGHMIDBANDFREQUENCY].toInt());
    equalizer->setHighBandFrequency(jsonEqualizer[KEY_EQ_HIGHBANDFREQUENCY].toInt());

    equalizer->setLowMidBandQualityFactor(
                jsonEqualizer[KEY_EQ_LOWMIDBANDQUALITYFACTOR].toInt());
    equalizer->setHighMidBandQualityFactor(
                jsonEqualizer[KEY_EQ_HIGHMIDBANDQUALITYFACTOR].toInt());

    equalizer->setEffectName(jsonEqualizer[KEY_EQ_EFFECTNAME].toString());

    return equalizer;
}


int DuEqualizer::getOnOff() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_ONOFF));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuEqualizer::setOnOff(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_ONOFF));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


int DuEqualizer::getLowBandGain() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_LOWBANDGAIN));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuEqualizer::setLowBandGain(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_LOWBANDGAIN));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuEqualizer::getLowMidBandGain() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_LOWMIDBANDGAIN));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuEqualizer::setLowMidBandGain(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_LOWMIDBANDGAIN));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuEqualizer::getHighMidBandGain() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_HIGHMIDBANDGAIN));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuEqualizer::setHighMidBandGain(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_HIGHMIDBANDGAIN));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuEqualizer::getHighBandGain() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_HIGHBANDGAIN));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuEqualizer::setHighBandGain(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_HIGHBANDGAIN));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


int DuEqualizer::getLowBandFrequency() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_LOWBANDFREQUENCY));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuEqualizer::setLowBandFrequency(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_LOWBANDFREQUENCY));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuEqualizer::getLowMidBandFrequency() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_LOWMIDBANDFREQUENCY));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuEqualizer::setLowMidBandFrequency(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_LOWMIDBANDFREQUENCY));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuEqualizer::getHighMidBandFrequency() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_HIGHMIDBANDFREQUENCY));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuEqualizer::setHighMidBandFrequency(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_HIGHMIDBANDFREQUENCY));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuEqualizer::getHighBandFrequency() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_HIGHBANDFREQUENCY));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuEqualizer::setHighBandFrequency(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_EQ_HIGHBANDFREQUENCY));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


int DuEqualizer::getLowMidBandQualityFactor() const
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_EQ_LOWMIDBANDQUALITYFACTOR));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuEqualizer::setLowMidBandQualityFactor(int value)
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_EQ_LOWMIDBANDQUALITYFACTOR));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuEqualizer::getHighMidBandQualityFactor() const
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_EQ_HIGHMIDBANDQUALITYFACTOR));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuEqualizer::setHighMidBandQualityFactor(int value)
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_EQ_HIGHMIDBANDQUALITYFACTOR));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


QString DuEqualizer::getEffectName() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_EQ_EFFECTNAME));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

void DuEqualizer::setEffectName(const QString &value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_EQ_EFFECTNAME));

    if (tmp == NULL)
        return;

    tmp->setString(value);
}
