#include "dupreset.h"

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

DuObjectPtr DuPreset::getChild(const QString &key)
{
    if (false
        #define X(_key, dutype, type, defaultReturn) || key == Key ## _key
            DuExpressionParam_Children
        #undef X
            )
    {
        DuExpressionParamPtr expressionParam = getExpressionParam();
        if (expressionParam == NULL)
        {
            return DuObjectPtr();
        }

        return expressionParam->getChild(key);
    }
    else if (false
         #define X(_key, dutype, type, defaultReturn) || key == Key ## _key
             DuControllers_Children
         #undef X
             )
    {
        DuControllersPtr controllers = getControllers();
        if (controllers == NULL)
        {
            return DuObjectPtr();
        }

        return controllers->getChild(key);
    }
    else if (false
         #define X(_key, dutype, type, defaultReturn) || key == Key ## _key
             DuEffectSet_Children
         #undef X
             )
    {
        DuEffectSetPtr effectSetParam = getEffectSet();
        if (effectSetParam == NULL)
        {
            return DuObjectPtr();
        }

        return effectSetParam->getChild(key);
    }
    else
    {
        return DuContainer::getChild(key);
    }
}

DuObjectConstPtr DuPreset::getChild(const QString &key) const
{
    if (false
        #define X(_key, dutype, type, defaultReturn) || key == Key ## _key
            DuExpressionParam_Children
        #undef X
            )
    {
        DuExpressionParamConstPtr expressionParam = getExpressionParam();
        if (expressionParam == NULL)
        {
            return DuObjectPtr();
        }

        return expressionParam->getChild(key);
    }
    else if (false
         #define X(_key, dutype, type, defaultReturn) || key == Key ## _key
             DuControllers_Children
         #undef X
             )
    {
        DuControllersConstPtr controllers = getControllers();
        if (controllers == NULL)
        {
            return DuObjectPtr();
        }

        return controllers->getChild(key);
    }
    else if (false
         #define X(_key, dutype, type, defaultReturn) || key == Key ## _key
             DuEffectSet_Children
         #undef X
             )
    {
        DuEffectSetConstPtr effectSetParam = getEffectSet();
        if (effectSetParam == NULL)
        {
            return DuObjectPtr();
        }

        return effectSetParam->getChild(key);
    }
    else
    {
        return DuContainer::getChild(key);
    }
}


DU_KEY_ACCESSORS_OBJECT_IMPL(DuPreset, ExpressionParam,  DuExpressionParam)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuPreset, Controllers, DuControllers)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuPreset, EffectSet,   DuEffectSet)

// ExpressionParam
#define X(key, dutype, type, defaultReturn) DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuPreset, key, DuExpressionParam, ExpressionParam, type, defaultReturn)
    DuExpressionParam_Children
#undef X

// Controllers
#define X(key, dutype, type, defaultReturn) DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuPreset, key, DuControllers, Controllers, type, defaultReturn)
    DuControllers_Children
#undef X

// EffectSet
#define X(key, dutype, type, defaultReturn) DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuPreset, key, DuEffectSet, EffectSet, type, defaultReturn)
    DuEffectSet_Children
#undef X
