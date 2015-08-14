#ifndef DUDREAMINSTRPARAM_H
#define DUDREAMINSTRPARAM_H

#include "../general/ducontainer.h"


#pragma pack(push, 4)
struct dream_ip
{
    uint8_t min_vel;
    uint8_t start_note;
    uint8_t max_vel;
    uint8_t end_note;

    uint16_t dream_sp_addr;
};

#define INSTR_DREAM_IP_SIZE 6
#pragma pack(pop)


DU_OBJECT(DuDreamInstrParam)

class DuDreamInstrParam : public DuContainer
{
public:
    DuDreamInstrParam();

    virtual DuObjectPtr clone() const;
    virtual int size() const;

    static DuDreamInstrParamPtr fromBinary(const dream_ip& data);

    virtual QByteArray toDuMusicBinary() const;

    DU_KEY_ACCESSORS(MinVelocity,     int)
    DU_KEY_ACCESSORS(StartNote,       int)
    DU_KEY_ACCESSORS(MaxVelocity,     int)
    DU_KEY_ACCESSORS(EndNote,         int)

    DU_KEY_ACCESSORS(SampleParamAddr, int)
};

#endif // DUDREAMINSTRPARAM_H
