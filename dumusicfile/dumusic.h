#ifndef DUMUSIC_H
#define DUMUSIC_H

#include "duheader.h"
#include "dusonginfo.h"
#include "dutrack.h"
#include "instrument/ducontrollers.h"
#include "instrument/effects/dureverb.h"


#define KEY_MUSIC_HEADER        "FileHeader"
#define KEY_MUSIC_CONTROLLERS   "ControllerParameters"
#define KEY_MUSIC_SONGINFO      "SongInfo"
#define KEY_MUSIC_REVERB        "ReverbSettings"
#define KEY_MUSIC_TRACKS        "Tracks"

#define KEY_MUSIC_TRANSPOSE     "Transpose"

#define KEY_MUSIC_PLAYHEAD      "Playhead"
#define KEY_MUSIC_STATE         "State"


class QIODevice;

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

    int getTranspose() const;
    bool setTranspose(int value);

    int getPlayhead() const;
    bool setPlayhead(int value);

    int getState() const;
    bool setState(int value);

private:
    int m_databaseId;
    QStringList m_lists;
};

Q_DECLARE_METATYPE(DuMusicPtr)
Q_DECLARE_METATYPE(DuMusicConstPtr)

#endif // DUMUSIC_H
