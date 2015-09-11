#include "duvibrato.h"

#include <cstring>

#include <QJsonObject>

#include "../../../general/dunumeric.h"
#include "../../../general/dustring.h"

DU_OBJECT_IMPL(DuVibrato)

DuVibrato::DuVibrato() :
    DuEffectSettings()
{
    addChild(KeyDepth,
             new DuNumeric(FX_VIB_DEPTH_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_VIB_DEPTH_MAXVALUE, FX_VIB_DEPTH_MINVALUE));

    addChild(KeyDelay,
             new DuNumeric(FX_VIB_DELAY_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_VIB_DELAY_MAXVALUE, FX_VIB_DELAY_MINVALUE));

    addChild(KeyRate,
             new DuNumeric(FX_VIB_RATE_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_VIB_RATE_MAXVALUE, FX_VIB_RATE_MINVALUE));

    addChild(KeyEffectName,
             new DuString(QStringLiteral(DEFAULT_EFFECTNAME), NAME_CARACT));
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

    verif = vibrato->setDepth(du_vibrato.v_depth) ? verif : false;
    verif = vibrato->setDelay(du_vibrato.v_delay) ? verif : false;
    verif = vibrato->setRate(du_vibrato.v_rate) ? verif : false;

    verif = vibrato->setEffectName(QString(QByteArray((char *)du_vibrato.v_name, NAME_CARACT))) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuVibrato::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    return vibrato;
}


DuVibratoPtr DuVibrato::fromJson(const QJsonObject &jsonVibrato)
{
    QJsonValue jsonDepth        = jsonVibrato[KeyDepth];
    QJsonValue jsonDelay        = jsonVibrato[KeyDelay];
    QJsonValue jsonRate         = jsonVibrato[KeyRate];
    QJsonValue jsonEffectName   = jsonVibrato[KeyEffectName];

    if (        !jsonDepth.isDouble()   ||  !jsonDelay.isDouble()
            ||  !jsonRate.isDouble()    ||  !jsonEffectName.isString())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuVibrato::fromJson():\n"
                    << "failed to generate DuVibrato\n"
                    << "a json key did not contain the proper type";

        return DuVibratoPtr();
    }


    DuVibratoPtr vibrato(new DuVibrato);
    bool verif = true;

    verif = vibrato->setDepth(jsonDepth.toInt()) ? verif : false;
    verif = vibrato->setDelay(jsonDelay.toInt()) ? verif : false;
    verif = vibrato->setRate(jsonRate.toInt()) ? verif : false;

    verif = vibrato->setEffectName(jsonEffectName.toString()) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuVibrato::fromJson():\n"
                   << "an attribute was not properly set";
    }

    return vibrato;
}


QByteArray DuVibrato::toDuMusicBinary() const
{
    FX_vibrato du_vibrato;

    QString tmpStr;
    int tmpNum = 0;

    std::memcpy((char*)&du_vibrato, QByteArray(size(), 0), size());

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


DU_KEY_ACCESSORS_IMPL(DuVibrato, Depth,      Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuVibrato, Delay,      Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuVibrato, Rate,       Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuVibrato, EffectName, String, QString, QString())
