#include "duvibrato.h"

#include <cstring>

#include <QJsonObject>
#include <QDebug>

DU_OBJECT_IMPL(DuVibrato)

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

DuObjectPtr DuVibrato::clone() const
{
    return DuVibratoPtr(new DuVibrato(*this));
}


DuVibratoPtr DuVibrato::fromDuMusicBinary(const FX_vibrato &du_vibrato)
{
    DuVibratoPtr vibrato(new DuVibrato);
    bool verif = true;

    verif = verif && vibrato->setDepth(du_vibrato.v_depth);
    verif = verif && vibrato->setDelay(du_vibrato.v_delay);
    verif = verif && vibrato->setRate(du_vibrato.v_rate);

    verif = verif && vibrato->setEffectName(
                QString(QByteArray((char *)du_vibrato.v_name, NAME_CARACT)));

    if (!verif)
    {
        qWarning() << "DuVibrato::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    return vibrato;
}


DuVibratoPtr DuVibrato::fromJson(const QJsonObject &jsonVibrato)
{
    QJsonValue jsonDepth        = jsonVibrato[KEY_VIB_DEPTH];
    QJsonValue jsonDelay        = jsonVibrato[KEY_VIB_DELAY];
    QJsonValue jsonRate         = jsonVibrato[KEY_VIB_RATE];
    QJsonValue jsonEffectName   = jsonVibrato[KEY_VIB_EFFECTNAME];

    if (        !jsonDepth.isDouble()   ||  !jsonDelay.isDouble()
            ||  !jsonRate.isDouble()    ||  !jsonEffectName.isString())
    {
        qCritical() << "DuVibrato::fromJson():\n"
                    << "failed to generate DuVibrato\n"
                    << "a json key did not contain the proper type";

        return DuVibratoPtr();
    }


    DuVibratoPtr vibrato(new DuVibrato);
    bool verif = true;

    verif = verif && vibrato->setDepth(jsonDepth.toInt());
    verif = verif && vibrato->setDelay(jsonDelay.toInt());
    verif = verif && vibrato->setRate(jsonRate.toInt());

    verif = verif && vibrato->setEffectName(jsonEffectName.toString());

    if (!verif)
    {
        qWarning() << "DuVibrato::fromJson():\n"
                   << "an attribute was not properly set";
    }

    return vibrato;
}


QByteArray DuVibrato::toDuMusicBinary() const
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

    std::memcpy(du_vibrato.v_name, tmpName.data(), NAME_CARACT);


    return QByteArray((char *)&(du_vibrato), size());
}


int DuVibrato::size() const
{
    return FX_VIB_SIZE;
}


int DuVibrato::getDepth() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_VIB_DEPTH);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuVibrato::setDepth(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_VIB_DEPTH);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuVibrato::getDelay() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_VIB_DELAY);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuVibrato::setDelay(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_VIB_DELAY);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuVibrato::getRate() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_VIB_RATE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuVibrato::setRate(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_VIB_RATE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


QString DuVibrato::getEffectName() const
{
    const DuStringConstPtr &tmp = getChildAs<DuString>(KEY_VIB_EFFECTNAME);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuVibrato::setEffectName(const QString &value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_VIB_EFFECTNAME);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}
