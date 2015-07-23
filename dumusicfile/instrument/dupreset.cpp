#include "dupreset.h"

#include <cstring>

#include <QJsonObject>


DU_OBJECT_IMPL(DuPreset)

DuPreset::DuPreset() :
    DuContainer()
{
    addChild(KEY_PRESET_EXPRESSION, new DuExpression);

    addChild(KEY_PRESET_CONTROLLERS, new DuControllers);

    addChild(KEY_PRESET_EFFECTSET, new DuEffectSet);
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

    const DuExpressionPtr &expression =
            DuExpression::fromDuMusicBinary(du_preset);
    if (expression != NULL)
        preset->setExpression(expression);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuPreset::fromDuMusicBinary():\n"
                    << "failed to generate DuPreset\n"
                    << "the DuExpression was not properly generated";

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
    QJsonValue jsonExpression       = jsonPreset[KEY_PRESET_EXPRESSION];
    QJsonValue jsonControllers      = jsonPreset[KEY_PRESET_CONTROLLERS];
    QJsonValue jsonEffectSet        = jsonPreset[KEY_PRESET_EFFECTSET];

    if (        !jsonExpression.isObject()      ||  !jsonControllers.isObject()
            ||  !jsonEffectSet.isObject())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuPreset::fromJson():\n"
                    << "failed to generate DuPreset\n"
                    << "a json key did not contain the proper type";

        return DuPresetPtr();
    }


    DuPresetPtr preset(new DuPreset);

    const DuExpressionPtr &expression =
            DuExpression::fromJson(jsonExpression.toObject());
    if (expression != NULL)
        preset->setExpression(expression);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuPreset::fromJson():\n"
                    << "failed to generate DuPreset\n"
                    << "the DuExpression was not properly generated";

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

    QByteArray tmpClear(size(), (char)0x00);
    std::memcpy((char *)&(du_preset), tmpClear.data(), size());


    const DuExpressionConstPtr &expression = getExpression();
    if (expression == NULL)
        return QByteArray();
    const QByteArray &expressionArray = expression->toDuMusicBinary();
    if (expressionArray.isNull())
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

    tmpPreset = expressionArray + controllersArray + effecSetArray;

    std::memcpy(&(du_preset), tmpPreset.data(), size());


    return QByteArray((char *)&(du_preset), size());
}


int DuPreset::size() const
{
    return PRESET_STRUCT_SIZE;
}


DuExpressionConstPtr DuPreset::getExpression() const
{
    return getChildAs<DuExpression>(KEY_PRESET_EXPRESSION);
}

void DuPreset::setExpression(const DuExpressionPtr &expression)
{
    addChild(KEY_PRESET_EXPRESSION, expression);
}


DuControllersConstPtr DuPreset::getControllers() const
{
    return getChildAs<DuControllers>(KEY_PRESET_CONTROLLERS);
}

void DuPreset::setControllers(const DuControllersPtr &controllers)
{
    addChild(KEY_PRESET_CONTROLLERS, controllers);
}


DuEffectSetConstPtr DuPreset::getEffectSet() const
{
    return getChildAs<DuEffectSet>(KEY_PRESET_EFFECTSET);
}

void DuPreset::setEffectSet(const DuEffectSetPtr &effectSet)
{
    addChild(KEY_PRESET_EFFECTSET, effectSet);
}
