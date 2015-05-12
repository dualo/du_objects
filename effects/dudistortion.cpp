#include "dudistortion.h"

DuDistortion::DuDistortion() :
    DuEffectSettings()
{
    addChild(KEY_DIST_ONOFF,
             new DuNumeric(FX_DISTO_ONOFF_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DISTO_ONOFF_MAXVALUE, FX_DISTO_ONOFF_MINVALUE));

    addChild(KEY_DIST_PREGAIN,
             new DuNumeric(FX_DISTO_PREGAIN_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DISTO_PREGAIN_MAXVALUE, FX_DISTO_PREGAIN_MINVALUE));

    addChild(KEY_DIST_TYPE,
             new DuNumeric(FX_DISTO_TYPE_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DISTO_TYPE_MAXVALUE, FX_DISTO_TYPE_MINVALUE));

    addChild(KEY_DIST_LOWPASSFILTERFREQUENCY,
             new DuNumeric(FX_DISTO_LPFREQ_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DISTO_LPFREQ_MAXVALUE, FX_DISTO_LPFREQ_MINVALUE));

    addChild(KEY_DIST_LOWPASSFILTERRESONANCE,
             new DuNumeric(FX_DISTO_LPRES_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DISTO_LPRES_MAXVALUE, FX_DISTO_LPRES_MINVALUE));

    addChild(KEY_DIST_POSTGAIN,
             new DuNumeric(FX_DISTO_POSTGAIN_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DISTO_POSTGAIN_MAXVALUE, FX_DISTO_POSTGAIN_MINVALUE));

    addChild(KEY_DIST_DRIVE,
             new DuNumeric(FX_DISTO_DRIVE_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_DISTO_DRIVE_MAXVALUE, FX_DISTO_DRIVE_MINVALUE));

    addChild(KEY_DIST_EFFECTNAME,
             new DuString(QString(DEFAULT_EFFECTNAME), NAME_CARACT));
}

DuDistortion::~DuDistortion()
{
}


DuDistortion *DuDistortion::fromDuMusicFile(const FX_distortion &du_distortion)
{
    DuDistortion *distortion = new DuDistortion();

    distortion->setOnOff(du_distortion.d_on_off);

    distortion->setPreGain(du_distortion.d_pre_gain);
    distortion->setEffectType(du_distortion.d_type);

    distortion->setLowPassFilterFrequency(du_distortion.d_lowpassfilterfreq);
    distortion->setLowPassFilterResonance(du_distortion.d_lowpassfilterres);

    distortion->setPostGain(du_distortion.d_postgain);
    distortion->setDrive(du_distortion.d_drive);

    distortion->setEffectName(
            QString(QByteArray((char *)du_distortion.d_name, NAME_CARACT)));

    return distortion;
}


DuDistortion *DuDistortion::fromJson(const QJsonObject &jsonDistortion)
{
    DuDistortion *distortion = new DuDistortion();
    const QStringList &keyList = distortion->keys();

    bool test = true;
    for (int i = 0; i < keyList.count(); i++)
    {
        test = test && jsonDistortion.contains(keyList[i]);
    }

    if (!test)
        return NULL;

    distortion->setOnOff(jsonDistortion[KEY_DIST_ONOFF].toInt());

    distortion->setPreGain(jsonDistortion[KEY_DIST_PREGAIN].toInt());
    distortion->setEffectType(jsonDistortion[KEY_DIST_TYPE].toInt());

    distortion->setLowPassFilterFrequency(
                jsonDistortion[KEY_DIST_LOWPASSFILTERFREQUENCY].toInt());
    distortion->setLowPassFilterResonance(
                jsonDistortion[KEY_DIST_LOWPASSFILTERRESONANCE].toInt());

    distortion->setPostGain(jsonDistortion[KEY_DIST_POSTGAIN].toInt());
    distortion->setDrive(jsonDistortion[KEY_DIST_DRIVE].toInt());

    distortion->setEffectName(jsonDistortion[KEY_DIST_EFFECTNAME].toString());

    return distortion;
}


int DuDistortion::getOnOff() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DIST_ONOFF));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuDistortion::setOnOff(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DIST_ONOFF));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


int DuDistortion::getPreGain() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DIST_PREGAIN));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuDistortion::setPreGain(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DIST_PREGAIN));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuDistortion::getEffectType() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DIST_TYPE));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuDistortion::setEffectType(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DIST_TYPE));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


int DuDistortion::getLowPassFilterFrequency() const
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_DIST_LOWPASSFILTERFREQUENCY));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuDistortion::setLowPassFilterFrequency(int value)
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_DIST_LOWPASSFILTERFREQUENCY));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuDistortion::getLowPassFilterResonance() const
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_DIST_LOWPASSFILTERRESONANCE));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuDistortion::setLowPassFilterResonance(int value)
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_DIST_LOWPASSFILTERRESONANCE));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


int DuDistortion::getPostGain() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DIST_POSTGAIN));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuDistortion::setPostGain(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DIST_POSTGAIN));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuDistortion::getDrive() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DIST_DRIVE));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuDistortion::setDrive(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DIST_DRIVE));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


QString DuDistortion::getEffectName() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_DIST_EFFECTNAME));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

void DuDistortion::setEffectName(const QString &value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_DIST_EFFECTNAME));

    if (tmp == NULL)
        return;

    tmp->setString(value);
}
