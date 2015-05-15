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

    int size() const;

    DuHeader *getHeader();
    void setHeader(DuHeader *header);

    DuSongInfo *getSongInfo();
    void setSongInfo(DuSongInfo *songInfo);

    DuArray *getTracks();
    void setTracks(DuArray *array);
};

#endif // DUMUSIC_H
