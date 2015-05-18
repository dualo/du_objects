#include "duchorus.h"

DuChorus::DuChorus() :
    DuEffectSettings()
{
    addChild(KEY_CHORUS_MODE,
             new DuNumeric(FX_CHORUS_MODE_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_CHORUS_MODE_MAXVALUE, FX_CHORUS_MODE_MINVALUE));

    addChild(KEY_CHORUS_EFFECTLEVEL,
             new DuNumeric(FX_CHORUS_LEVEL_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_CHORUS_LEVEL_MAXVALUE, FX_CHORUS_LEVEL_MINVALUE));

    addChild(KEY_CHORUS_DELAYTIME,
             new DuNumeric(FX_CHORUS_DELAY_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_CHORUS_DELAY_MAXVALUE, FX_CHORUS_DELAY_MINVALUE));

    addChild(KEY_CHORUS_FEEDBACK,
             new DuNumeric(FX_CHORUS_FEEDBACK_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_CHORUS_FEEDBACK_MAXVALUE, FX_CHORUS_FEEDBACK_MINVALUE));

    addChild(KEY_CHORUS_INPUTHIGHPASSFILTER,
             new DuNumeric(FX_CHORUS_INPUTHP_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_CHORUS_INPUTHP_MAXVALUE, FX_CHORUS_INPUTHP_MINVALUE));

    addChild(KEY_CHORUS_HDAMP,
             new DuNumeric(FX_CHORUS_HDAMP_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_CHORUS_HDAMP_MAXVALUE, FX_CHORUS_HDAMP_MINVALUE));

    addChild(KEY_CHORUS_MODULATIONDEPTH,
             new DuNumeric(FX_CHORUS_MODDEPTH_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_CHORUS_MODDEPTH_MAXVALUE, FX_CHORUS_MODDEPTH_MINVALUE));

    addChild(KEY_CHORUS_MODULATIONRATE,
             new DuNumeric(FX_CHORUS_MODRATE_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_CHORUS_MODRATE_MAXVALUE, FX_CHORUS_MODRATE_MINVALUE));

    addChild(KEY_CHORUS_TREMOLOSHAPE,
             new DuNumeric(FX_CHORUS_TREMOLO_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_CHORUS_TREMOLO_MAXVALUE, FX_CHORUS_TREMOLO_MINVALUE));

    addChild(KEY_CHORUS_ROTARYSPEED,
             new DuNumeric(FX_CHORUS_ROTARY_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_CHORUS_ROTARY_MAXVALUE, FX_CHORUS_ROTARY_MINVALUE));

    addChild(KEY_CHORUS_EFFECTNAME,
             new DuString(QString(DEFAULT_EFFECTNAME), NAME_CARACT));
}

DuChorus::~DuChorus()
{
}


DuChorus *DuChorus::fromDuMusicFile(const FX_chorus &du_chorus)
{
    DuChorus *chorus = new DuChorus;
    bool verif = true;

    verif = verif && chorus->setMode(du_chorus.c_mode);

    verif = verif && chorus->setEffectLevel(du_chorus.c_effectlevel);
    verif = verif && chorus->setDelayTime(du_chorus.c_delaytime);
    verif = verif && chorus->setFeedback(du_chorus.c_feedback);
    verif = verif && chorus->setInputHighPassFilterFrequency(
                du_chorus.c_inputhighpassfilter);
    verif = verif && chorus->setHDAmp(du_chorus.c_hdamp);

    verif = verif && chorus->setModulationDepth(du_chorus.c_modulationdepth);
    verif = verif && chorus->setModulationRate(du_chorus.c_modulationrate);
    verif = verif && chorus->setTremoloShape(du_chorus.c_tremoloshape);
    verif = verif && chorus->setRotarySpeed(du_chorus.c_rotaryspeed);

    verif = verif && chorus->setEffectName(
                QString(QByteArray((char *)du_chorus.c_name, NAME_CARACT)));

    if (!verif)
    {
        delete chorus;
        return NULL;
    }

    return chorus;
}


DuChorus *DuChorus::fromJson(const QJsonObject &jsonChorus)
{
    QJsonValue jsonMode         = jsonChorus[KEY_CHORUS_MODE];
    QJsonValue jsonEffectLvl    = jsonChorus[KEY_CHORUS_EFFECTLEVEL];
    QJsonValue jsonDelayTime    = jsonChorus[KEY_CHORUS_DELAYTIME];
    QJsonValue jsonFeedback     = jsonChorus[KEY_CHORUS_FEEDBACK];
    QJsonValue jsonHiPassFreq   = jsonChorus[KEY_CHORUS_INPUTHIGHPASSFILTER];
    QJsonValue jsonHDAmp        = jsonChorus[KEY_CHORUS_HDAMP];
    QJsonValue jsonModDepth     = jsonChorus[KEY_CHORUS_MODULATIONDEPTH];
    QJsonValue jsonModRate      = jsonChorus[KEY_CHORUS_MODULATIONRATE];
    QJsonValue jsonTremShape    = jsonChorus[KEY_CHORUS_TREMOLOSHAPE];
    QJsonValue jsonRotSpeed     = jsonChorus[KEY_CHORUS_ROTARYSPEED];
    QJsonValue jsonEffectName   = jsonChorus[KEY_CHORUS_EFFECTNAME];

    if (        !jsonMode.isDouble()        ||  !jsonEffectLvl.isDouble()
            ||  !jsonDelayTime.isDouble()   ||  !jsonFeedback.isDouble()
            ||  !jsonHiPassFreq.isDouble()  ||  !jsonHDAmp.isDouble()
            ||  !jsonModDepth.isDouble()    ||  !jsonModRate.isDouble()
            ||  !jsonTremShape.isDouble()   ||  !jsonRotSpeed.isDouble()
            ||  !jsonEffectName.isString())

        return NULL;


    DuChorus *chorus = new DuChorus;
    bool verif = true;

    verif = verif && chorus->setMode(jsonMode.toInt());
    verif = verif && chorus->setEffectLevel(jsonEffectLvl.toInt());
    verif = verif && chorus->setDelayTime(jsonDelayTime.toInt());
    verif = verif && chorus->setFeedback(jsonFeedback.toInt());
    verif = verif && chorus->setInputHighPassFilterFrequency(jsonHiPassFreq.toInt());
    verif = verif && chorus->setHDAmp(jsonHDAmp.toInt());

    verif = verif && chorus->setModulationDepth(jsonModDepth.toInt());
    verif = verif && chorus->setModulationRate(jsonModRate.toInt());
    verif = verif && chorus->setTremoloShape(jsonTremShape.toInt());
    verif = verif && chorus->setRotarySpeed(jsonRotSpeed.toInt());

    verif = verif && chorus->setEffectName(jsonEffectName.toString());

    if (!verif)
    {
        delete chorus;
        return NULL;
    }

    return chorus;
}


QByteArray DuChorus::toDuMusicFile() const
{
    FX_chorus du_chorus;
    int tmp = 0;


    tmp = getMode();
    if (tmp == -1)
        return QByteArray();
    du_chorus.c_mode = tmp;

    tmp = getEffectLevel();
    if (tmp == -1)
        return QByteArray();
    du_chorus.c_effectlevel = tmp;

    tmp = getDelayTime();
    if (tmp == -1)
        return QByteArray();
    du_chorus.c_delaytime = tmp;

    tmp = getFeedback();
    if (tmp == -1)
        return QByteArray();
    du_chorus.c_feedback = tmp;

    tmp = getInputHighPassFilterFrequency();
    if (tmp == -1)
        return QByteArray();
    du_chorus.c_inputhighpassfilter = tmp;

    tmp = getHDAmp();
    if (tmp == -1)
        return QByteArray();
    du_chorus.c_hdamp = tmp;

    tmp = getModulationDepth();
    if (tmp == -1)
        return QByteArray();
    du_chorus.c_modulationdepth = tmp;

    tmp = getModulationRate();
    if (tmp == -1)
        return QByteArray();
    du_chorus.c_modulationrate = tmp;

    tmp = getTremoloShape();
    if (tmp == -1)
        return QByteArray();
    du_chorus.c_tremoloshape = tmp;

    tmp = getRotarySpeed();
    if (tmp == -1)
        return QByteArray();
    du_chorus.c_rotaryspeed = tmp;


    QByteArray tmpName(NAME_CARACT, (char)0x00);
    tmpName.prepend(getEffectName().toUtf8());
    if (tmpName == QString(""))
        return QByteArray();

#ifdef Q_OS_WIN
    memcpy_s(du_chorus.c_name, NAME_CARACT, tmpName.data(), NAME_CARACT);
#else
    memcpy(du_chorus.c_name, tmpName.data(), NAME_CARACT);
#endif


    return QByteArray((char *)&(du_chorus), FX_CHORUS_SIZE);
}


int DuChorus::size() const
{
    return FX_CHORUS_SIZE;
}


int DuChorus::getMode() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_MODE));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuChorus::setMode(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_MODE));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuChorus::getEffectLevel() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_EFFECTLEVEL));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuChorus::setEffectLevel(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_EFFECTLEVEL));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuChorus::getDelayTime() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_DELAYTIME));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuChorus::setDelayTime(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_DELAYTIME));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuChorus::getFeedback() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_FEEDBACK));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuChorus::setFeedback(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_FEEDBACK));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuChorus::getInputHighPassFilterFrequency() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_INPUTHIGHPASSFILTER));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuChorus::setInputHighPassFilterFrequency(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_INPUTHIGHPASSFILTER));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuChorus::getHDAmp() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_HDAMP));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuChorus::setHDAmp(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_HDAMP));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuChorus::getModulationDepth() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_MODULATIONDEPTH));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuChorus::setModulationDepth(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_MODULATIONDEPTH));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuChorus::getModulationRate() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_MODULATIONRATE));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuChorus::setModulationRate(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_MODULATIONRATE));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuChorus::getTremoloShape() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_TREMOLOSHAPE));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuChorus::setTremoloShape(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_TREMOLOSHAPE));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuChorus::getRotarySpeed() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_ROTARYSPEED));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuChorus::setRotarySpeed(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_ROTARYSPEED));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


QString DuChorus::getEffectName() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_CHORUS_EFFECTNAME));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

bool DuChorus::setEffectName(const QString &value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_CHORUS_EFFECTNAME));

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}
