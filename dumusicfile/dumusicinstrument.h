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

    QByteArray toDuMusicBinary() const;

    int size() const;

    virtual DuObjectPtr getChild(const QString &key) override;
    virtual DuObjectConstPtr getChild(const QString &key) const override;

    DU_KEY_ACCESSORS_IN_CHILD(NameForDevice, QString)

    DU_KEY_ACCESSORS_OBJECT(InstrumentInfo, DuInstrumentInfo)
    DU_KEY_ACCESSORS_OBJECT(Preset,         DuPreset)
};

#endif // DUINSTRUMENT_H
