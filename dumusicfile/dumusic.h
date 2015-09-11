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
    explicit DuMusic();
    ~DuMusic();

    virtual DuObjectPtr clone() const;

    static DuMusicPtr fromDuMusicBinary(s_total_buffer &du_music, int fileSize);
    static DuMusicPtr fromBinary(const QByteArray &data);
    static DuMusicPtr fromBinary(QIODevice *input);

    static bool upgrade(s_total_buffer &du_music);

    static DuMusicPtr fromJson(const QJsonObject &jsonMusic);

    static DuMusicPtr fromMidi(const MidiConversionHelper &helper);

    QByteArray toDuMusicBinary() const;
    QByteArray toMidiBinary() const;

    int size() const;
    bool isEmpty() const;

    int databaseId() const;
    void setDatabaseId(int databaseId);

    QStringList lists() const;
    void setLists(const QStringList &lists);

    QString getSongName() const;
    bool setSongName(const QString &value);

    int getFileVersion() const;
    bool setFileVersion(int value);

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
};

Q_DECLARE_METATYPE(DuMusicPtr)
Q_DECLARE_METATYPE(DuMusicConstPtr)

#endif // DUMUSIC_H
