#include "dumusic.h"

#include <cstring>

#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

#include "../midifile/dumidifile.h"


DU_OBJECT_IMPL(DuMusic)

DuMusic::DuMusic() :
    DuContainer(),
    m_databaseId(-1)
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


DuMusicPtr DuMusic::fromDuMusicBinary(const s_total_buffer &du_music, int fileSize)
{
    if (fileSize - MUSIC_SONG_SIZE !=
            du_music.local_song.s_totalsample * MUSIC_SAMPLE_SIZE)
    {
        qCritical() << "DuMusic::fromDuMusicBinary():\n"
                    << "failed to generate DuMusic\n"
                    << "invalid total event size";

        return DuMusicPtr();
    }

    DuMusicPtr music(new DuMusic);

    const DuHeaderPtr &header =
            DuHeader::fromDuMusicBinary(du_music.local_song);
    if (header != NULL)
        music->setHeader(header);
    else
    {
        qCritical() << "DuMusic::fromDuMusicBinary():\n"
                    << "failed to generate DuMusic\n"
                    << "the DuHeader was not properly generated";

        return DuMusicPtr();
    }

    const DuSongInfoPtr &songInfo =
            DuSongInfo::fromDuMusicBinary(du_music.local_song);
    if (songInfo != NULL)
        music->setSongInfo(songInfo);
    else
    {
        qCritical() << "DuMusic::fromDuMusicBinary():\n"
                    << "failed to generate DuMusic\n"
                    << "the DuSongInfo was not properly generated";

        return DuMusicPtr();
    }

    for (int i = 0; i < MUSIC_MAXTRACK; i++)
    {
        const DuTrackPtr &track =
                DuTrack::fromDuMusicBinary(du_music.local_song.s_track[i],
                                           du_music.local_buffer);
        if (track == NULL)
        {
            qCritical() << "DuMusic::fromDuMusicBinary():\n"
                        << "failed to generate DuMusic\n"
                        << "a DuTrack was not properly generated";

            return DuMusicPtr();
        }
        if (!music->appendTrack(track))
        {
            qCritical() << "DuMusic::fromDuMusicBinary():\n"
                        << "failed to generate DuMusic\n"
                        << "a DuTrack was not properly appended";

            return DuMusicPtr();
        }
    }

    return music;
}

DuMusicPtr DuMusic::fromDuMusicBinary(const music_song &du_song)
{
    DuMusicPtr music(new DuMusic);

    const DuHeaderPtr &header =
            DuHeader::fromDuMusicBinary(du_song);
    if (header != NULL)
        music->setHeader(header);
    else
    {
        qCritical() << "DuMusic::fromDuMusicBinary():\n"
                    << "failed to generate DuMusic\n"
                    << "the DuHeader was not properly generated";

        return DuMusicPtr();
    }

    const DuSongInfoPtr &songInfo =
            DuSongInfo::fromDuMusicBinary(du_song);
    if (songInfo != NULL)
        music->setSongInfo(songInfo);
    else
    {
        qCritical() << "DuMusic::fromDuMusicBinary():\n"
                    << "failed to generate DuMusic\n"
                    << "the DuSongInfo was not properly generated";

        return DuMusicPtr();
    }

    for (int i = 0; i < MUSIC_MAXTRACK; i++)
    {
        const DuTrackPtr &track =
                DuTrack::fromDuMusicBinary(du_song.s_track[i]);
        if (track == NULL)
        {
            qCritical() << "DuMusic::fromDuMusicBinary():\n"
                        << "failed to generate DuMusic\n"
                        << "a DuTrack was not properly generated";

            return DuMusicPtr();
        }
        if (!music->appendTrack(track))
        {
            qCritical() << "DuMusic::fromDuMusicBinary():\n"
                        << "failed to generate DuMusic\n"
                        << "a DuTrack was not properly appended";

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
    {
        qCritical() << "DuMusic::fromJson():\n"
                    << "failed to generate DuMusic\n"
                    << "a json key did not contain the proper type";

        return DuMusicPtr();
    }


    DuMusicPtr music(new DuMusic);

    const DuHeaderPtr &header = DuHeader::fromJson(jsonHeader.toObject());
    if (header != NULL)
        music->setHeader(header);
    else
    {
        qCritical() << "DuMusic::fromJson():\n"
                    << "failed to generate DuMusic\n"
                    << "the DuHeader was not properly generated";

        return DuMusicPtr();
    }

    const DuSongInfoPtr &songInfo = DuSongInfo::fromJson(jsonSongInfo.toObject());
    if (songInfo != NULL)
        music->setSongInfo(songInfo);
    else
    {
        qCritical() << "DuMusic::fromJson():\n"
                    << "failed to generate DuMusic\n"
                    << "the DuSongInfo was not properly generated";

        return DuMusicPtr();
    }

    const QJsonArray &jsonTrackArray = jsonTracks.toArray();
    if (jsonTrackArray.count() != MUSIC_MAXTRACK)
    {
        qCritical() << "DuMusic::fromJson():\n"
                    << "failed to generate DuMusic\n"
                    << "json file does not contain the proper amount of tracks";

        return DuMusicPtr();
    }

    for (int i = 0; i < jsonTrackArray.count(); i++)
    {
        const DuTrackPtr &track = DuTrack::fromJson(jsonTrackArray[i].toObject());
        if (track == NULL)
        {
            qCritical() << "DuMusic::fromJson():\n"
                        << "failed to generate DuMusic\n"
                        << "a DuTrack was not properly generated";

            return DuMusicPtr();
        }
        if (!music->appendTrack(track))
        {
            qCritical() << "DuMusic::fromJson():\n"
                        << "failed to generate DuMusic\n"
                        << "a DuTrack was not properly appended";

            return DuMusicPtr();
        }
    }

    return music;
}


DuMusicPtr DuMusic::fromBinary(const QByteArray &data)
{
    int sizeTest = data.size() - MUSIC_SONG_SIZE;
    if (sizeTest < 0 || sizeTest > RECORD_SAMPLEBUFFERSIZE)
    {
        qCritical() << "DuMusic::fromBinary():\n"
                    << "failed to generate DuMusic\n"
                    << "this file is not a dumusic file";

        return DuMusicPtr();
    }

    QScopedPointer<s_total_buffer> temp_total_buffer(new s_total_buffer);

    std::memcpy((char *)(temp_total_buffer.data()), data.data(), data.size());

    return DuMusic::fromDuMusicBinary(*temp_total_buffer, data.size());
}

DuMusicPtr DuMusic::fromBinary(QIODevice *input)
{
    QByteArray array = input->readAll();

    if (array.isEmpty())
    {
        qCritical() << "DuMusic::fromBinary():\n"
                    << "failed to generate DuMusic\n"
                    << "selected file could not be read";

        return DuMusicPtr();
    }

    return DuMusic::fromBinary(array);
}


QByteArray DuMusic::toDuMusicBinary() const
{
    QScopedPointer<s_total_buffer> du_music(new s_total_buffer);

    QByteArray tmpLocalSong;
    tmpLocalSong.clear();

    int musicSize = size();
    if (musicSize == -1)
        return QByteArray();

    QByteArray tmpClear(musicSize, (char)0x00);
    std::memcpy((char *)(du_music.data()), tmpClear.data(), musicSize);


    const DuHeaderConstPtr &header = getHeader();
    if (header == NULL)
        return QByteArray();
    const QByteArray &headerArray = header->toDuMusicBinary();
    if (headerArray.isNull())
        return QByteArray();

    const DuSongInfoConstPtr &songInfo = getSongInfo();
    if (songInfo == NULL)
        return QByteArray();
    const QByteArray &songInfoArray = songInfo->toDuMusicBinary();
    if (songInfoArray.isNull())
        return QByteArray();

    const DuArrayConstPtr &tracks = getTracks();
    if (tracks == NULL)
        return QByteArray();
    const QByteArray &tracksArray = tracks->toDuMusicBinary();
    if (tracksArray.isNull())
        return QByteArray();

    tmpLocalSong = headerArray.left(header->size())
            + songInfoArray.mid(header->size(), songInfo->size())
            + tracksArray;

    std::memcpy(&(du_music->local_song), tmpLocalSong.data(), MUSIC_SONG_SIZE);


    QByteArray tmpLocalBuffer;
    tmpLocalBuffer.clear();
    int eventTotal = 0;

    int trackCount = tracks->count();
    for (int i = 0; i < trackCount; i++)
    {
        const DuTrackConstPtr &track = tracks->at(i).dynamicCast<const DuTrack>();
        if (track == NULL)
            return QByteArray();

        const DuArrayConstPtr &loops = track->getLoops();
        if (loops == NULL)
            return QByteArray();

        int loopCount = loops->count();
        for (int j = 0; j < loopCount; j++)
        {
            const DuLoopConstPtr &loop = loops->at(j).dynamicCast<const DuLoop>();
            if (loop == NULL)
                return QByteArray();

            int tmp = loop->countEvents();
            if (tmp == -1)
                return QByteArray();

            music_loop *tmp_loop = &(du_music->local_song.s_track[i].t_loop[j]);
            tmp_loop->l_numsample = tmp;

            if (tmp > 0)
                tmp_loop->l_adress = eventTotal * MUSIC_SAMPLE_SIZE;
            else
                tmp_loop->l_adress = 0;

            const DuArrayConstPtr &events = loop->getEvents();
            if (events == NULL)
                return QByteArray();
            tmpLocalBuffer.append(events->toDuMusicBinary());

            eventTotal += tmp;
        }
    }

    du_music->local_song.s_totalsample = eventTotal;

    std::memcpy(du_music->local_buffer, tmpLocalBuffer.data(),
                eventTotal * MUSIC_SAMPLE_SIZE);


    return QByteArray((char *)(du_music.data()), musicSize);
}


QByteArray DuMusic::toMidiBinary() const
{
    const DuHeaderConstPtr &header = getHeader();
    if (header == NULL)
        return QByteArray();

    const DuSongInfoConstPtr &songInfo = getSongInfo();
    if (songInfo == NULL)
        return QByteArray();

    const DuArrayConstPtr &tracks = getTracks();

    if (tracks == NULL)
    {
        qCritical() << "DuMusic::toMidiBinary():\n"
                    << "could not retrieve track array";

        return QByteArray();
    }


    const QString &songName = header->getSongName();
    int tempo = songInfo->getTempo();
    int timeSig = songInfo->getTimeSignature();
    int tonality = songInfo->getTonality();
    int scale = songInfo->getScale();

    int durationRef = songInfo->getReferenceLoopDuration();
    if (durationRef == -1)
    {
        qCritical() << "DuMusic::toMidiBinary():\n"
                    << "this du-music doesn't have a reference loop duration";

        return QByteArray();
    }


    if (songName.isEmpty() || tempo == -1  ||  timeSig == -1
            ||  tonality == -1  || scale == -1)
    {
        qCritical() << "DuMusic::toMidiBinary():\n"
                    << "incorrect song data";

        return QByteArray();
    }


    DuMidiTrackPtr tempoTrack(new DuMidiTrack);

    DuMidiMetaEventPtr titleEvent(new DuMidiMetaEvent());

    titleEvent->setTitle(songName);
    tempoTrack->appendEvent(titleEvent);

    DuMidiMetaEventPtr tempoEvent(new DuMidiMetaEvent());

    tempoEvent->setTempo((quint8)tempo);
    tempoTrack->appendEvent(tempoEvent);

    if (timeSig > 0 && timeSig < 5)
    {
        DuMidiMetaEventPtr timeSigEvent(new DuMidiMetaEvent());

        timeSigEvent->setTimeSignature((quint8)timeSig + 1);
        tempoTrack->appendEvent(timeSigEvent);
    }

    if (scale > 0 && scale < 3)
    {
        DuMidiMetaEventPtr keySigEvent(new DuMidiMetaEvent());

        keySigEvent->setKeySignature(((quint8)tonality + 11) % 12, (scale == 2));
        tempoTrack->appendEvent(keySigEvent);
    }

    DuMidiMetaEventPtr eotEvent(new DuMidiMetaEvent());

    eotEvent->setEndOfTrack();
    tempoTrack->appendEvent(eotEvent);


    DuMidiFilePtr midiFile(new DuMidiFile());
    midiFile->appendTrack(tempoTrack);


    for (int i = 0; i < MUSIC_MAXTRACK; i++)
    {
        const DuTrackConstPtr &track = tracks->at(i).dynamicCast<const DuTrack>();
        if (track == NULL)
            return QByteArray();

        if (!midiFile->appendTracks(track->toDuMidiTrackArray(durationRef)))
            return QByteArray();
    }

    return midiFile->toMidiBinary();
}


int DuMusic::size() const
{
    int eventsSize = 0;
    int tmpSize = 0;

    const DuArrayConstPtr &tracks = getChildAs<DuArray>(KEY_MUSIC_TRACKS);
    if (tracks == NULL)
    {
        qDebug() << "tracks null";
        return -1;
    }

    int count = tracks->count();
    for (int i = 0; i < count; i++)
    {
        const DuTrackConstPtr &track = tracks->at(i).dynamicCast<const DuTrack>();
        if (track == NULL)
        {
            qDebug() << "track" << i << "null";
            return -1;
        }

        tmpSize = track->eventsSize();
        if (tmpSize == -1)
        {
            qDebug() << "track" << i << "size null";
            return -1;
        }

        eventsSize += tmpSize;
    }

    return eventsSize + MUSIC_SONG_SIZE;
}

bool DuMusic::isEmpty() const
{
    return size() <= MUSIC_SONG_SIZE;
}


int DuMusic::getDatabaseId() const
{
    return m_databaseId;
}

void DuMusic::setDatabaseId(int databaseId)
{
    m_databaseId = databaseId;
}

QStringList DuMusic::getLists() const
{
    return m_lists;
}

void DuMusic::setLists(const QStringList &lists)
{
    m_lists = lists;
}

QString DuMusic::getSongName() const
{
    const DuHeaderConstPtr &header = getChildAs<DuHeader>(KEY_MUSIC_HEADER);

    if (header == NULL)
    {
        return QString();
    }

    return header->getSongName();
}

bool DuMusic::setSongName(const QString &value)
{
    const DuHeaderPtr &header = getChildAs<DuHeader>(KEY_MUSIC_HEADER);

    if (header == NULL)
    {
        return false;
    }

    return header->setSongName(value);
}

int DuMusic::getFileVersion() const
{
    const DuHeaderConstPtr &header = getChildAs<DuHeader>(KEY_MUSIC_HEADER);

    if (header == NULL)
    {
        return -1;
    }

    return header->getFileVersion();
}

bool DuMusic::setFileVersion(int value)
{
    const DuHeaderPtr &header = getChildAs<DuHeader>(KEY_MUSIC_HEADER);

    if (header == NULL)
    {
        return false;
    }

    return header->setFileVersion(value);
}

DuHeaderConstPtr DuMusic::getHeader() const
{
    return getChildAs<DuHeader>(KEY_MUSIC_HEADER);
}

void DuMusic::setHeader(const DuHeaderPtr &header)
{
    addChild(KEY_MUSIC_HEADER, header);
}

DuSongInfoConstPtr DuMusic::getSongInfo() const
{
    return getChildAs<DuSongInfo>(KEY_MUSIC_SONGINFO);
}

void DuMusic::setSongInfo(const DuSongInfoPtr &songInfo)
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

    return tmp->append(track);
}
