#ifndef DUVIBRATO_H
#define DUVIBRATO_H

#include "dueffectsettings.h"


DU_OBJECT(DuVibrato);

class DuVibrato : public DuEffectSettings
{
public:
    explicit DuVibrato();
    ~DuVibrato();

    virtual DuObjectPtr clone() const;

    static DuVibratoPtr fromDuMusicBinary(const FX_vibrato &du_vibrato);
    static DuVibratoPtr fromJson(const QJsonObject &jsonVibrato);

    QByteArray toDuMusicBinary() const;

    int size() const;


    DU_KEY_ACCESSORS(Depth,      int)
    DU_KEY_ACCESSORS(Delay,      int)
    DU_KEY_ACCESSORS(Rate,       int)
    DU_KEY_ACCESSORS(EffectName, QString)
};

#endif // DUVIBRATO_H
