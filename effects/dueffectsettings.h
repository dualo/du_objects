#ifndef DUEFFECTSETTINGS_H
#define DUEFFECTSETTINGS_H

#include "../ducontainer.h"
#include "../dunumeric.h"
#include "../dustring.h"


#define DEFAULT_EFFECTNAME      "off"


DU_OBJECT(DuEffectSettings)

class DuEffectSettings : public DuContainer
{
public:
    explicit DuEffectSettings();
    virtual ~DuEffectSettings();

    virtual DuObjectPtr clone() const;
};

#endif // DUEFFECTSETTINGS_H
