#include "dumusic.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

DU_OBJECT_IMPL(DuMusic)

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

DuObjectPtr DuMusic::clone() const
{
    return DuMusicPtr(new DuMusic(*this));
}


DuMusicPtr DuMusic::fromDuMusicBinary(const s_total_buffer &du_music)
{
    DuMusicPtr music(new DuMusic);

    const DuHeaderPtr& header =
            DuHeader::fromDuMusicBinary(du_music.local_song);
    if (header != NULL)
        music->setHeader(header);
    else
    {
        return DuMusicPtr();
    }

    const DuSongInfoPtr& songInfo =
            DuSongInfo::fromDuMusicBinary(du_music.local_song);
    if (songInfo != NULL)
        music->setSongInfo(songInfo);
    else
    {
        return DuMusicPtr();
    }

    for (int i = 0; i < MUSIC_MAXTRACK; i++)
    {
        const DuTrackPtr track =
                DuTrack::fromDuMusicBinary(du_music.local_song.s_track[i],
                                           du_music.local_buffer);
        if (track == NULL)
        {
            return DuMusicPtr();
        }
        if (!music->appendTrack(track))
        {
            return DuMusicPtr();
        }
    }

    return music;
}


DuMusicPtr DuMusic::fromJson(const QJsonObject &jsonMusic)
{
    QJsonValue jsonHeader   = jsonMusic[KEY_MUSIC_HEADER];
    QJsonValue jsonSongInfo = jsonMusic[KEY_MUSIC_SONGINFO];
    QJsonValue jsonTracks   = jsonMusic[KEY_MUSIC_TRACKS];

    if (        !jsonHeader.isObject()  ||  !jsonSongInfo.isObject()
            ||  !jsonTracks.isArray())

        return DuMusicPtr();


    DuMusicPtr music(new DuMusic);

    const DuHeaderPtr& header = DuHeader::fromJson(jsonHeader.toObject());
    if (header != NULL)
        music->setHeader(header);
    else
    {
        return DuMusicPtr();
    }

    const DuSongInfoPtr& songInfo = DuSongInfo::fromJson(jsonSongInfo.toObject());
    if (songInfo != NULL)
        music->setSongInfo(songInfo);
    else
    {
        return DuMusicPtr();
    }

    const QJsonArray &jsonTrackArray = jsonTracks.toArray();
    for (int i = 0; i < jsonTrackArray.count(); i++)
    {
        const DuTrackPtr& track = DuTrack::fromJson(jsonTrackArray[i].toObject());
        if (track == NULL)
        {
            return DuMusicPtr();
        }
        if (!music->appendTrack(track))
        {
            return DuMusicPtr();
        }
    }

    return music;
}


QByteArray DuMusic::toDuMusicBinary() const
{
    s_total_buffer du_music;

    QByteArray tmpLocalSong;
    tmpLocalSong.clear();

    int musicSize = size();
    if (musicSize == -1)
        return QByteArray();

    QByteArray tmpClear(musicSize, (char)0x00);
    std::memcpy((char *)&(du_music), tmpClear.data(), musicSize);


    const DuHeaderConstPtr& header = getHeader();
    if (header == NULL)
        return QByteArray();
    const QByteArray &headerArray = header->toDuMusicBinary();
    if (headerArray.isNull())
        return QByteArray();

    const DuSongInfoConstPtr& songInfo = getSongInfo();
    if (songInfo == NULL)
        return QByteArray();
    const QByteArray &songInfoArray = songInfo->toDuMusicBinary();
    if (songInfoArray.isNull())
        return QByteArray();

    const DuArrayConstPtr& tracks = getTracks();
    if (tracks == NULL)
        return QByteArray();
    const QByteArray &tracksArray = tracks->toDuMusicBinary();
    if (tracksArray.isNull())
        return QByteArray();

    tmpLocalSong = headerArray.left(header->size())
            + songInfoArray.mid(header->size(), songInfo->size())
            + tracksArray;

    std::memcpy(&(du_music.local_song), tmpLocalSong.data(), MUSIC_SONG_SIZE);


    QByteArray tmpLocalBuffer;
    tmpLocalBuffer.clear();
    int eventTotal = 0;

    int trackCount = tracks->count();
    for (int i = 0; i < trackCount; i++)
    {
        const DuTrackConstPtr& track = tracks->at(i).dynamicCast<const DuTrack>();
        if (track == NULL)
            return QByteArray();

        const DuArrayConstPtr& loops = track->getLoops();
        if (loops == NULL)
            return QByteArray();

        int loopCount = loops->count();
        for (int j = 0; j < loopCount; j++)
        {
            const DuLoopConstPtr& loop = loops->at(j).dynamicCast<const DuLoop>();
            if (loop == NULL)
                return QByteArray();

            int tmp = loop->countEvents();
            if (tmp == -1)
                return QByteArray();

            music_loop *tmp_loop = &(du_music.local_song.s_track[i].t_loop[j]);
            tmp_loop->l_numsample = tmp;

            if (tmp > 0)
                tmp_loop->l_adress = eventTotal * MUSIC_SAMPLE_SIZE;
            else
                tmp_loop->l_adress = 0;

            const DuArrayConstPtr& events = loop->getEvents();
            if (events == NULL)
                return QByteArray();
            tmpLocalBuffer.append(events->toDuMusicBinary());

            eventTotal += tmp;
        }
    }

    du_music.local_song.s_totalsample = eventTotal;

    std::memcpy(du_music.local_buffer, tmpLocalBuffer.data(),
                eventTotal * MUSIC_SAMPLE_SIZE);


    return QByteArray((char *)&(du_music), musicSize);
}


int DuMusic::size() const
{
    int eventsSize = 0;
    int tmpSize = 0;

    const DuArrayConstPtr& tracks = getChildAs<DuArray>(KEY_MUSIC_TRACKS);
    if (tracks == NULL)
        return -1;

    int count = tracks->count();
    for (int i = 0; i < count; i++)
    {
        const DuTrackConstPtr& track = tracks->at(i).dynamicCast<const DuTrack>();
        if (track == NULL)
            return -1;

        tmpSize = track->eventsSize();
        if (tmpSize == -1)
            return -1;

        eventsSize += tmpSize;
    }

    return eventsSize + MUSIC_SONG_SIZE;
}


DuHeaderConstPtr DuMusic::getHeader() const
{
    return getChildAs<DuHeader>(KEY_MUSIC_HEADER);
}

void DuMusic::setHeader(const DuHeaderPtr& header)
{
    addChild(KEY_MUSIC_HEADER, header);
}

DuSongInfoConstPtr DuMusic::getSongInfo() const
{
    return getChildAs<DuSongInfo>(KEY_MUSIC_SONGINFO);
}

void DuMusic::setSongInfo(const DuSongInfoPtr& songInfo)
{
    addChild(KEY_MUSIC_SONGINFO, songInfo);
}

DuArrayConstPtr DuMusic::getTracks() const
{
    return getChildAs<DuArray>(KEY_MUSIC_TRACKS);
}

void DuMusic::setTracks(const DuArrayPtr &array)
{
    addChild(KEY_MUSIC_TRACKS, array);
}

bool DuMusic::appendTrack(const DuTrackPtr &track)
{
    DuArrayPtr tmp = getChildAs<DuArray>(KEY_MUSIC_TRACKS);

    if (tmp == NULL)
        return false;

    tmp->append(track);
    return true;
}
