#ifndef DUGAMEEXITCONDITION_H
#define DUGAMEEXITCONDITION_H

#include "../general/ducontainer.h"

#pragma pack(push, 4)
#include "../du-touch/dualo_structs/arrangement_structs.h"
#pragma pack(pop)

DU_OBJECT(DuGameExitCondition);

class DuGameExitCondition : public DuContainer
{
public:
    DuGameExitCondition();
    virtual ~DuGameExitCondition() = default;

    static DuGameExitConditionPtr fromStruct(const s_arrangement_exit_condition& conditionStruct);

    // DuObject interface
public:
    virtual DuObjectPtr clone() const Q_DECL_OVERRIDE;
    virtual QByteArray toDuMusicBinary() const Q_DECL_OVERRIDE;
    virtual int size() const Q_DECL_OVERRIDE;

    DU_KEY_ACCESSORS_TEMPLATE(Type, ARRANGEMENT_EXITCONDITION)
    DU_KEY_ACCESSORS(Value, int)
};

#endif // DUGAMEEXITCONDITION_H
