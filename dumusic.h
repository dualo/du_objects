#ifndef DUMUSIC_H
#define DUMUSIC_H

#include "duheader.h"
#include "dusonginfo.h"
#include "dutrack.h"


#define KEY_MUSIC_HEADER        "FileHeader"
#define KEY_MUSIC_SONGINFO      "SongInfo"
#define KEY_MUSIC_TRACKS        "Tracks"


class DuMusic : public DuContainer
{
public:
    explicit DuMusic();
    ~DuMusic();

    static DuMusic *fromDuMusicFile(const s_total_buffer &du_music);
    static DuMusic *fromJson(const QJsonObject &jsonMusic);

    QByteArray toDuMusicFile() const;

    int size() const;

    DuHeader *getHeader() const;
    void setHeader(DuHeader *header);

    DuSongInfo *getSongInfo() const;
    void setSongInfo(DuSongInfo *songInfo);

    DuArray *getTracks() const;
    void setTracks(DuArray *array);

    bool appendTrack(DuTrack *track);
};

#endif // DUMUSIC_H
