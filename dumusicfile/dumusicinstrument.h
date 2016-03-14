#ifndef DUINSTRUMENT_H
#define DUINSTRUMENT_H

#include "../general/ducontainer.h"


DU_OBJECT(DuInstrumentInfo);
DU_OBJECT(DuPreset);

DU_OBJECT(DuMusicInstrument);

class DuMusicInstrument : public DuContainer
{
public:
    explicit DuMusicInstrument();
    ~DuMusicInstrument();

    virtual DuObjectPtr clone() const;

    static DuMusicInstrumentPtr fromDuMusicBinary(const music_instr &du_instr);
    static DuMusicInstrumentPtr fromJson(const QJsonObject &jsonInstrument);

    QByteArray toDuMusicBinary() const;

    int size() const;

    DU_KEY_ACCESSORS_OBJECT(InstrumentInfo, DuInstrumentInfo)
    DU_KEY_ACCESSORS_OBJECT(Preset,         DuPreset)
};

#endif // DUINSTRUMENT_H
