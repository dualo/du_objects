#ifndef DUPRESET_H
#define DUPRESET_H

#include "duexpression.h"
#include "ducontrollers.h"
#include "dueffectset.h"


#define KEY_PRESET_EXPRESSION           "ExpressionSettings"
#define KEY_PRESET_CONTROLLERS          "ControllerParameters"
#define KEY_PRESET_EFFECTSET            "EffectChainSettings"


#define PARAMS_NUMERIC_SIZE     2
#define DIRECTIONGYRO_MINVALUE  -1
#define DIRECTIONGYRO_MAXVALUE  1


DU_OBJECT(DuPreset)

class DuPreset : public DuContainer
{
public:
    explicit DuPreset();
    ~DuPreset();

    virtual DuObjectPtr clone() const;

    static DuPresetPtr fromDuMusicBinary(const preset_instr &du_preset);
    static DuPresetPtr fromJson(const QJsonObject &jsonPreset);

    QByteArray toDuMusicBinary() const;

    int size() const;

    DuExpressionConstPtr getExpression() const;
    void setExpression(const DuExpressionPtr &expression);

    DuControllersConstPtr getControllers() const;
    void setControllers(const DuControllersPtr &controllers);

    DuEffectSetConstPtr getEffectSet() const;
    void setEffectSet(const DuEffectSetPtr &effectSet);
};

#endif // DUPRESET_H
