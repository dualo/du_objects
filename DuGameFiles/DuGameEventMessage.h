#ifndef DUGAMEEVENTMESSAGE_H
#define DUGAMEEVENTMESSAGE_H

#include "../general/ducontainer.h"

#pragma pack(push, 4)
#include "../du-touch/dualo_structs/arrangement_structs.h"
#pragma pack(pop)

DU_OBJECT(DuString);

DU_OBJECT(DuGameEventMessage);

class DuGameEventMessage : public DuContainer
{
public:
    DuGameEventMessage();
    virtual ~DuGameEventMessage() = default;

    static DuGameEventMessagePtr fromStruct(const s_arrangement_msg& messageStruct);

    // DuObject interface
public:
    virtual DuObjectPtr clone() const Q_DECL_OVERRIDE;
    virtual QByteArray toDuMusicBinary() const Q_DECL_OVERRIDE;
    virtual int size() const Q_DECL_OVERRIDE;

    DU_KEY_ACCESSORS(Line1, QString)
    DU_KEY_ACCESSORS(Line2, QString)
    DU_KEY_ACCESSORS(Line3, QString)
};

#endif // DUGAMEEVENTMESSAGE_H
