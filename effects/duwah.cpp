#include "duwah.h"

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


DuWah *DuWah::fromDuMusicFile(const FX_wah &du_wah)
{
    DuWah *wah = new DuWah;
    bool verif = true;

    verif = verif && wah->setFilterType(du_wah.w_filtertype);
    verif = verif && wah->setFilterFrequency(du_wah.w_filterfreq);
    verif = verif && wah->setFilterResonance(du_wah.w_filterres);
    verif = verif && wah->setAutoWahSensitivity(du_wah.w_autowahsensitivity);

    verif = verif && wah->setEffectName(
                QString(QByteArray((char *)du_wah.w_name, NAME_CARACT)));

    if (!verif)
    {
        delete wah;
        return NULL;
    }

    return wah;
}


DuWah *DuWah::fromJson(const QJsonObject &jsonWah)
{
    QJsonValue jsonFilterType   = jsonWah[KEY_WAH_FILTERTYPE];
    QJsonValue jsonFilterFreq   = jsonWah[KEY_WAH_FILTERFREQUENCY];
    QJsonValue jsonFilterRes    = jsonWah[KEY_WAH_FILTERRESONANCE];
    QJsonValue jsonSensitivity  = jsonWah[KEY_WAH_AUTOWAHSENSITIVITY];
    QJsonValue jsonEffectName   = jsonWah[KEY_WAH_EFFECTNAME];

    if (        !jsonFilterType.isDouble()  ||  !jsonFilterFreq.isDouble()
            ||  !jsonFilterRes.isDouble()   ||  !jsonSensitivity.isDouble()
            ||  !jsonEffectName.isString())

        return NULL;


    DuWah *wah = new DuWah;
    bool verif = true;

    verif = verif && wah->setFilterType(jsonFilterType.toInt());
    verif = verif && wah->setFilterFrequency(jsonFilterFreq.toInt());
    verif = verif && wah->setFilterResonance(jsonFilterRes.toInt());
    verif = verif && wah->setAutoWahSensitivity(jsonSensitivity.toInt());

    verif = verif && wah->setEffectName(jsonEffectName.toString());

    if (!verif)
    {
        delete wah;
        return NULL;
    }

    return wah;
}


QByteArray DuWah::toDuMusicFile() const
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
    QSharedPointer<DuNumeric> tmp = getChildAs<DuNumeric>(KEY_WAH_FILTERTYPE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuWah::setFilterType(int value)
{
    QSharedPointer<DuNumeric> tmp = getChildAs<DuNumeric>(KEY_WAH_FILTERTYPE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuWah::getFilterFrequency() const
{
    QSharedPointer<DuNumeric> tmp = getChildAs<DuNumeric>(KEY_WAH_FILTERFREQUENCY);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuWah::setFilterFrequency(int value)
{
    QSharedPointer<DuNumeric> tmp = getChildAs<DuNumeric>(KEY_WAH_FILTERFREQUENCY);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuWah::getFilterResonance() const
{
    QSharedPointer<DuNumeric> tmp = getChildAs<DuNumeric>(KEY_WAH_FILTERRESONANCE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuWah::setFilterResonance(int value)
{
    QSharedPointer<DuNumeric> tmp = getChildAs<DuNumeric>(KEY_WAH_FILTERRESONANCE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuWah::getAutoWahSensitivity() const
{
    QSharedPointer<DuNumeric> tmp = getChildAs<DuNumeric>(KEY_WAH_AUTOWAHSENSITIVITY);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuWah::setAutoWahSensitivity(int value)
{
    QSharedPointer<DuNumeric> tmp = getChildAs<DuNumeric>(KEY_WAH_AUTOWAHSENSITIVITY);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


QString DuWah::getEffectName() const
{
    QSharedPointer<DuString> tmp = getChildAs<DuString>(KEY_WAH_EFFECTNAME);

    if (tmp == NULL)
        return QString();

    return tmp->getString();
}

bool DuWah::setEffectName(const QString &value)
{
    QSharedPointer<DuString> tmp = getChildAs<DuString>(KEY_WAH_EFFECTNAME);

    if (tmp == NULL)
        return false;

    return tmp->setString(value);
}
