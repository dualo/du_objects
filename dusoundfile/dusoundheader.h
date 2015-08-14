#ifndef DUSOUNDHEADER_H
#define DUSOUNDHEADER_H

#include "../general/ducontainer.h"


DU_OBJECT(DuSoundHeader)

class DuSoundHeader : public DuContainer
{
public:
    DuSoundHeader();

    virtual DuObjectPtr clone() const;
    virtual int size() const;

    virtual QByteArray toDuMusicBinary() const;

    static DuSoundHeaderPtr fromBinary(const s_instr_header& data);

    DU_KEY_ACCESSORS(Size,            int)
    DU_KEY_ACCESSORS(MappingAddress,  int)
    DU_KEY_ACCESSORS(MetadataAddress, int)
};

#endif // DUSOUNDHEADER_H
