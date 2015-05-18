#include "dumusic.h"

DuMusic::DuMusic() :
    DuContainer()
{
    addChild(KEY_MUSIC_HEADER, new DuHeader());

    addChild(KEY_MUSIC_SONGINFO, new DuSongInfo());

    addChild(KEY_MUSIC_TRACKS, new DuArray(MUSIC_MAXTRACK));
}

DuMusic::~DuMusic()
{
}


DuMusic *DuMusic::fromDuMusicFile(const s_total_buffer &du_music)
{
    DuMusic *music = new DuMusic;

    DuHeader *header = DuHeader::fromDuMusicFile(du_music.local_song);
    if (header != NULL)
        music->setHeader(header);
    else
    {
        delete music;
        return NULL;
    }

    DuSongInfo *songInfo = DuSongInfo::fromDuMusicFile(du_music.local_song);
    if (songInfo != NULL)
        music->setSongInfo(songInfo);
    else
    {
        delete music;
        return NULL;
    }

    for (int i = 0; i < MUSIC_MAXTRACK; i++)
    {
        DuTrack *track = DuTrack::fromDuMusicFile(du_music.local_song.s_track[i],
                                                  du_music.local_buffer);
        if (track == NULL)
        {
            delete music;
            return NULL;
        }
        if (!music->appendTrack(track))
        {
            delete music;
            return NULL;
        }
    }

    return music;
}


DuMusic *DuMusic::fromJson(const QJsonObject &jsonMusic)
{
    QJsonValue jsonHeader   = jsonMusic[KEY_MUSIC_HEADER];
    QJsonValue jsonSongInfo = jsonMusic[KEY_MUSIC_SONGINFO];
    QJsonValue jsonTracks   = jsonMusic[KEY_MUSIC_TRACKS];

    if (        !jsonHeader.isObject()  ||  !jsonSongInfo.isObject()
            ||  !jsonTracks.isArray())

        return NULL;


    DuMusic *music = new DuMusic;

    DuHeader *header = DuHeader::fromJson(jsonHeader.toObject());
    if (header != NULL)
        music->setHeader(header);
    else
    {
        delete music;
        return NULL;
    }

    DuSongInfo *songInfo = DuSongInfo::fromJson(jsonSongInfo.toObject());
    if (songInfo != NULL)
        music->setSongInfo(songInfo);
    else
    {
        delete music;
        return NULL;
    }

    const QJsonArray &jsonTrackArray = jsonTracks.toArray();
    for (int i = 0; i < jsonTrackArray.count(); i++)
    {
        DuTrack *track = DuTrack::fromJson(jsonTrackArray[i].toObject());
        if (track == NULL)
        {
            delete music;
            return NULL;
        }
        if (!music->appendTrack(track))
        {
            delete music;
            return NULL;
        }
    }

    return music;
}


int DuMusic::size() const
{
    int size = 0;
    int tmpSize;

    DuHeader *header = dynamic_cast<DuHeader *>(getChild(KEY_MUSIC_HEADER));
    if (header == NULL)
        return -1;

    tmpSize = header->size();
    if (tmpSize == -1)
        return -1;

    size += tmpSize;

    DuSongInfo *songInfo = dynamic_cast<DuSongInfo *>(getChild(KEY_MUSIC_SONGINFO));
    if (songInfo == NULL)
        return -1;

    tmpSize = songInfo->size();
    if (tmpSize == -1)
        return -1;

    size += tmpSize;

    DuArray *tracks = dynamic_cast<DuArray *>(getChild(KEY_MUSIC_TRACKS));
    if (tracks == NULL)
        return -1;

    tmpSize = tracks->size();
    if (tmpSize == -1)
        return -1;

    size += tmpSize;

    return size;
}


DuHeader *DuMusic::getHeader()
{
    return dynamic_cast<DuHeader *>(getChild(KEY_MUSIC_HEADER));
}

void DuMusic::setHeader(DuHeader *header)
{
    if (getChild(KEY_MUSIC_HEADER) != NULL)
        delete getChild(KEY_MUSIC_HEADER);

    addChild(KEY_MUSIC_HEADER, header);
}

DuSongInfo *DuMusic::getSongInfo()
{
    return dynamic_cast<DuSongInfo *>(getChild(KEY_MUSIC_SONGINFO));
}

void DuMusic::setSongInfo(DuSongInfo *songInfo)
{
    if (getChild(KEY_MUSIC_SONGINFO) != NULL)
        delete getChild(KEY_MUSIC_SONGINFO);

    addChild(KEY_MUSIC_SONGINFO, songInfo);
}

DuArray *DuMusic::getTracks()
{
    return dynamic_cast<DuArray *>(getChild(KEY_MUSIC_TRACKS));
}

void DuMusic::setTracks(DuArray *array)
{
    if (getChild(KEY_MUSIC_TRACKS) != NULL)
        delete getChild(KEY_MUSIC_TRACKS);

    addChild(KEY_MUSIC_TRACKS, array);
}


bool DuMusic::appendTrack(DuTrack *track)
{
    DuArray *tmp = dynamic_cast<DuArray *>(getChild(KEY_MUSIC_TRACKS));

    if (tmp == NULL)
        return false;

    tmp->append(track);
    return true;
}
