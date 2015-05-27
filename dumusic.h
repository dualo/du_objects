#ifndef DUMUSIC_H
#define DUMUSIC_H

#include "duheader.h"
#include "dusonginfo.h"
#include "dutrack.h"


#define KEY_MUSIC_HEADER        "FileHeader"
#define KEY_MUSIC_SONGINFO      "SongInfo"
#define KEY_MUSIC_TRACKS        "Tracks"


DU_OBJECT(DuMusic)

class DuMusic : public DuContainer
{
public:
    explicit DuMusic();
    ~DuMusic();

    virtual DuObjectPtr clone() const;

    static DuMusicPtr fromDuMusicFile(const s_total_buffer &du_music);
    static DuMusicPtr fromJson(const QJsonObject &jsonMusic);

    QByteArray toDuMusicFile() const;

    int size() const;

    DuHeaderConstPtr getHeader() const;
    void setHeader(const DuHeaderPtr &header);

    DuSongInfoConstPtr getSongInfo() const;
    void setSongInfo(const DuSongInfoPtr &songInfo);

    DuArrayConstPtr getTracks() const;
    void setTracks(const DuArrayPtr& array);

    bool appendTrack(const DuTrackPtr& track);
};

#endif // DUMUSIC_H
