#ifndef DUINSTRUMENT_H
#define DUINSTRUMENT_H

#include "../general/ducontainer.h"


DU_OBJECT(DuInstrumentInfo);
DU_OBJECT(DuPreset);

DU_OBJECT(DuMusicInstrument);

class DuMusicInstrument : public DuContainer
{
public:
    DuMusicInstrument();
    virtual ~DuMusicInstrument() = default;

    virtual DuObjectPtr clone() const override;

    static DuMusicInstrumentPtr fromDuMusicBinary(const music_instr &du_instr);

    QByteArray toDuMusicBinary() const override;

    int size() const override;

    virtual DuObjectPtr getChild(const QString &key) override;
    virtual DuObjectConstPtr getChild(const QString &key) const override;

    DU_KEY_ACCESSORS_IN_CHILD(NameForDevice, QString)
    DU_KEY_ACCESSORS_IN_CHILD(UserID,        int)
    DU_KEY_ACCESSORS_IN_CHILD(ID,            int)
    DU_KEY_ACCESSORS_IN_CHILD(InstrVersion,  int)

    DU_KEY_ACCESSORS_OBJECT(InstrumentInfo, DuInstrumentInfo)
    DU_KEY_ACCESSORS_OBJECT(Preset,         DuPreset)
};

#endif // DUINSTRUMENT_H
