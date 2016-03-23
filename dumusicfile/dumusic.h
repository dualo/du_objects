#ifndef DUMUSIC_H
#define DUMUSIC_H

#include "../general/ducontainer.h"


class MidiConversionHelper;
class QIODevice;

DU_OBJECT(DuArray);
DU_OBJECT(DuControllers);
DU_OBJECT(DuHeader);
DU_OBJECT(DuReverb);
DU_OBJECT(DuSongInfo);
DU_OBJECT(DuTrack);

DU_OBJECT(DuMusic);

class DuMusic : public DuContainer
{
public:
    DuMusic();

    virtual DuObjectPtr clone() const override;

    static DuMusicPtr fromDuMusicBinary(s_total_buffer &du_music, int fileSize);
    static DuMusicPtr fromBinary(const QByteArray &data);
    static DuMusicPtr fromBinary(QIODevice *input);

    static bool upgrade(s_total_buffer &du_music);

    static DuMusicPtr fromJson(const QJsonObject &jsonMusic);

    static DuMusicPtr fromMidi(const MidiConversionHelper &helper);

    QByteArray toDuMusicBinary() const override;
    QByteArray toMidiBinary() const override;

    virtual DuObjectPtr getChild(const QString &key) override;
    virtual DuObjectConstPtr getChild(const QString &key) const override;

    int size() const override;
    bool isEmpty() const;

    int databaseId() const;
    void setDatabaseId(int databaseId);

    QStringList lists() const;
    void setLists(const QStringList &lists);

    int indexInDevice() const;
    void setIndexInDevice(int indexInDevice);

    QString deviceSerialNumber() const;
    void setDeviceSerialNumber(const QString &deviceSerialNumber);

    DU_KEY_ACCESSORS_IN_CHILD(SongName,     QString)
    DU_KEY_ACCESSORS_IN_CHILD(FileVersion,  int)

    DU_KEY_ACCESSORS_OBJECT(Header,      DuHeader)
    DU_KEY_ACCESSORS_OBJECT(Controllers, DuControllers)
    DU_KEY_ACCESSORS_OBJECT(SongInfo,    DuSongInfo)
    DU_KEY_ACCESSORS_OBJECT(Reverb,      DuReverb)
    DU_KEY_ACCESSORS_OBJECT(Tracks,      DuArray)

public:
    bool appendTrack(const DuTrackPtr &track);

    DU_KEY_ACCESSORS(Transpose, int)

    DU_KEY_ACCESSORS(Playhead,  int)
    DU_KEY_ACCESSORS(State,     int)

private:
    int m_databaseId;
    QStringList m_lists;

    int m_indexInDevice;
    QString m_deviceSerialNumber;
};

Q_DECLARE_METATYPE(DuMusicPtr)
Q_DECLARE_METATYPE(DuMusicConstPtr)

#endif // DUMUSIC_H
