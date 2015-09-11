#ifndef DUPRESET_H
#define DUPRESET_H

#include "duexpression.h"
#include "ducontrollers.h"
#include "dueffectset.h"


#define PARAMS_NUMERIC_SIZE     2


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

    DU_KEY_ACCESSORS_OBJECT(Expression,  DuExpression)
    DU_KEY_ACCESSORS_OBJECT(Controllers, DuControllers)
    DU_KEY_ACCESSORS_OBJECT(EffectSet,   DuEffectSet)
};

#endif // DUPRESET_H
