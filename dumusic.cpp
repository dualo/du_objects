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


QByteArray DuMusic::toDuMusicFile() const
{
    s_total_buffer du_music;

    QByteArray tmpArray;
    tmpArray.clear();

    int musicSize = size();
    if (musicSize == -1)
        return QByteArray();

    QByteArray tmpClear(musicSize, (char)0x00);
    std::memcpy((char *)&(du_music), tmpClear.data(), musicSize);


    DuHeader *header = getHeader();
    if (header == NULL)
        return QByteArray();
    const QByteArray &headerArray = header->toDuMusicFile();
    if (headerArray.isNull())
        return QByteArray();

    DuSongInfo *songInfo = getSongInfo();
    if (songInfo == NULL)
        return QByteArray();
    const QByteArray &songInfoArray = songInfo->toDuMusicFile();
    if (songInfoArray.isNull())
        return QByteArray();

    DuArray *tracks = getTracks();
    if (tracks == NULL)
        return QByteArray();
    const QByteArray &tracksArray = tracks->toDuMusicFile();
    if (tracksArray.isNull())
        return QByteArray();

    tmpArray = headerArray.left(header->size())
            + songInfoArray.mid(header->size(), songInfo->size())
            + tracksArray;

    std::memcpy(&(du_music.local_song), tmpArray.data(), MUSIC_SONG_SIZE);


    tmpArray.clear();
    music_sample_p* eventTotal = 0;

    int trackCount = tracks->count();
    for (int i = 0; i < trackCount; i++)
    {
        DuTrack *track = dynamic_cast<DuTrack *>(tracks->at(i));
        if (track == NULL)
            return QByteArray();

        DuArray *loops = track->getLoops();
        if (loops == NULL)
            return QByteArray();

        int loopCount = loops->count();
        for (int j = 0; j < loopCount; j++)
        {
            DuLoop *loop = dynamic_cast<DuLoop *>(loops->at(j));
            if (loop == NULL)
                return QByteArray();

            int tmp = loop->countEvents();
            if (tmp == -1)
                return QByteArray();

            if (tmp > 0)
            {
                music_loop *tmp_loop = &(du_music.local_song.s_track[i].t_loop[j]);

                tmp_loop->l_numsample = tmp;
                tmp_loop->l_adress = eventTotal;

                eventTotal += tmp;
            }
        }
    }


    return QByteArray((char *)&(du_music), musicSize);
}


int DuMusic::size() const
{
    int eventsSize = 0;
    int tmpSize = 0;

    DuArray *tracks = dynamic_cast<DuArray *>(getChild(KEY_MUSIC_TRACKS));
    if (tracks == NULL)
        return -1;

    int count = tracks->count();
    for (int i = 0; i < count; i++)
    {
        DuTrack *track = dynamic_cast<DuTrack *>(tracks->at(i));
        if (track == NULL)
            return -1;

        tmpSize = track->eventsSize();
        if (tmpSize == -1)
            return -1;

        eventsSize += tmpSize;
    }

    return eventsSize + MUSIC_SONG_SIZE;
}


DuHeader *DuMusic::getHeader() const
{
    return dynamic_cast<DuHeader *>(getChild(KEY_MUSIC_HEADER));
}

void DuMusic::setHeader(DuHeader *header)
{
    if (getChild(KEY_MUSIC_HEADER) != NULL)
        delete getChild(KEY_MUSIC_HEADER);

    addChild(KEY_MUSIC_HEADER, header);
}

DuSongInfo *DuMusic::getSongInfo() const
{
    return dynamic_cast<DuSongInfo *>(getChild(KEY_MUSIC_SONGINFO));
}

void DuMusic::setSongInfo(DuSongInfo *songInfo)
{
    if (getChild(KEY_MUSIC_SONGINFO) != NULL)
        delete getChild(KEY_MUSIC_SONGINFO);

    addChild(KEY_MUSIC_SONGINFO, songInfo);
}

DuArray *DuMusic::getTracks() const
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
