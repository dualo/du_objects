#include "DuGameEvent.h"

#include "DuArrangementAction.h"
#include "DuGameEventMessage.h"
#include "DuGameExitCondition.h"

#include "../general/duarray.h"
#include "../general/dunumeric.h"

DU_OBJECT_IMPL(DuGameEvent)

DuGameEvent::DuGameEvent() : DuContainer()
{
    addChild(KeyIntroMessage, new DuGameEventMessage);
    addChild(KeyIntroComment, new DuGameEventMessage);

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

    const DuGameEventMessagePtr& introComment = DuGameEventMessage::fromStruct(eventStruct.ae_intro_cmt);
    if (introComment == Q_NULLPTR)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Can't parse DuGameEvent: intro comment corrupted";
        return {};
    }
    event->setIntroComment(introComment);

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

    return event;
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


    const DuGameEventMessageConstPtr &introComment = getIntroComment();
    if (introComment == Q_NULLPTR)
        return QByteArray();
    const QByteArray &introCommentData = introComment->toDuMusicBinary();
    if (introCommentData.isNull())
        return QByteArray();
    std::memcpy(&(event.ae_intro_cmt), introCommentData.constData(), static_cast<size_t>(introCommentData.size()));


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
DU_KEY_ACCESSORS_OBJECT_IMPL(DuGameEvent, IntroComment, DuGameEventMessage)

DU_KEY_ACCESSORS_OBJECT_TEMPLATE_IMPL(DuGameEvent, Actions, DuArray, DuArrangementAction)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuGameEvent, ExitCondition, DuGameExitCondition)

DU_KEY_ACCESSORS_OBJECT_TEMPLATE_IMPL(DuGameEvent, Leds, DuArray, DuNumeric)
