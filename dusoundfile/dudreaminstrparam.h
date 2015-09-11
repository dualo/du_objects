#ifndef DUDREAMINSTRPARAM_H
#define DUDREAMINSTRPARAM_H

#include "../general/ducontainer.h"


DU_OBJECT(DuDreamInstrParam);

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
