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
    DuChorus *chorus = new DuChorus();

    chorus->setMode(du_chorus.c_mode);

    chorus->setEffectLevel(du_chorus.c_effectlevel);
    chorus->setDelayTime(du_chorus.c_delaytime);
    chorus->setFeedback(du_chorus.c_feedback);
    chorus->setInputHighPassFilterFrequency(du_chorus.c_inputhighpassfilter);
    chorus->setHDAmp(du_chorus.c_hdamp);

    chorus->setModulationDepth(du_chorus.c_modulationdepth);
    chorus->setModulationRate(du_chorus.c_modulationrate);
    chorus->setTremoloShape(du_chorus.c_tremoloshape);
    chorus->setRotarySpeed(du_chorus.c_rotaryspeed);

    chorus->setEffectName(QString(QByteArray((char *)du_chorus.c_name, NAME_CARACT)));

    return chorus;
}


DuChorus *DuChorus::fromJson(const QJsonObject &jsonChorus)
{
    DuChorus *chorus = new DuChorus();
    const QStringList &keyList = chorus->keys();

    bool test = true;
    for (int i = 0; i < keyList.count(); i++)
    {
        test = test && jsonChorus.contains(keyList[i]);
    }

    if (!test)
        return NULL;

    chorus->setMode(jsonChorus[KEY_CHORUS_MODE].toInt());
    chorus->setEffectLevel(jsonChorus[KEY_CHORUS_EFFECTLEVEL].toInt());
    chorus->setDelayTime(jsonChorus[KEY_CHORUS_DELAYTIME].toInt());
    chorus->setFeedback(jsonChorus[KEY_CHORUS_FEEDBACK].toInt());
    chorus->setInputHighPassFilterFrequency(
                jsonChorus[KEY_CHORUS_INPUTHIGHPASSFILTER].toInt());
    chorus->setHDAmp(jsonChorus[KEY_CHORUS_HDAMP].toInt());

    chorus->setModulationDepth(jsonChorus[KEY_CHORUS_MODULATIONDEPTH].toInt());
    chorus->setModulationRate(jsonChorus[KEY_CHORUS_MODULATIONRATE].toInt());
    chorus->setTremoloShape(jsonChorus[KEY_CHORUS_TREMOLOSHAPE].toInt());
    chorus->setRotarySpeed(jsonChorus[KEY_CHORUS_ROTARYSPEED].toInt());

    chorus->setEffectName(jsonChorus[KEY_CHORUS_EFFECTNAME].toString());

    return chorus;
}


int DuChorus::getMode() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_MODE));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuChorus::setMode(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_MODE));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuChorus::getEffectLevel() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_EFFECTLEVEL));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuChorus::setEffectLevel(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_EFFECTLEVEL));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuChorus::getDelayTime() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_DELAYTIME));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuChorus::setDelayTime(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_DELAYTIME));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuChorus::getFeedback() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_FEEDBACK));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuChorus::setFeedback(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_FEEDBACK));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuChorus::getInputHighPassFilterFrequency() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_INPUTHIGHPASSFILTER));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuChorus::setInputHighPassFilterFrequency(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_INPUTHIGHPASSFILTER));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuChorus::getHDAmp() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_HDAMP));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuChorus::setHDAmp(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_HDAMP));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


int DuChorus::getModulationDepth() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_MODULATIONDEPTH));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuChorus::setModulationDepth(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_MODULATIONDEPTH));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuChorus::getModulationRate() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_MODULATIONRATE));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuChorus::setModulationRate(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_MODULATIONRATE));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuChorus::getTremoloShape() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_TREMOLOSHAPE));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuChorus::setTremoloShape(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_TREMOLOSHAPE));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuChorus::getRotarySpeed() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_ROTARYSPEED));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuChorus::setRotarySpeed(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_CHORUS_ROTARYSPEED));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


QString DuChorus::getEffectName() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_CHORUS_EFFECTNAME));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

void DuChorus::setEffectName(const QString &value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_CHORUS_EFFECTNAME));

    if (tmp == NULL)
        return;

    tmp->setString(value);
}
