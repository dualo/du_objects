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
    DuVibrato *vibrato = new DuVibrato;
    bool verif = true;

    verif = verif && vibrato->setDepth(du_vibrato.v_depth);
    verif = verif && vibrato->setDelay(du_vibrato.v_delay);
    verif = verif && vibrato->setRate(du_vibrato.v_rate);

    verif = verif && vibrato->setEffectName(
                QString(QByteArray((char *)du_vibrato.v_name, NAME_CARACT)));

    if (!verif)
    {
        delete vibrato;
        return NULL;
    }

    return vibrato;
}


DuVibrato *DuVibrato::fromJson(const QJsonObject &jsonVibrato)
{
    QJsonValue jsonDepth        = jsonVibrato[KEY_VIB_DEPTH];
    QJsonValue jsonDelay        = jsonVibrato[KEY_VIB_DELAY];
    QJsonValue jsonRate         = jsonVibrato[KEY_VIB_RATE];
    QJsonValue jsonEffectName   = jsonVibrato[KEY_VIB_EFFECTNAME];

    if (        !jsonDepth.isDouble()   ||  !jsonDelay.isDouble()
            ||  !jsonRate.isDouble()    ||  !jsonEffectName.isString())

        return NULL;


    DuVibrato *vibrato = new DuVibrato;
    bool verif = true;

    verif = verif && vibrato->setDepth(jsonDepth.toInt());
    verif = verif && vibrato->setDelay(jsonDelay.toInt());
    verif = verif && vibrato->setRate(jsonRate.toInt());

    verif = verif && vibrato->setEffectName(jsonEffectName.toString());

    if (!verif)
    {
        delete vibrato;
        return NULL;
    }

    return vibrato;
}


QByteArray DuVibrato::toDuMusicFile() const
{
    FX_vibrato du_vibrato;

    QString tmpStr;
    int tmpNum = 0;


    tmpNum = getDepth();
    if (tmpNum == -1)
        return QByteArray();
    du_vibrato.v_depth = tmpNum;

    tmpNum = getDelay();
    if (tmpNum == -1)
        return QByteArray();
    du_vibrato.v_delay = tmpNum;

    tmpNum = getRate();
    if (tmpNum == -1)
        return QByteArray();
    du_vibrato.v_rate = tmpNum;


    QByteArray tmpName(NAME_CARACT, (char)0x00);
    tmpStr = getEffectName();
    if (tmpStr.isNull())
        return QByteArray();
    tmpName.prepend(tmpStr.toUtf8());

#ifdef Q_OS_WIN
    memcpy_s(du_vibrato.v_name, NAME_CARACT, tmpName.data(), NAME_CARACT);
#else
    memcpy(du_vibrato.v_name, tmpName.data(), NAME_CARACT);
#endif


    return QByteArray((char *)&(du_vibrato), FX_VIB_SIZE);
}


int DuVibrato::size() const
{
    return FX_VIB_SIZE;
}


int DuVibrato::getDepth() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_VIB_DEPTH));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuVibrato::setDepth(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_VIB_DEPTH));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuVibrato::getDelay() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_VIB_DELAY));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuVibrato::setDelay(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_VIB_DELAY));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuVibrato::getRate() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_VIB_RATE));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuVibrato::setRate(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_VIB_RATE));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


QString DuVibrato::getEffectName() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_VIB_EFFECTNAME));

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuVibrato::setEffectName(const QString &value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_VIB_EFFECTNAME));

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}
