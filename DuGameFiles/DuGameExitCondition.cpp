#include "DuGameExitCondition.h"

#include "../general/DuEnum.h"
#include "../general/dunumeric.h"

DU_OBJECT_IMPL(DuGameExitCondition)

DuGameExitCondition::DuGameExitCondition() : DuContainer()
{
    addChild(KeyType,
#if QT_VERSION >= QT_VERSION_CHECK(5, 8, 0)
             new DuEnum<ARRANGEMENT_EXITCONDITION>(ARRANGEMENTEXITCONDITION_NONE, 4, NUM_ARRANGEMENTEXITCONDITION, ARRANGEMENTEXITCONDITION_NONE)
#else
             new DuNumeric(ARRANGEMENTEXITCONDITION_NONE, 4, NUM_ARRANGEMENTEXITCONDITION, ARRANGEMENTEXITCONDITION_NONE)
#endif
    );
    addChild(KeyValue1, new DuNumeric(0, 2, UINT16_MAX, 0));
    addChild(KeyValue2, new DuNumeric(0, 2, UINT16_MAX, 0));
}

DuGameExitConditionPtr DuGameExitCondition::fromStruct(const s_arrangement_exit_condition &conditionStruct)
{
    DuGameExitConditionPtr condition(new DuGameExitCondition);

    bool verif = true;

    verif = condition->setType(static_cast<ARRANGEMENT_EXITCONDITION>(conditionStruct.aec_type)) ? verif : false;
    verif = condition->setValue1(conditionStruct.aec_value_1) ? verif : false;
    verif = condition->setValue2(conditionStruct.aec_value_2) ? verif : false;

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
    if (tmpEnum == static_cast<ARRANGEMENT_EXITCONDITION>(-1))
        return QByteArray();
    condition.aec_type = static_cast<quint32>(tmpEnum);

    int tmp = getValue1();
    if (tmp == -1)
        return QByteArray();
    condition.aec_value_1 = static_cast<quint16>(tmp);

    tmp = getValue2();
    if (tmp == -1)
        return QByteArray();
    condition.aec_value_2 = static_cast<quint16>(tmp);

    return QByteArray(reinterpret_cast<const char*>(&condition), ARRANGEMENT_EXITCONDITION_SIZE);
}

int DuGameExitCondition::size() const
{
    return ARRANGEMENT_EXITCONDITION_SIZE;
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 8, 0)
DU_KEY_ACCESSORS_TEMPLATE_IMPL(DuGameExitCondition, Type, Enum, ARRANGEMENT_EXITCONDITION, ARRANGEMENT_EXITCONDITION, static_cast<ARRANGEMENT_EXITCONDITION>(-1))
#else
DU_KEY_ACCESSORS_IMPL(DuGameExitCondition, Type, Numeric, ARRANGEMENT_EXITCONDITION, static_cast<ARRANGEMENT_EXITCONDITION>(-1))
#endif
DU_KEY_ACCESSORS_IMPL(DuGameExitCondition, Value1, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuGameExitCondition, Value2, Numeric, int, -1)
