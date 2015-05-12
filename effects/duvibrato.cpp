#include "duvibrato.h"

DuVibrato::DuVibrato() :
    DuEffectSettings()
{
    addChild(KEY_VIB_DEPTH,
             new DuNumeric(FX_VIB_DEPTH_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_VIB_DEPTH_MAXVALUE, FX_VIB_DEPTH_MINVALUE));

    addChild(KEY_VIB_DELAY,
             new DuNumeric(FX_VIB_DELAY_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_VIB_DELAY_MAXVALUE, FX_VIB_DELAY_MINVALUE));

    addChild(KEY_VIB_RATE,
             new DuNumeric(FX_VIB_RATE_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_VIB_RATE_MAXVALUE, FX_VIB_RATE_MINVALUE));

    addChild(KEY_VIB_EFFECTNAME,
             new DuString(QString(DEFAULT_EFFECTNAME), NAME_CARACT));
}

DuVibrato::~DuVibrato()
{
}


DuVibrato *DuVibrato::fromDuMusicFile(const FX_vibrato &du_vibrato)
{
    DuVibrato *vibrato = new DuVibrato();

    vibrato->setDepth(du_vibrato.v_depth);
    vibrato->setDelay(du_vibrato.v_delay);
    vibrato->setRate(du_vibrato.v_rate);

    vibrato->setEffectName(QString(QByteArray((char *)du_vibrato.v_name, NAME_CARACT)));

    return vibrato;
}


DuVibrato *DuVibrato::fromJson(const QJsonObject &jsonVibrato)
{
    DuVibrato *vibrato = new DuVibrato();
    QStringList &keyList = vibrato->keys();

    bool test = true;
    for (int i = 0; i < keyList.count(); i++)
    {
        test = test && jsonVibrato.contains(keyList[i]);
    }

    if (!test)
        return NULL;

    vibrato->setDepth(jsonVibrato[KEY_VIB_DEPTH].toInt());
    vibrato->setDelay(jsonVibrato[KEY_VIB_DELAY].toInt());
    vibrato->setRate(jsonVibrato[KEY_VIB_RATE].toInt());

    vibrato->setEffectName(jsonVibrato[KEY_VIB_EFFECTNAME].toString());

    return vibrato;
}


int DuVibrato::getDepth() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_VIB_DEPTH));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuVibrato::setDepth(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_VIB_DEPTH));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuVibrato::getDelay() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_VIB_DELAY));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuVibrato::setDelay(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_VIB_DELAY));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuVibrato::getRate() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_VIB_RATE));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuVibrato::setRate(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_VIB_RATE));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


QString DuVibrato::getEffectName() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_VIB_EFFECTNAME));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

void DuVibrato::setEffectName(QString &value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_VIB_EFFECTNAME));

    if (tmp == NULL)
        return;

    tmp->setString(value);
}
