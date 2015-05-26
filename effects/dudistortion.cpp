#include "dudistortion.h"

#include <QJsonObject>
#include <QDebug>

DU_OBJECT_IMPL(DuDistortion)

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


DuDistortionPtr DuDistortion::fromDuMusicFile(const FX_distortion &du_distortion)
{
    DuDistortionPtr distortion(new DuDistortion);
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
        return DuDistortionPtr();
    }

    return distortion;
}


DuDistortionPtr DuDistortion::fromJson(const QJsonObject &jsonDistortion)
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

        return DuDistortionPtr();


    DuDistortionPtr distortion(new DuDistortion);
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
        return DuDistortionPtr();
    }

    return distortion;
}


QByteArray DuDistortion::toDuMusicFile() const
{
    FX_distortion du_distortion;

    QString tmpStr;
    int tmpNum = 0;


    tmpNum = getOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_distortion.d_on_off = tmpNum;

    tmpNum = getPreGain();
    if (tmpNum == -1)
        return QByteArray();
    du_distortion.d_pre_gain = tmpNum;

    tmpNum = getEffectType();
    if (tmpNum == -1)
        return QByteArray();
    du_distortion.d_type = tmpNum;

    tmpNum = getLowPassFilterFrequency();
    if (tmpNum == -1)
        return QByteArray();
    du_distortion.d_lowpassfilterfreq = tmpNum;

    tmpNum = getLowPassFilterResonance();
    if (tmpNum == -1)
        return QByteArray();
    du_distortion.d_lowpassfilterres = tmpNum;

    tmpNum = getPostGain();
    if (tmpNum == -1)
        return QByteArray();
    du_distortion.d_postgain = tmpNum;

    tmpNum = getDrive();
    if (tmpNum == -1)
        return QByteArray();
    du_distortion.d_drive = tmpNum;


    QByteArray tmpName(NAME_CARACT, (char)0x00);
    tmpStr = getEffectName();
    if (tmpStr.isNull())
        return QByteArray();
    tmpName.prepend(tmpStr.toUtf8());

    std::memcpy(du_distortion.d_name, tmpName.data(), NAME_CARACT);


    return QByteArray((char *)&(du_distortion), size());
}


int DuDistortion::size() const
{
    return FX_DIST_SIZE;
}


int DuDistortion::getOnOff() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_DIST_ONOFF);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDistortion::setOnOff(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_DIST_ONOFF);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuDistortion::getPreGain() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_DIST_PREGAIN);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDistortion::setPreGain(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_DIST_PREGAIN);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuDistortion::getEffectType() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_DIST_TYPE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDistortion::setEffectType(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_DIST_TYPE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuDistortion::getLowPassFilterFrequency() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_DIST_LOWPASSFILTERFREQUENCY);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDistortion::setLowPassFilterFrequency(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_DIST_LOWPASSFILTERFREQUENCY);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuDistortion::getLowPassFilterResonance() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_DIST_LOWPASSFILTERRESONANCE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDistortion::setLowPassFilterResonance(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_DIST_LOWPASSFILTERRESONANCE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuDistortion::getPostGain() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_DIST_POSTGAIN);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDistortion::setPostGain(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_DIST_POSTGAIN);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuDistortion::getDrive() const
{
    const DuNumericConstPtr& tmp = getChildAs<DuNumeric>(KEY_DIST_DRIVE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuDistortion::setDrive(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_DIST_DRIVE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


QString DuDistortion::getEffectName() const
{
    const DuStringConstPtr& tmp = getChildAs<DuString>(KEY_DIST_EFFECTNAME);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuDistortion::setEffectName(const QString &value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_DIST_EFFECTNAME);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}
