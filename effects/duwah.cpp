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

    wah->setFilterType(du_wah.w_filtertype);
    wah->setFilterFrequency(du_wah.w_filterfreq);
    wah->setFilterResonance(du_wah.w_filterres);
    wah->setAutoWahSensitivity(du_wah.w_autowahsensitivity);

    wah->setEffectName(QString(QByteArray((char *)du_wah.w_name, NAME_CARACT)));

    return wah;
}


DuWah *DuWah::fromJson(const QJsonObject &jsonWah)
{
    DuWah *wah = new DuWah();
    QStringList &keyList = wah->keys();

    bool test = true;
    for (int i = 0; i < keyList.count(); i++)
    {
        test = test && jsonWah.contains(keyList[i]);
    }

    if (!test)
        return NULL;

    wah->setFilterType(jsonWah[KEY_WAH_FILTERTYPE].toInt());
    wah->setFilterFrequency(jsonWah[KEY_WAH_FILTERFREQUENCY].toInt());
    wah->setFilterResonance(jsonWah[KEY_WAH_FILTERRESONANCE].toInt());
    wah->setAutoWahSensitivity(jsonWah[KEY_WAH_AUTOWAHSENSITIVITY].toInt());

    wah->setEffectName(jsonWah[KEY_WAH_EFFECTNAME].toString());

    return wah;
}


int DuWah::getFilterType() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_WAH_FILTERTYPE));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuWah::setFilterType(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_WAH_FILTERTYPE));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuWah::getFilterFrequency() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_WAH_FILTERFREQUENCY));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuWah::setFilterFrequency(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_WAH_FILTERFREQUENCY));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuWah::getFilterResonance() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_WAH_FILTERRESONANCE));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuWah::setFilterResonance(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_WAH_FILTERRESONANCE));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


int DuWah::getAutoWahSensitivity() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_WAH_AUTOWAHSENSITIVITY));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuWah::setAutoWahSensitivity(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_WAH_AUTOWAHSENSITIVITY));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


QString DuWah::getEffectName() const
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_WAH_EFFECTNAME));

    if (tmp == NULL)
        return "";

    return tmp->getString();
}

void DuWah::setEffectName(QString &value)
{
    DuString *tmp = dynamic_cast<DuString *>(getChild(KEY_WAH_EFFECTNAME));

    if (tmp == NULL)
        return;

    tmp->setString(value);
}
