#include "DuArrangementAction.h"

#include "../general/DuEnum.h"
#include "../general/dunumeric.h"

DU_OBJECT_IMPL(DuArrangementAction)

DuArrangementAction::DuArrangementAction() : DuContainer()
{
    addChild(KeyType, new DuEnum<ARRANGEMENT_EVENTACTION>(ARRANGEMENTEVENTACTION_NONE, NUMERIC_DEFAULT_SIZE, NUM_ARRANGEMENTEVENTACTION, ARRANGEMENTEVENTACTION_NONE));
    addChild(KeyItem, new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));
    addChild(KeyCommand, new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));
    addChild(KeyValue, new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));
}

DuArrangementActionPtr DuArrangementAction::fromStruct(const s_arrangement_event_action &actionStruct)
{
    DuArrangementActionPtr action(new DuArrangementAction);

    bool verif = true;

    verif = action->setType(static_cast<ARRANGEMENT_EVENTACTION>(actionStruct.aea_type)) ? verif : false;
    verif = action->setItem(actionStruct.aea_item) ? verif : false;
    verif = action->setCommand(actionStruct.aea_cmd) ? verif : false;
    verif = action->setValue(actionStruct.aea_value) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "An attribute was not properly set";
    }

    return action;
}

DuArrangementActionPtr DuArrangementAction::fromBinary(const QByteArray &data)
{
    s_arrangement_event_action actionStruct;
    std::memcpy(&actionStruct, data.constData(), data.size());
    return fromStruct(actionStruct);
}

DuObjectPtr DuArrangementAction::clone() const
{
    return DuArrangementActionPtr(new DuArrangementAction(*this));
}

QByteArray DuArrangementAction::toDuMusicBinary() const
{
    s_arrangement_event_action action;

    ARRANGEMENT_EVENTACTION tmpEnum = getType();
    if (tmpEnum == static_cast<ARRANGEMENT_EVENTACTION>(-1))
        return QByteArray();
    action.aea_type = static_cast<quint8>(tmpEnum);

    int tmp = 0;

    tmp = getItem();
    if (tmp == -1)
        return QByteArray();
    action.aea_item = static_cast<quint8>(tmp);

    tmp = getCommand();
    if (tmp == -1)
        return QByteArray();
    action.aea_cmd = static_cast<quint8>(tmp);

    tmp = getValue();
    if (tmp == -1)
        return QByteArray();
    action.aea_value = static_cast<quint8>(tmp);

    return QByteArray(reinterpret_cast<const char*>(&action), ARRANGEMENT_EVENTACTION_SIZE);
}

int DuArrangementAction::size() const
{
    return ARRANGEMENT_EVENTACTION_SIZE;
}

DU_KEY_ACCESSORS_TEMPLATE_IMPL(DuArrangementAction, Type, Enum, ARRANGEMENT_EVENTACTION, ARRANGEMENT_EVENTACTION, static_cast<ARRANGEMENT_EVENTACTION>(-1))
DU_KEY_ACCESSORS_IMPL(DuArrangementAction, Item, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuArrangementAction, Command, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuArrangementAction, Value, Numeric, int, -1)
