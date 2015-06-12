#ifndef DUSOUND_H
#define DUSOUND_H

#include <du_objects/general/dubinarydata.h>

DU_OBJECT(DuSound)

class DuSound : public DuBinaryData
{
public:
    DuSound();

    virtual DuObjectPtr clone() const;
};

Q_DECLARE_METATYPE(DuSoundPtr)
Q_DECLARE_METATYPE(DuSoundConstPtr)

#endif // DUSOUND_H
