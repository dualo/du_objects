#include "dupreset.h"

#include "ducontrollers.h"
#include "dueffectset.h"
#include "duexpressionparam.h"

#include <cstring>

#include <QJsonObject>


DU_OBJECT_IMPL(DuPreset)

DuPreset::DuPreset() :
    DuContainer()
{
    addChild(KeyExpressionParam, new DuExpressionParam);

    addChild(KeyControllers, new DuControllers);

    addChild(KeyEffectSet, new DuEffectSet);
}

DuPreset::~DuPreset()
{
}

DuObjectPtr DuPreset::clone() const
{
    return DuPresetPtr(new DuPreset(*this));
}


DuPresetPtr DuPreset::fromDuMusicBinary(const preset_instr &du_preset)
{
    DuPresetPtr preset(new DuPreset);

    const DuExpressionParamPtr &expressionParam =
            DuExpressionParam::fromDuMusicBinary(du_preset);
    if (expressionParam != NULL)
        preset->setExpressionParam(expressionParam);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuPreset::fromDuMusicBinary():\n"
                    << "failed to generate DuPreset\n"
                    << "the DuExpressionParam was not properly generated";

        return DuPresetPtr();
    }

    const DuControllersPtr &controllers =
            DuControllers::fromDuMusicBinary(du_preset);
    if (controllers != NULL)
        preset->setControllers(controllers);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuPreset::fromDuMusicBinary():\n"
                    << "failed to generate DuPreset\n"
                    << "the DuControllers was not properly generated";

        return DuPresetPtr();
    }

    const DuEffectSetPtr &effectSet =
            DuEffectSet::fromDuMusicBinary(du_preset);
    if (effectSet != NULL)
        preset->setEffectSet(effectSet);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuPreset::fromDuMusicBinary():\n"
                    << "failed to generate DuPreset\n"
                    << "the DuEffectSet was not properly generated";

        return DuPresetPtr();
    }

    return preset;
}


DuPresetPtr DuPreset::fromJson(const QJsonObject &jsonPreset)
{
    QJsonValue jsonExpressionParam  = jsonPreset[KeyExpressionParam];
    QJsonValue jsonControllers      = jsonPreset[KeyControllers];
    QJsonValue jsonEffectSet        = jsonPreset[KeyEffectSet];

    if (        !jsonExpressionParam.isObject() ||  !jsonControllers.isObject()
            ||  !jsonEffectSet.isObject())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuPreset::fromJson():\n"
                    << "failed to generate DuPreset\n"
                    << "a json key did not contain the proper type";

        return DuPresetPtr();
    }


    DuPresetPtr preset(new DuPreset);

    const DuExpressionParamPtr &expressionParam =
            DuExpressionParam::fromJson(jsonExpressionParam.toObject());
    if (expressionParam != NULL)
        preset->setExpressionParam(expressionParam);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuPreset::fromJson():\n"
                    << "failed to generate DuPreset\n"
                    << "the DuExpressionParam was not properly generated";

        return DuPresetPtr();
    }

    const DuControllersPtr &controllers =
            DuControllers::fromJson(jsonControllers.toObject());
    if (controllers != NULL)
        preset->setControllers(controllers);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuPreset::fromJson():\n"
                    << "failed to generate DuPreset\n"
                    << "the DuControllers was not properly generated";

        return DuPresetPtr();
    }

    const DuEffectSetPtr &effectSet =
            DuEffectSet::fromJson(jsonEffectSet.toObject());
    if (controllers != NULL)
        preset->setEffectSet(effectSet);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuPreset::fromJson():\n"
                    << "failed to generate DuPreset\n"
                    << "the DuEffectSet was not properly generated";

        return DuPresetPtr();
    }

    return preset;
}


QByteArray DuPreset::toDuMusicBinary() const
{
    preset_instr du_preset;

    QByteArray tmpPreset;
    tmpPreset.clear();

    std::memset((char*)&du_preset, 0, size());


    const DuExpressionParamConstPtr &expressionParam = getExpressionParam();
    if (expressionParam == NULL)
        return QByteArray();
    const QByteArray &expressionParamArray = expressionParam->toDuMusicBinary();
    if (expressionParamArray.isNull())
        return QByteArray();

    const DuControllersConstPtr &controllers = getControllers();
    if (controllers == NULL)
        return QByteArray();
    const QByteArray &controllersArray = controllers->toDuMusicBinary();
    if (controllersArray.isNull())
        return QByteArray();

    const DuEffectSetConstPtr &effectSet = getEffectSet();
    if (effectSet == NULL)
        return QByteArray();
    const QByteArray &effecSetArray = effectSet->toDuMusicBinary();
    if (effecSetArray.isNull())
        return QByteArray();

    tmpPreset = expressionParamArray + controllersArray + effecSetArray;

    std::memcpy(&(du_preset), tmpPreset.data(), size());


    return QByteArray((char *)&(du_preset), size());
}


int DuPreset::size() const
{
    return PRESET_STRUCT_SIZE;
}


DU_KEY_ACCESSORS_OBJECT_IMPL(DuPreset, ExpressionParam,  DuExpressionParam)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuPreset, Controllers, DuControllers)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuPreset, EffectSet,   DuEffectSet)
