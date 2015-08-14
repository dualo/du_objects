#ifndef DUMUSIC_H
#define DUMUSIC_H

#include "../general/ducontainer.h"


#define KEY_MUSIC_HEADER        "FileHeader"
#define KEY_MUSIC_CONTROLLERS   "ControllerParameters"
#define KEY_MUSIC_SONGINFO      "SongInfo"
#define KEY_MUSIC_REVERB        "ReverbSettings"
#define KEY_MUSIC_TRACKS        "Tracks"


class MidiConversionHelper;
class QIODevice;

DU_OBJECT(DuArray)
DU_OBJECT(DuControllers)
DU_OBJECT(DuHeader)
DU_OBJECT(DuReverb)
DU_OBJECT(DuSongInfo)
DU_OBJECT(DuTrack)

DU_OBJECT(DuMusic)

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

    DuHeaderConstPtr getHeader() const;
    void setHeader(const DuHeaderPtr &header);

    DuControllersConstPtr getControllers() const;
    void setControllers(const DuControllersPtr &controllers);

    DuSongInfoConstPtr getSongInfo() const;
    void setSongInfo(const DuSongInfoPtr &songInfo);

    DuReverbConstPtr getReverb() const;
    void setReverb(const DuReverbPtr &reverb);

    DuArrayConstPtr getTracks() const;
    void setTracks(const DuArrayPtr &array);

    bool appendTrack(const DuTrackPtr &track);

private:
    int m_databaseId;
    QStringList m_lists;
};

Q_DECLARE_METATYPE(DuMusicPtr)
Q_DECLARE_METATYPE(DuMusicConstPtr)

#endif // DUMUSIC_H
