#include "duchorus.h"

#include <QJsonObject>
#include <QDebug>

DU_OBJECT_IMPL(DuChorus)

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

DuObjectPtr DuChorus::clone() const
{
    return DuChorusPtr(new DuChorus(*this));
}


DuChorusPtr DuChorus::fromDuMusicBinary(const FX_chorus &du_chorus)
{
    DuChorusPtr chorus(new DuChorus);
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
        qWarning() << "DuChorus::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    return chorus;
}


DuChorusPtr DuChorus::fromJson(const QJsonObject &jsonChorus)
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
    {

        qCritical() << "DuChorus::fromJson():\n"
                    << "failed to generate DuChorus\n"
                    << "a json key did not contain the proper type";

        return DuChorusPtr();
    }


    DuChorusPtr chorus(new DuChorus);
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
        qWarning() << "DuChorus::fromJson():\n"
                   << "an attribute was not properly set";
    }

    return chorus;
}


QByteArray DuChorus::toDuMusicBinary() const
{
    FX_chorus du_chorus;

    QString tmpStr;
    int tmpNum = 0;


    tmpNum = getMode();
    if (tmpNum == -1)
        return QByteArray();
    du_chorus.c_mode = tmpNum;

    tmpNum = getEffectLevel();
    if (tmpNum == -1)
        return QByteArray();
    du_chorus.c_effectlevel = tmpNum;

    tmpNum = getDelayTime();
    if (tmpNum == -1)
        return QByteArray();
    du_chorus.c_delaytime = tmpNum;

    tmpNum = getFeedback();
    if (tmpNum == -1)
        return QByteArray();
    du_chorus.c_feedback = tmpNum;

    tmpNum = getInputHighPassFilterFrequency();
    if (tmpNum == -1)
        return QByteArray();
    du_chorus.c_inputhighpassfilter = tmpNum;

    tmpNum = getHDAmp();
    if (tmpNum == -1)
        return QByteArray();
    du_chorus.c_hdamp = tmpNum;

    tmpNum = getModulationDepth();
    if (tmpNum == -1)
        return QByteArray();
    du_chorus.c_modulationdepth = tmpNum;

    tmpNum = getModulationRate();
    if (tmpNum == -1)
        return QByteArray();
    du_chorus.c_modulationrate = tmpNum;

    tmpNum = getTremoloShape();
    if (tmpNum == -1)
        return QByteArray();
    du_chorus.c_tremoloshape = tmpNum;

    tmpNum = getRotarySpeed();
    if (tmpNum == -1)
        return QByteArray();
    du_chorus.c_rotaryspeed = tmpNum;


    QByteArray tmpName(NAME_CARACT, (char)0x00);
    tmpStr = getEffectName();
    if (tmpStr.isNull())
        return QByteArray();
    tmpName.prepend(tmpStr.toUtf8());

    std::memcpy(du_chorus.c_name, tmpName.data(), NAME_CARACT);


    return QByteArray((char *)&(du_chorus), size());
}


int DuChorus::size() const
{
    return FX_CHORUS_SIZE;
}


int DuChorus::getMode() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_CHORUS_MODE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuChorus::setMode(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_CHORUS_MODE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuChorus::getEffectLevel() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_CHORUS_EFFECTLEVEL);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuChorus::setEffectLevel(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_CHORUS_EFFECTLEVEL);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuChorus::getDelayTime() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_CHORUS_DELAYTIME);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuChorus::setDelayTime(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_CHORUS_DELAYTIME);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuChorus::getFeedback() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_CHORUS_FEEDBACK);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuChorus::setFeedback(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_CHORUS_FEEDBACK);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuChorus::getInputHighPassFilterFrequency() const
{
    const DuNumericConstPtr &tmp =
            getChildAs<DuNumeric>(KEY_CHORUS_INPUTHIGHPASSFILTER);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuChorus::setInputHighPassFilterFrequency(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_CHORUS_INPUTHIGHPASSFILTER);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuChorus::getHDAmp() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_CHORUS_HDAMP);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuChorus::setHDAmp(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_CHORUS_HDAMP);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuChorus::getModulationDepth() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_CHORUS_MODULATIONDEPTH);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuChorus::setModulationDepth(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_CHORUS_MODULATIONDEPTH);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuChorus::getModulationRate() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_CHORUS_MODULATIONRATE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuChorus::setModulationRate(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_CHORUS_MODULATIONRATE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuChorus::getTremoloShape() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_CHORUS_TREMOLOSHAPE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuChorus::setTremoloShape(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_CHORUS_TREMOLOSHAPE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuChorus::getRotarySpeed() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_CHORUS_ROTARYSPEED);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuChorus::setRotarySpeed(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_CHORUS_ROTARYSPEED);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


QString DuChorus::getEffectName() const
{
    const DuStringConstPtr &tmp = getChildAs<DuString>(KEY_CHORUS_EFFECTNAME);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuChorus::setEffectName(const QString &value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_CHORUS_EFFECTNAME);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}
