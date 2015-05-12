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
    DuMusic *music = new DuMusic();

    music->setHeader(DuHeader::fromDuMusicFile(du_music.local_song));
    music->setSongInfo(DuSongInfo::fromDuMusicFile(du_music.local_song));

    DuArray *tracks = music->getTracks();

    for (int i = 0; i < MUSIC_MAXTRACK; i++)
    {
        DuTrack *track = DuTrack::fromDuMusicFile(du_music.local_song.s_track[i],
                                                  du_music.local_buffer);

        if (track != NULL)
            tracks->append(track);
    }

    return music;
}


DuMusic *DuMusic::fromJson(const QJsonObject &jsonMusic)
{
    //TODO

    DuMusic *music = new DuMusic();
    const QStringList &keyList = music->keys();

    bool test = true;
    for (int i = 0; i < keyList.count(); i++)
    {
        test = test && jsonMusic.contains(keyList[i]);
    }

    if (!test)
        return NULL;

    music->setHeader(DuHeader::fromJson(jsonMusic[KEY_MUSIC_HEADER].toObject()));
    music->setSongInfo(DuSongInfo::fromJson(jsonMusic[KEY_MUSIC_SONGINFO].toObject()));

    DuArray *tracks = music->getTracks();
    const QJsonArray &jsonTracks = jsonMusic[KEY_MUSIC_TRACKS].toArray();

    for (int i = 0; i < jsonTracks.count(); i++)
    {
        DuTrack *track = DuTrack::fromJson(jsonTracks[i].toObject());
        if (track != NULL)
            tracks->append(track);
    }

    return music;
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
