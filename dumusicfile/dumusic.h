#ifndef DUMUSIC_H
#define DUMUSIC_H

#include "duheader.h"
#include "duparameters.h"
#include "dusonginfo.h"
#include "dutrack.h"
#include "instrument/effects/dureverb.h"

#include <QIODevice>


#define KEY_MUSIC_HEADER        "FileHeader"
#define KEY_MUSIC_PARAMETERS    "ControllerParameters"
#define KEY_MUSIC_SONGINFO      "SongInfo"
#define KEY_MUSIC_REVERB        "ReverbSettings"
#define KEY_MUSIC_TRACKS        "Tracks"


DU_OBJECT(DuMusic)

class DuMusic : public DuContainer
{
public:
    explicit DuMusic();
    ~DuMusic();

    virtual DuObjectPtr clone() const;

    static DuMusicPtr fromDuMusicBinary(const s_total_buffer &du_music, int fileSize);
    static DuMusicPtr fromDuMusicBinary(const music_song &du_song);
    static DuMusicPtr fromJson(const QJsonObject &jsonMusic);
    static DuMusicPtr fromBinary(const QByteArray &data);
    static DuMusicPtr fromBinary(QIODevice *input);

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

    DuParametersConstPtr getParameters() const;
    void setParameters(const DuParametersPtr &parameters);

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
