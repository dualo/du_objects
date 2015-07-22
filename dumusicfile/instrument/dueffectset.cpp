#include "dueffectset.h"

#include <cstring>

#include <QJsonObject>


DU_OBJECT_IMPL(DuEffectSet)

DuEffectSet::DuEffectSet() :
    DuContainer()
{
    addChild(KEY_EFFECTSET_ADSR_ONOFF,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x01, 0x00));

    addChild(KEY_EFFECTSET_COMPR_ONOFF,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x01, 0x00));

    addChild(KEY_EFFECTSET_DELAY_ONOFF,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x01, 0x00));

    addChild(KEY_EFFECTSET_DISTO_ONOFF,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x01, 0x00));

    addChild(KEY_EFFECTSET_EQUAL_ONOFF,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x01, 0x00));

    addChild(KEY_EFFECTSET_CHORUS_ONOFF,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x01, 0x00));

    addChild(KEY_EFFECTSET_VIB_ONOFF,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x01, 0x00));

    addChild(KEY_EFFECTSET_WAH_ONOFF,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x01, 0x00));


    addChild(KEY_EFFECTSET_PITCH,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));


    addChild(KEY_EFFECTSET_AUTOPITCHRATE,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KEY_EFFECTSET_AUTOPITCHRANGE,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));


    addChild(KEY_EFFECTSET_TREMOLORATE,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KEY_EFFECTSET_TREMOLORANGE,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));


    addChild(KEY_EFFECTSET_AUTOPANRATE,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KEY_EFFECTSET_AUTOPANRANGE,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));


    addChild(KEY_EFFECTSET_AUTOWAHRATE,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KEY_EFFECTSET_AUTOWAHRANGE,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));
}

DuEffectSet::~DuEffectSet()
{
}


DuObjectPtr DuEffectSet::clone() const
{
    return DuEffectSetPtr(new DuEffectSet(*this));
}


DuEffectSetPtr DuEffectSet::fromDuMusicBinary(const preset_instr &du_preset)
{
    DuEffectSetPtr effectSet(new DuEffectSet);
    bool verif = true;

    verif = effectSet->setAdsrOnOff(du_preset.s_adsr_onoff) ? verif : false;
    verif = effectSet->setCompressorOnOff(du_preset.s_compressor_onoff) ? verif : false;
    verif = effectSet->setDelayOnOff(du_preset.s_delay_onoff) ? verif : false;
    verif = effectSet->setDistortionOnOff(du_preset.s_distortion_onoff) ? verif : false;
    verif = effectSet->setEqualizerOnOff(du_preset.s_eq_onoff) ? verif : false;
    verif = effectSet->setChorusOnOff(du_preset.s_chorus_onoff) ? verif : false;
    verif = effectSet->setVibratoOnOff(du_preset.s_vibrato_onoff) ? verif : false;
    verif = effectSet->setWahOnOff(du_preset.s_wah_onoff) ? verif : false;

    verif = effectSet->setPitch(du_preset.s_pitch) ? verif : false;

    verif = effectSet->setAutopitchRate(du_preset.s_autopitch_rate) ? verif : false;
    verif = effectSet->setAutopitchRange(du_preset.s_autopitch_range) ? verif : false;
    verif = effectSet->setTremoloRate(du_preset.s_tremolo_rate) ? verif : false;
    verif = effectSet->setTremoloRange(du_preset.s_tremolo_range) ? verif : false;
    verif = effectSet->setAutopanRate(du_preset.s_autopan_rate) ? verif : false;
    verif = effectSet->setAutopanRange(du_preset.s_autopan_range) ? verif : false;
    verif = effectSet->setAutowahRate(du_preset.s_autowah_rate) ? verif : false;
    verif = effectSet->setAutowahRange(du_preset.s_autowah_range) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuEffectSet::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    return effectSet;
}


DuEffectSetPtr DuEffectSet::fromJson(const QJsonObject &jsonEffectSet)
{
    QJsonValue jsonAdsrOnOff            = jsonEffectSet[KEY_EFFECTSET_ADSR_ONOFF];
    QJsonValue jsonComprOnOff           = jsonEffectSet[KEY_EFFECTSET_COMPR_ONOFF];
    QJsonValue jsonDelayOnOff           = jsonEffectSet[KEY_EFFECTSET_DELAY_ONOFF];
    QJsonValue jsonDistoOnOff           = jsonEffectSet[KEY_EFFECTSET_DISTO_ONOFF];
    QJsonValue jsonEqualOnOff           = jsonEffectSet[KEY_EFFECTSET_EQUAL_ONOFF];
    QJsonValue jsonChorusOnOff          = jsonEffectSet[KEY_EFFECTSET_CHORUS_ONOFF];
    QJsonValue jsonVibOnOff             = jsonEffectSet[KEY_EFFECTSET_VIB_ONOFF];
    QJsonValue jsonWahOnOff             = jsonEffectSet[KEY_EFFECTSET_WAH_ONOFF];

    QJsonValue jsonPitch                = jsonEffectSet[KEY_EFFECTSET_PITCH];

    QJsonValue jsonAutopitchRate        = jsonEffectSet[KEY_EFFECTSET_AUTOPITCHRATE];
    QJsonValue jsonAutopitchRange       = jsonEffectSet[KEY_EFFECTSET_AUTOPITCHRANGE];
    QJsonValue jsonTremoloRate          = jsonEffectSet[KEY_EFFECTSET_TREMOLORATE];
    QJsonValue jsonTremoloRange         = jsonEffectSet[KEY_EFFECTSET_TREMOLORANGE];
    QJsonValue jsonAutopanRate          = jsonEffectSet[KEY_EFFECTSET_AUTOPANRATE];
    QJsonValue jsonAutopanRange         = jsonEffectSet[KEY_EFFECTSET_AUTOPANRANGE];
    QJsonValue jsonAutowahRate          = jsonEffectSet[KEY_EFFECTSET_AUTOWAHRATE];
    QJsonValue jsonAutowahRange         = jsonEffectSet[KEY_EFFECTSET_AUTOWAHRANGE];

    if (        !jsonAdsrOnOff.isDouble()       ||  !jsonComprOnOff.isDouble()
            ||  !jsonDelayOnOff.isDouble()      ||  !jsonDistoOnOff.isDouble()
            ||  !jsonEqualOnOff.isDouble()      ||  !jsonChorusOnOff.isDouble()
            ||  !jsonVibOnOff.isDouble()        ||  !jsonWahOnOff.isDouble()

            ||  !jsonPitch.isDouble()

            ||  !jsonAutopitchRate.isDouble()   ||  !jsonAutopitchRange.isDouble()
            ||  !jsonTremoloRate.isDouble()     ||  !jsonTremoloRange.isDouble()
            ||  !jsonAutopanRate.isDouble()     ||  !jsonAutopanRange.isDouble()
            ||  !jsonAutowahRate.isDouble()     ||  !jsonAutowahRange.isDouble())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuEffectSet::fromJson():\n"
                    << "failed to generate DuEffectSet\n"
                    << "a json key did not contain the proper type";

        return DuEffectSetPtr();
    }


    DuEffectSetPtr effectSet(new DuEffectSet);
    bool verif = true;

    verif = effectSet->setAdsrOnOff(jsonAdsrOnOff.toInt()) ? verif : false;
    verif = effectSet->setCompressorOnOff(jsonComprOnOff.toInt()) ? verif : false;
    verif = effectSet->setDelayOnOff(jsonDelayOnOff.toInt()) ? verif : false;
    verif = effectSet->setDistortionOnOff(jsonDistoOnOff.toInt()) ? verif : false;
    verif = effectSet->setEqualizerOnOff(jsonEqualOnOff.toInt()) ? verif : false;
    verif = effectSet->setChorusOnOff(jsonChorusOnOff.toInt()) ? verif : false;
    verif = effectSet->setVibratoOnOff(jsonVibOnOff.toInt()) ? verif : false;
    verif = effectSet->setWahOnOff(jsonWahOnOff.toInt()) ? verif : false;

    verif = effectSet->setPitch(jsonPitch.toInt()) ? verif : false;

    verif = effectSet->setAutopitchRate(jsonAutopitchRate.toInt()) ? verif : false;
    verif = effectSet->setAutopitchRange(jsonAutopitchRange.toInt()) ? verif : false;
    verif = effectSet->setTremoloRate(jsonTremoloRate.toInt()) ? verif : false;
    verif = effectSet->setTremoloRange(jsonTremoloRange.toInt()) ? verif : false;
    verif = effectSet->setAutopanRate(jsonAutopanRate.toInt()) ? verif : false;
    verif = effectSet->setAutopanRange(jsonAutopanRange.toInt()) ? verif : false;
    verif = effectSet->setAutowahRate(jsonAutowahRate.toInt()) ? verif : false;
    verif = effectSet->setAutowahRange(jsonAutowahRange.toInt()) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuEffectSet::fromJson():\n"
                   << "an attribute was not properly set";
    }

    return effectSet;
}


QByteArray DuEffectSet::toDuMusicBinary() const
{
    preset_instr du_effectset;

    int tmpNum = 0;

    QByteArray tmpClear(size(), (char)0x00);
    std::memcpy((char *)&(du_effectset), tmpClear.data(), size());


    tmpNum = getAdsrOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_adsr_onoff = tmpNum;

    tmpNum = getCompressorOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_compressor_onoff = tmpNum;

    tmpNum = getDelayOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_delay_onoff = tmpNum;

    tmpNum = getDistortionOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_distortion_onoff = tmpNum;

    tmpNum = getEqualizerOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_eq_onoff = tmpNum;

    tmpNum = getChorusOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_chorus_onoff = tmpNum;

    tmpNum = getVibratoOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_vibrato_onoff = tmpNum;

    tmpNum = getWahOnOff();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_wah_onoff = tmpNum;


    tmpNum = getPitch();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_pitch = tmpNum;


    tmpNum = getAutopitchRate();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_autopitch_rate = tmpNum;

    tmpNum = getAutopitchRange();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_autopitch_range = tmpNum;

    tmpNum = getTremoloRate();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_tremolo_rate = tmpNum;

    tmpNum = getTremoloRange();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_tremolo_range = tmpNum;

    tmpNum = getAutopanRate();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_autopan_rate = tmpNum;

    tmpNum = getAutopanRange();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_autopan_range = tmpNum;

    tmpNum = getAutowahRate();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_autowah_rate = tmpNum;

    tmpNum = getAutowahRange();
    if (tmpNum == -1)
        return QByteArray();
    du_effectset.s_autowah_range = tmpNum;


    return QByteArray((char *)&(du_effectset) + EFFECTSET_PRESET_OFFSET, size());
}


int DuEffectSet::size() const
{
    return EFFECTSET_SIZE;
}


int DuEffectSet::getAdsrOnOff() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_ADSR_ONOFF);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEffectSet::setAdsrOnOff(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_ADSR_ONOFF);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuEffectSet::getCompressorOnOff() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_COMPR_ONOFF);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEffectSet::setCompressorOnOff(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_COMPR_ONOFF);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuEffectSet::getDelayOnOff() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_DELAY_ONOFF);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEffectSet::setDelayOnOff(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_DELAY_ONOFF);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuEffectSet::getDistortionOnOff() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_DISTO_ONOFF);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEffectSet::setDistortionOnOff(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_DISTO_ONOFF);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuEffectSet::getEqualizerOnOff() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_EQUAL_ONOFF);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEffectSet::setEqualizerOnOff(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_EQUAL_ONOFF);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuEffectSet::getChorusOnOff() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_CHORUS_ONOFF);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEffectSet::setChorusOnOff(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_CHORUS_ONOFF);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuEffectSet::getVibratoOnOff() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_VIB_ONOFF);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEffectSet::setVibratoOnOff(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_VIB_ONOFF);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuEffectSet::getWahOnOff() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_WAH_ONOFF);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEffectSet::setWahOnOff(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_WAH_ONOFF);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuEffectSet::getPitch() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_PITCH);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEffectSet::setPitch(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_PITCH);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuEffectSet::getAutopitchRate() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_AUTOPITCHRATE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEffectSet::setAutopitchRate(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_AUTOPITCHRATE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuEffectSet::getAutopitchRange() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_AUTOPITCHRANGE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEffectSet::setAutopitchRange(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_AUTOPITCHRANGE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuEffectSet::getTremoloRate() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_TREMOLORATE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEffectSet::setTremoloRate(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_TREMOLORATE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuEffectSet::getTremoloRange() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_TREMOLORANGE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEffectSet::setTremoloRange(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_TREMOLORANGE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuEffectSet::getAutopanRate() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_AUTOPANRATE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEffectSet::setAutopanRate(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_AUTOPANRATE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuEffectSet::getAutopanRange() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_AUTOPANRANGE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEffectSet::setAutopanRange(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_AUTOPANRANGE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuEffectSet::getAutowahRate() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_AUTOWAHRATE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEffectSet::setAutowahRate(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_AUTOWAHRATE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuEffectSet::getAutowahRange() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_AUTOWAHRANGE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuEffectSet::setAutowahRange(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_EFFECTSET_AUTOWAHRANGE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}
