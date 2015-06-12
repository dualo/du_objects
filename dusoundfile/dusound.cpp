#include "dusound.h"

#include <QDebug>

DU_OBJECT_IMPL(DuSound)

DuSound::DuSound() :
    DuBinaryData()
{

}

DuObjectPtr DuSound::clone() const
{
    return DuSoundPtr(new DuSound(*this));
}
