#ifndef DUPRESET_H
#define DUPRESET_H

#include "ducontrollers.h"
#include "dueffectset.h"
#include "duexpressionparam.h"


#define PARAMS_NUMERIC_SIZE     2


DU_OBJECT(DuPreset);

class DuPreset : public DuContainer
{
public:
    explicit DuPreset();
    ~DuPreset();

    virtual DuObjectPtr clone() const override;

    static DuPresetPtr fromDuMusicBinary(const preset_instr &du_preset);
    static DuPresetPtr fromJson(const QJsonObject &jsonPreset);

    QByteArray toDuMusicBinary() const override;

    int size() const override;

    virtual DuObjectPtr getChild(const QString &key) override;
    virtual DuObjectConstPtr getChild(const QString &key) const override;

    DU_KEY_ACCESSORS_OBJECT(ExpressionParam,    DuExpressionParam)
    DU_KEY_ACCESSORS_OBJECT(Controllers,        DuControllers)
    DU_KEY_ACCESSORS_OBJECT(EffectSet,          DuEffectSet)

    // ExpressionParam
#define X(key, dutype, type, defaultReturn) DU_KEY_ACCESSORS_IN_CHILD(key, type)
    DuExpressionParam_Children
#undef X

    // Controllers
#define X(key, dutype, type, defaultReturn) DU_KEY_ACCESSORS_IN_CHILD(key, type)
    DuControllers_Children
#undef X

    // EffectSet
#define X(key, dutype, type, defaultReturn) DU_KEY_ACCESSORS_IN_CHILD(key, type)
    DuEffectSet_Children
#undef X
};

#endif // DUPRESET_H
