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
    DuDistortion *distortion = new DuDistortion;
    bool verif = true;

    verif = verif && distortion->setOnOff(du_distortion.d_on_off);

    verif = verif && distortion->setPreGain(du_distortion.d_pre_gain);
    verif = verif && distortion->setEffectType(du_distortion.d_type);

    verif = verif && distortion->setLowPassFilterFrequency(
                du_distortion.d_lowpassfilterfreq);
    verif = verif && distortion->setLowPassFilterResonance(
                du_distortion.d_lowpassfilterres);

    verif = verif && distortion->setPostGain(du_distortion.d_postgain);
    verif = verif && distortion->setDrive(du_distortion.d_drive);

    verif = verif && distortion->setEffectName(
            QString(QByteArray((char *)du_distortion.d_name, NAME_CARACT)));

    if (!verif)
    {
        delete distortion;
        return NULL;
    }

    return distortion;
}


DuDistortion *DuDistortion::fromJson(const QJsonObject &jsonDistortion)
{
    QJsonValue jsonOnOff        = jsonDistortion[KEY_DIST_ONOFF];
    QJsonValue jsonPreGain      = jsonDistortion[KEY_DIST_PREGAIN];
    QJsonValue jsonEffectType   = jsonDistortion[KEY_DIST_TYPE];
    QJsonValue jsonLoPassFreq   = jsonDistortion[KEY_DIST_LOWPASSFILTERFREQUENCY];
    QJsonValue jsonLoPassRes    = jsonDistortion[KEY_DIST_LOWPASSFILTERRESONANCE];
    QJsonValue jsonPostGain     = jsonDistortion[KEY_DIST_POSTGAIN];
    QJsonValue jsonDrive        = jsonDistortion[KEY_DIST_DRIVE];
    QJsonValue jsonEffectName   = jsonDistortion[KEY_DIST_EFFECTNAME];

    if (        !jsonOnOff.isDouble()       ||  !jsonPreGain.isDouble()
            ||  !jsonEffectType.isDouble()  ||  !jsonLoPassFreq.isDouble()
            ||  !jsonLoPassRes.isDouble()   ||  !jsonPostGain.isDouble()
            ||  !jsonDrive.isDouble()       ||  !jsonEffectName.isString())

        return NULL;


    DuDistortion *distortion = new DuDistortion;
    bool verif = true;

    verif = verif && distortion->setOnOff(jsonOnOff.toInt());

    verif = verif && distortion->setPreGain(jsonPreGain.toInt());
    verif = verif && distortion->setEffectType(jsonEffectType.toInt());

    verif = verif && distortion->setLowPassFilterFrequency(jsonLoPassFreq.toInt());
    verif = verif && distortion->setLowPassFilterResonance(jsonLoPassRes.toInt());

    verif = verif && distortion->setPostGain(jsonPostGain.toInt());
    verif = verif && distortion->setDrive(jsonDrive.toInt());

    verif = verif && distortion->setEffectName(jsonEffectName.toString());

    if (!verif)
    {
        delete distortion;
        return NULL;
    }

    return distortion;
}


int DuDistortion::size() const
{
    return FX_DIST_SIZE;
}


int DuDistortion::getOnOff() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DIST_ONOFF));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDistortion::setOnOff(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DIST_ONOFF));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuDistortion::getPreGain() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DIST_PREGAIN));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDistortion::setPreGain(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DIST_PREGAIN));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuDistortion::getEffectType() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DIST_TYPE));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDistortion::setEffectType(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DIST_TYPE));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuDistortion::getLowPassFilterFrequency() const
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_DIST_LOWPASSFILTERFREQUENCY));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDistortion::setLowPassFilterFrequency(int value)
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_DIST_LOWPASSFILTERFREQUENCY));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuDistortion::getLowPassFilterResonance() const
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_DIST_LOWPASSFILTERRESONANCE));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDistortion::setLowPassFilterResonance(int value)
{
    DuNumeric *tmp =
            dynamic_cast<DuNumeric *>(getChild(KEY_DIST_LOWPASSFILTERRESONANCE));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuDistortion::getPostGain() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DIST_POSTGAIN));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDistortion::setPostGain(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DIST_POSTGAIN));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuDistortion::getDrive() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DIST_DRIVE));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDistortion::setDrive(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_DIST_DRIVE));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


QString DuDistortion::getEffectName() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_DIST_EFFECTNAME));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

bool DuDistortion::setEffectName(const QString &value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_DIST_EFFECTNAME));

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}
