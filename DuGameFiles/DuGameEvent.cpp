#include "DuGameEvent.h"

#include "DuArrangementAction.h"
#include "DuGameEventMessage.h"
#include "DuGameExitCondition.h"

#include "../general/duarray.h"
#include "../general/dunumeric.h"

#include <cstring>

DU_OBJECT_IMPL(DuGameEvent)

DuGameEvent::DuGameEvent() : DuContainer()
{
    addChild(KeyIntroMessage, new DuGameEventMessage);

    addChild(KeyWaitForLoopStart, new DuNumeric(0xFF, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));

    addChild(KeyNextEvent, new DuNumeric(0xFF, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));
    addChild(KeyBackwardEvent, new DuNumeric(0xFF, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));
    addChild(KeyForwardEvent, new DuNumeric(0xFF, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));

    addChild(KeyActions, new DuArray<DuArrangementAction>(ARRANGEMENT_MAXEVENTACTION));

    addChild(KeyExitCondition, new DuGameExitCondition);

    DuArrayPtr<DuNumeric> ledsArray(new DuArray<DuNumeric>(NUM_LED_VALUE));
    for (int i = 0; i < NUM_LED_VALUE; ++i)
        ledsArray->append(new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));
    addChild(KeyLeds, ledsArray);
}

DuGameEventPtr DuGameEvent::fromStruct(const s_arrangement_event& eventStruct)
{
    DuGameEventPtr event(new DuGameEvent);

    const DuGameEventMessagePtr& introMessage = DuGameEventMessage::fromStruct(eventStruct.ae_intro_msg);
    if (introMessage == Q_NULLPTR)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Can't parse DuGameEvent: intro message corrupted";
        return {};
    }
    event->setIntroMessage(introMessage);

    if (eventStruct.ae_nb_actions > ARRANGEMENT_MAXEVENTACTION)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Can't parse DuGameEvent: nb actions too large -> got" << eventStruct.ae_nb_actions << ", max is" << ARRANGEMENT_MAXEVENTACTION;
        return {};
    }

    DuArrayPtr<DuArrangementAction> actionsArray(new DuArray<DuArrangementAction>(ARRANGEMENT_MAXEVENTACTION));
    for (uint i = 0; i < eventStruct.ae_nb_actions; ++i)
    {
        const DuArrangementActionPtr& action = DuArrangementAction::fromStruct(eventStruct.ae_actionlist[i]);
        if (action == Q_NULLPTR)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Can't parse DuGameEvent: action" << i << "corrupted";
            return {};
        }

        if (!actionsArray->append(action))
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Can't parse DuGameEvent: the action" << i << "could not be appended";
            return {};
        }
    }
    event->setActions(actionsArray);

    const DuGameExitConditionPtr& exitCondition = DuGameExitCondition::fromStruct(eventStruct.ae_exit_condition);
    if (exitCondition == Q_NULLPTR)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Can't parse DuGameEvent: exit condition corrupted";
        return {};
    }
    event->setExitCondition(exitCondition);

    DuArrayPtr<DuNumeric> ledsArray(new DuArray<DuNumeric>(NUM_LED_VALUE));
    for (int i = 0; i < NUM_LED_VALUE; ++i)
    {
        DuNumericPtr led(new DuNumeric(eventStruct.ae_led[i], NUMERIC_DEFAULT_SIZE, 0xFF, 0x00, 0));
        if (led == Q_NULLPTR)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Can't parse DuGameEvent: led" << i << "corrupted";
            return {};
        }

        if (!ledsArray->append(led))
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Can't parse DuGameEvent: the led" << i << "could not be appended";
            return {};
        }
    }
    event->setLeds(ledsArray);

    bool verif = true;

    verif = event->setWaitForLoopStart(eventStruct.ae_wait_for_loop_start) ? verif : false;
    verif = event->setNextEvent(eventStruct.ae_nextevent) ? verif : false;
    verif = event->setBackwardEvent(eventStruct.ae_backwardevent) ? verif : false;
    verif = event->setForwardEvent(eventStruct.ae_forwardevent) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "An attribute was not properly set";
    }

    return event;
}

DuGameEventPtr DuGameEvent::fromBinary(const QByteArray &data)
{
    s_arrangement_event eventStruct;
    std::memcpy(&eventStruct, data.constData(), data.size());
    return fromStruct(eventStruct);
}

DuObjectPtr DuGameEvent::clone() const
{
    return DuGameEventPtr(new DuGameEvent(*this));
}

QByteArray DuGameEvent::toDuMusicBinary() const
{
    s_arrangement_event event;
    std::memset(&event, 0x00, ARRANGEMENT_EVENT_SIZE);


    const DuGameEventMessageConstPtr &introMessage = getIntroMessage();
    if (introMessage == Q_NULLPTR)
        return QByteArray();
    const QByteArray &introMessageData = introMessage->toDuMusicBinary();
    if (introMessageData.isNull())
        return QByteArray();
    std::memcpy(&(event.ae_intro_msg), introMessageData.constData(), static_cast<size_t>(introMessageData.size()));


    int tmp = getWaitForLoopStart();
    if (tmp == -1)
        return QByteArray();
    event.ae_wait_for_loop_start = static_cast<quint8>(tmp);


    tmp = getNextEvent();
    if (tmp == -1)
        return QByteArray();
    event.ae_nextevent = static_cast<quint8>(tmp);

    tmp = getForwardEvent();
    if (tmp == -1)
        return QByteArray();
    event.ae_forwardevent = static_cast<quint8>(tmp);

    tmp = getBackwardEvent();
    if (tmp == -1)
        return QByteArray();
    event.ae_backwardevent = static_cast<quint8>(tmp);


    const DuArrayConstPtr<DuArrangementAction> &actions = getActions();
    if (actions == Q_NULLPTR)
        return QByteArray();

    event.ae_nb_actions = actions->count();

    const QByteArray &actionsData = actions->toDuMusicBinary();
    if (actionsData.isNull())
        return QByteArray();
    std::memcpy(&(event.ae_actionlist), actionsData.constData(), static_cast<size_t>(actionsData.size()));


    const DuGameExitConditionConstPtr &exitConditon = getExitCondition();
    if (exitConditon == Q_NULLPTR)
        return QByteArray();
    const QByteArray &exitConditonData = exitConditon->toDuMusicBinary();
    if (exitConditonData.isNull())
        return QByteArray();
    std::memcpy(&(event.ae_exit_condition), exitConditonData.constData(), static_cast<size_t>(exitConditonData.size()));


    const DuArrayConstPtr<DuNumeric> &leds = getLeds();
    if (leds == Q_NULLPTR)
        return QByteArray();
    std::memcpy(event.ae_led, leds->toDuMusicBinary().constData(), static_cast<size_t>(leds->size()));


    return QByteArray(reinterpret_cast<const char*>(&event), ARRANGEMENT_EVENT_SIZE);
}

int DuGameEvent::size() const
{
    return ARRANGEMENT_EVENT_SIZE;
}

DU_KEY_ACCESSORS_OBJECT_IMPL(DuGameEvent, IntroMessage, DuGameEventMessage)

DU_KEY_ACCESSORS_IMPL(DuGameEvent, WaitForLoopStart, Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuGameEvent, NextEvent, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuGameEvent, ForwardEvent, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuGameEvent, BackwardEvent, Numeric, int, -1)

DU_KEY_ACCESSORS_OBJECT_TEMPLATE_IMPL(DuGameEvent, Actions, DuArray, DuArrangementAction)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuGameEvent, ExitCondition, DuGameExitCondition)

DU_KEY_ACCESSORS_OBJECT_TEMPLATE_IMPL(DuGameEvent, Leds, DuArray, DuNumeric)
