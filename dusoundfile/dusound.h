#ifndef DUSOUND_H
#define DUSOUND_H

#include "../general/ducontainer.h"


DU_OBJECT(DuBinaryData);
DU_OBJECT(DuLayer);
DU_OBJECT(DuNote);
DU_OBJECT(DuSoundHeader);
DU_OBJECT(DuSoundInfo);
#ifdef Q_OS_WIN
#include "../general/duarray.h"
#else
DU_OBJECT_TEMPLATE(DuArray);
#endif

DU_OBJECT(DuSound);

class DuSound : public DuContainer
{
    static QAtomicInt m_currentGlobalID;
public:
    static int currentGlobalID();

public:
    DuSound();
    virtual ~DuSound() = default;

    virtual DuObjectPtr clone() const override;

    virtual int size() const override;
    bool isEmpty() const;

    static DuSoundPtr fromHeaderBinary(const QByteArray &data);
    static DuSoundPtr fromBinary(const QByteArray &data);
    static DuSoundPtr fromBinary(QIODevice *input);

    QByteArray toBinary(bool forDuTouchSOrL) const;
    bool headerIpSpSamplesBinary(bool forDuTouchSOrL,
                                 QByteArray& outHeader,
                                 QByteArray& outIpHeader,
                                 QByteArray& outIP,
                                 QByteArray& outSP,
                                 QByteArray& outSamples) const;
    QByteArray mappingLBinary() const;
    QByteArray mappingSBinary() const;
    QByteArray metadataBinary() const;

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

    bool getHasSamplesDownloaded() const;
    void setHasSamplesDownloaded(bool hasSamplesDownloaded);

    int getSizeWithSamples() const;
    void setSizeWithSamples(int sizeWithSamples);

    int getDefaultSlotL() const;
    void setDefaultSlotL(int defaultSlotL);

    int getDefaultSlotS() const;
    void setDefaultSlotS(int defaultSlotS);

    bool getIsInLFactorySndbk() const;
    void setIsInLFactorySndbk(bool isInLFactorySndbk);

    bool getIsInSFactorySndbk() const;
    void setIsInSFactorySndbk(bool isInSFactorySndbk);

    bool getIsFromCommunity() const;
    void setIsFromCommunity(bool isFromCommunity);

    int getAssociatedGameId() const;
    void setAssociatedGameId(int associatedGameId);

    DU_KEY_ACCESSORS_IN_CHILD(NameForDevice,        QString)
    DU_KEY_ACCESSORS_IN_CHILD(Octave,               int)
    DU_KEY_ACCESSORS_IN_CHILD(UserID,               int)
    DU_KEY_ACCESSORS_IN_CHILD(ID,                   int)
    DU_KEY_ACCESSORS_IN_CHILD(ActiveNoteOff,        bool)
    DU_KEY_ACCESSORS_IN_CHILD(Category,             QString)
    DU_KEY_ACCESSORS_IN_CHILD(RelativeVolume,       int)
    DU_KEY_ACCESSORS_IN_CHILD(InstrType,            INSTRUMENT_TYPE)
    DU_KEY_ACCESSORS_IN_CHILD(InstrVersion,         int)
    DU_KEY_ACCESSORS_IN_CHILD(HardInstrVersion,     int)
    DU_KEY_ACCESSORS_IN_CHILD(SoftInstrVersion,     int)

    DU_KEY_ACCESSORS_IN_CHILD(Name,                 QString)

    DU_KEY_ACCESSORS_OBJECT(Info,                   DuSoundInfo)

    DU_KEY_ACCESSORS_OBJECT_TEMPLATE(LayerArray,    DuArray, DuLayer)

    DU_KEY_ACCESSORS_OBJECT_TEMPLATE(MappingL,      DuArray, DuNote)
    DU_KEY_ACCESSORS_OBJECT_TEMPLATE(MappingS,      DuArray, DuNote)

    DU_KEY_ACCESSORS_OBJECT_TEMPLATE(Metadata,      DuArray, DuBinaryData)

    void cleanMappingForSpecificKey(int note);

private:
    int m_databaseId;
    QStringList m_lists;

    int m_indexInDevice;
    QString m_deviceSerialNumber;
    bool m_hasSamplesDownloaded;
    int m_sizeWithSamples;

    int m_defaultSlotL;
    int m_defaultSlotS;

    bool m_isInLFactorySndbk;
    bool m_isInSFactorySndbk;

    bool m_isFromCommunity;
    int m_associatedGameId;
};

Q_DECLARE_METATYPE(DuSoundPtr)
Q_DECLARE_METATYPE(DuSoundConstPtr)

#endif // DUSOUND_H
