#ifndef DUARRANGEMENTACTION_H
#define DUARRANGEMENTACTION_H

#include "../general/ducontainer.h"

#pragma pack(push, 4)
#include "../du-touch/dualo_structs/arrangement_structs.h"
#pragma pack(pop)

DU_OBJECT(DuArrangementAction);

class DuArrangementAction : public DuContainer
{
public:
    DuArrangementAction();
    virtual ~DuArrangementAction() = default;

    static DuArrangementActionPtr fromStruct(const s_arrangement_event_action& actionStruct);

    // DuObject interface
public:
    virtual DuObjectPtr clone() const Q_DECL_OVERRIDE;
    virtual QByteArray toDuMusicBinary() const Q_DECL_OVERRIDE;
    virtual int size() const Q_DECL_OVERRIDE;

    DU_KEY_ACCESSORS(Type, ARRANGEMENT_EVENTACTION)
    DU_KEY_ACCESSORS(Loop, int)
    DU_KEY_ACCESSORS(Command, int)
    DU_KEY_ACCESSORS(Value, int)
};

#endif // DUARRANGEMENTACTION_H
