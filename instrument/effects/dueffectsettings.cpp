#include "dueffectsettings.h"


DU_OBJECT_IMPL(DuEffectSettings)

DuEffectSettings::DuEffectSettings() :
    DuContainer()
{
}

DuObjectPtr DuEffectSettings::clone() const
{
    return DuEffectSettingsPtr(new DuEffectSettings(*this));
}
