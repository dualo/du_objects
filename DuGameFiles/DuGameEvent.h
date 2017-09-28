#ifndef DUGAMEEVENT_H
#define DUGAMEEVENT_H

#include "../general/ducontainer.h"

#pragma pack(push, 4)
#include "../du-touch/dualo_structs/arrangement_structs.h"
#pragma pack(pop)

#ifdef Q_OS_WIN
#include "../general/duarray.h"
#else
DU_OBJECT_TEMPLATE(DuArray);
#endif
DU_OBJECT(DuArrangementAction);
DU_OBJECT(DuGameEventMessage);
DU_OBJECT(DuGameExitCondition);
DU_OBJECT(DuNumeric);

DU_OBJECT(DuGameEvent);

class DuGameEvent : public DuContainer
{
public:
    DuGameEvent();
    virtual ~DuGameEvent() = default;

    static DuGameEventPtr fromStruct(const s_arrangement_event& eventStruct);
    static DuGameEventPtr fromBinary(const QByteArray& data);

    // DuObject interface
public:
    virtual DuObjectPtr clone() const Q_DECL_OVERRIDE;
    virtual QByteArray toDuMusicBinary() const Q_DECL_OVERRIDE;
    virtual int size() const Q_DECL_OVERRIDE;

    DU_KEY_ACCESSORS_OBJECT(IntroMessage, DuGameEventMessage)
    DU_KEY_ACCESSORS_OBJECT(IntroComment, DuGameEventMessage)

    DU_KEY_ACCESSORS_OBJECT_TEMPLATE(Actions, DuArray, DuArrangementAction)

    DU_KEY_ACCESSORS_OBJECT(ExitCondition, DuGameExitCondition)

    DU_KEY_ACCESSORS_OBJECT_TEMPLATE(Leds, DuArray, DuNumeric)
};

#endif // DUGAMEEVENT_H
