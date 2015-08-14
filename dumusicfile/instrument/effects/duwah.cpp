#include "duwah.h"

#include <cstring>

#include <QJsonObject>

#include "../../../general/dunumeric.h"
#include "../../../general/dustring.h"

DU_OBJECT_IMPL(DuWah)

DuWah::DuWah() :
    DuEffectSettings()
{
    addChild(KEY_WAH_FILTERTYPE,
             new DuNumeric(FX_WAH_FILTERTYPE_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_WAH_FILTERTYPE_MAXVALUE, FX_WAH_FILTERTYPE_MINVALUE));

    addChild(KEY_WAH_FILTERFREQUENCY,
             new DuNumeric(FX_WAH_FILTERFREQ_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_WAH_FILTERFREQ_MAXVALUE, FX_WAH_FILTERFREQ_MINVALUE));

    addChild(KEY_WAH_FILTERRESONANCE,
             new DuNumeric(FX_WAH_FILTERRES_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_WAH_FILTERRES_MAXVALUE, FX_WAH_FILTERRES_MINVALUE));

    addChild(KEY_WAH_AUTOWAHSENSITIVITY,
             new DuNumeric(FX_WAH_SENSITIVITY_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           FX_WAH_SENSITIVITY_MAXVALUE, FX_WAH_SENSITIVITY_MINVALUE));

    addChild(KEY_WAH_EFFECTNAME,
             new DuString(QString(DEFAULT_EFFECTNAME), NAME_CARACT));
}

DuWah::~DuWah()
{
}

DuObjectPtr DuWah::clone() const
{
    return DuWahPtr(new DuWah(*this));
}


DuWahPtr DuWah::fromDuMusicBinary(const FX_wah &du_wah)
{
    DuWahPtr wah(new DuWah);
    bool verif = true;

    verif = wah->setFilterType(du_wah.w_filtertype) ? verif : false;
    verif = wah->setFilterFrequency(du_wah.w_filterfreq) ? verif : false;
    verif = wah->setFilterResonance(du_wah.w_filterres) ? verif : false;
    verif = wah->setAutoWahSensitivity(du_wah.w_autowahsensitivity) ? verif : false;

    verif = wah->setEffectName(QString(QByteArray((char *)du_wah.w_name, NAME_CARACT))) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuWah::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    return wah;
}


DuWahPtr DuWah::fromJson(const QJsonObject &jsonWah)
{
    QJsonValue jsonFilterType   = jsonWah[KEY_WAH_FILTERTYPE];
    QJsonValue jsonFilterFreq   = jsonWah[KEY_WAH_FILTERFREQUENCY];
    QJsonValue jsonFilterRes    = jsonWah[KEY_WAH_FILTERRESONANCE];
    QJsonValue jsonSensitivity  = jsonWah[KEY_WAH_AUTOWAHSENSITIVITY];
    QJsonValue jsonEffectName   = jsonWah[KEY_WAH_EFFECTNAME];

    if (        !jsonFilterType.isDouble()  ||  !jsonFilterFreq.isDouble()
            ||  !jsonFilterRes.isDouble()   ||  !jsonSensitivity.isDouble()
            ||  !jsonEffectName.isString())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuWah::fromJson():\n"
                    << "failed to generate DuWah\n"
                    << "a json key did not contain the proper type";

        return DuWahPtr();
    }


    DuWahPtr wah(new DuWah);
    bool verif = true;

    verif = wah->setFilterType(jsonFilterType.toInt()) ? verif : false;
    verif = wah->setFilterFrequency(jsonFilterFreq.toInt()) ? verif : false;
    verif = wah->setFilterResonance(jsonFilterRes.toInt()) ? verif : false;
    verif = wah->setAutoWahSensitivity(jsonSensitivity.toInt()) ? verif : false;

    verif = wah->setEffectName(jsonEffectName.toString()) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuWah::fromJson():\n"
                   << "an attribute was not properly set";
    }

    return wah;
}


QByteArray DuWah::toDuMusicBinary() const
{
    FX_wah du_wah;

    QString tmpStr;
    int tmpNum = 0;


    tmpNum = getFilterType();
    if (tmpNum == -1)
        return QByteArray();
    du_wah.w_filtertype = tmpNum;

    tmpNum = getFilterFrequency();
    if (tmpNum == -1)
        return QByteArray();
    du_wah.w_filterfreq = tmpNum;

    tmpNum = getFilterResonance();
    if (tmpNum == -1)
        return QByteArray();
    du_wah.w_filterres = tmpNum;

    tmpNum = getAutoWahSensitivity();
    if (tmpNum == -1)
        return QByteArray();
    du_wah.w_autowahsensitivity = tmpNum;


    QByteArray tmpName(NAME_CARACT, (char)0x00);
    tmpStr = getEffectName();
    if (tmpStr.isNull())
        return QByteArray();
    tmpName.prepend(tmpStr.toUtf8());

    std::memcpy(du_wah.w_name, tmpName.data(), NAME_CARACT);


    return QByteArray((char *)&(du_wah), size());
}


int DuWah::size() const
{
    return FX_WAH_SIZE;
}


int DuWah::getFilterType() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_WAH_FILTERTYPE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuWah::setFilterType(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_WAH_FILTERTYPE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuWah::getFilterFrequency() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_WAH_FILTERFREQUENCY);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuWah::setFilterFrequency(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_WAH_FILTERFREQUENCY);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuWah::getFilterResonance() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_WAH_FILTERRESONANCE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuWah::setFilterResonance(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_WAH_FILTERRESONANCE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuWah::getAutoWahSensitivity() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_WAH_AUTOWAHSENSITIVITY);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuWah::setAutoWahSensitivity(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_WAH_AUTOWAHSENSITIVITY);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


QString DuWah::getEffectName() const
{
    const DuStringConstPtr &tmp = getChildAs<DuString>(KEY_WAH_EFFECTNAME);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuWah::setEffectName(const QString &value)
{
    DuStringPtr tmp = getChildAs<DuString>(KEY_WAH_EFFECTNAME);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}
