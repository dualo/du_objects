#include "DuGameExitCondition.h"

#include "../general/dunumeric.h"

DU_OBJECT_IMPL(DuGameExitCondition)

DuGameExitCondition::DuGameExitCondition() : DuContainer()
{
    addChild(KeyType, new DuNumeric(ARRANGEMENTEXITCONDITION_NONE, 4, NUM_ARRANGEMENTEXITCONDITION - 1, ARRANGEMENTEXITCONDITION_NONE));
    addChild(KeyValue, new DuNumeric);
}

DuGameExitConditionPtr DuGameExitCondition::fromStruct(const s_arrangement_exit_condition &conditionStruct)
{
    DuGameExitConditionPtr condition(new DuGameExitCondition);

    bool verif = true;

    verif = condition->setType(static_cast<ARRANGEMENT_EXITCONDITION>(conditionStruct.aec_type)) ? verif : false;
    verif = condition->setValue(conditionStruct.aec_value) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "An attribute was not properly set";
    }

    return condition;
}

DuObjectPtr DuGameExitCondition::clone() const
{
    return DuGameExitConditionPtr(new DuGameExitCondition(*this));
}

QByteArray DuGameExitCondition::toDuMusicBinary() const
{
    s_arrangement_exit_condition condition;

    ARRANGEMENT_EXITCONDITION tmpEnum = getType();
    if (tmpEnum == NUM_ARRANGEMENTEXITCONDITION)
        return QByteArray();
    condition.aec_type = static_cast<quint32>(tmpEnum);

    int tmp = getValue();
    if (tmp == -1)
        return QByteArray();
    condition.aec_value = static_cast<quint32>(tmp);

    return QByteArray(reinterpret_cast<const char*>(&condition), ARRANGEMENT_EXITCONDITION_SIZE);
}

int DuGameExitCondition::size() const
{
    return ARRANGEMENT_EXITCONDITION_SIZE;
}

DU_KEY_ACCESSORS_IMPL(DuGameExitCondition, Type, Numeric, ARRANGEMENT_EXITCONDITION, NUM_ARRANGEMENTEXITCONDITION)
DU_KEY_ACCESSORS_IMPL(DuGameExitCondition, Value, Numeric, int, -1)
