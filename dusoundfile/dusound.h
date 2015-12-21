#ifndef DUSOUND_H
#define DUSOUND_H

#include "../general/ducontainer.h"


DU_OBJECT(DuSoundHeader);
DU_OBJECT(DuSoundInfo);
DU_OBJECT(DuArray);

DU_OBJECT(DuSound);

class DuSound : public DuContainer
{
public:
    DuSound();

    virtual DuObjectPtr clone() const;

    virtual int size() const;

    static DuSoundPtr fromBinary(const QByteArray &data);
    static DuSoundPtr fromBinary(QIODevice *input);

    QByteArray toBinary() const;

    virtual DuObjectPtr getChild(const QString &key) override;
    virtual DuObjectConstPtr getChild(const QString &key) const override;

    int databaseId() const;
    void setDatabaseId(int databaseId);

    QStringList lists() const;
    void setLists(const QStringList &lists);

    int indexInDevice() const;
    void setIndexInDevice(int indexInDevice);

    QString deviceSerialNumber() const;
    void setDeviceSerialNumber(const QString &deviceSerialNumber);

    DU_KEY_ACCESSORS_IN_CHILD(NameForDevice,        QString)
    DU_KEY_ACCESSORS_IN_CHILD(DreamProgramChange,   int)
    DU_KEY_ACCESSORS_IN_CHILD(MidiControlChange0,   int)
    DU_KEY_ACCESSORS_IN_CHILD(KeyMapping,           int)
    DU_KEY_ACCESSORS_IN_CHILD(Octave,               int)
    DU_KEY_ACCESSORS_IN_CHILD(UserID,               int)
    DU_KEY_ACCESSORS_IN_CHILD(ID,                   int)
    DU_KEY_ACCESSORS_IN_CHILD(ActiveNoteOff,        int)
    DU_KEY_ACCESSORS_IN_CHILD(Category,             QString)
    DU_KEY_ACCESSORS_IN_CHILD(RelativeVolume,       int)
    DU_KEY_ACCESSORS_IN_CHILD(InstrType,            INSTRUMENT_TYPE)
    DU_KEY_ACCESSORS_IN_CHILD(InstrVersion,         int)
    DU_KEY_ACCESSORS_IN_CHILD(HardInstrVersion,     int)
    DU_KEY_ACCESSORS_IN_CHILD(SoftInstrVersion,     int)

    DU_KEY_ACCESSORS_IN_CHILD(Name,                 QString)

    DU_KEY_ACCESSORS_OBJECT(Info,                   DuSoundInfo)

    DU_KEY_ACCESSORS_OBJECT(LayerArray,             DuArray)

    DU_KEY_ACCESSORS_OBJECT(Mapping,                DuArray)

    DU_KEY_ACCESSORS_OBJECT(Metadata,               DuArray)

private:
    int m_databaseId;
    QStringList m_lists;

    int m_indexInDevice;
    QString m_deviceSerialNumber;
};

Q_DECLARE_METATYPE(DuSoundPtr)
Q_DECLARE_METATYPE(DuSoundConstPtr)

#endif // DUSOUND_H
