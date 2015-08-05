#include "dumusic.h"

#include <cstring>

#include <QIODevice>
#include <QJsonArray>
#include <QJsonObject>

#include "../midifile/dumidifile.h"


DU_OBJECT_IMPL(DuMusic)

DuMusic::DuMusic() :
    DuContainer(),
    m_databaseId(-1)
{
    addChild(KEY_MUSIC_HEADER, new DuHeader);

    addChild(KEY_MUSIC_CONTROLLERS, new DuControllers);

    addChild(KEY_MUSIC_SONGINFO, new DuSongInfo);

    addChild(KEY_MUSIC_REVERB, new DuReverb);

    addChild(KEY_MUSIC_TRACKS, new DuArray(MUSIC_MAXTRACK));

    addChild(KEY_MUSIC_TRANSPOSE,
             new DuNumeric(RECORD_TRANSPOSEDEFAULT, NUMERIC_DEFAULT_SIZE,
                           RECORD_TRANSPOSEMAX, RECORD_TRANSPOSEDEFAULT));
}

DuMusic::~DuMusic()
{
}

DuObjectPtr DuMusic::clone() const
{
    return DuMusicPtr(new DuMusic(*this));
}


DuMusicPtr DuMusic::fromDuMusicBinary(s_total_buffer &du_music, int fileSize)
{
    if (!upgrade(du_music))
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuMusic::fromDuMusicBinary():\n"
                                     << "failed to generate DuMusic\n"
                                     << "upgrade failed";
    }

    if (fileSize - MUSIC_SONG_SIZE !=
            du_music.local_song.s_totalsample * MUSIC_SAMPLE_SIZE)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMusic::fromDuMusicBinary():\n"
                    << "failed to generate DuMusic\n"
                    << "invalid total event size :"
                    << fileSize - MUSIC_SONG_SIZE << "!=" << du_music.local_song.s_totalsample * MUSIC_SAMPLE_SIZE;

        return DuMusicPtr();
    }

    DuMusicPtr music(new DuMusic);

    const DuHeaderPtr &header =
            DuHeader::fromDuMusicBinary(du_music.local_song);
    if (header != NULL)
        music->setHeader(header);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMusic::fromDuMusicBinary():\n"
                    << "failed to generate DuMusic\n"
                    << "the DuHeader was not properly generated";

        return DuMusicPtr();
    }

    const DuControllersPtr &controllers =
            DuControllers::fromDuMusicBinary(du_music.local_song);
    if (controllers != NULL)
        music->setControllers(controllers);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMusic::fromDuMusicBinary():\n"
                    << "failed to generate DuMusic\n"
                    << "the DuControllers was not properly generated";

        return DuMusicPtr();
    }

    const DuSongInfoPtr &songInfo =
            DuSongInfo::fromDuMusicBinary(du_music.local_song);
    if (songInfo != NULL)
        music->setSongInfo(songInfo);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMusic::fromDuMusicBinary():\n"
                    << "failed to generate DuMusic\n"
                    << "the DuSongInfo was not properly generated";

        return DuMusicPtr();
    }

    const DuReverbPtr &reverb =
            DuReverb::fromDuMusicBinary(du_music.local_song.s_reverb);
    if (reverb != NULL)
        music->setReverb(reverb);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMusic::fromDuMusicBinary():\n"
                    << "failed to generate DuMusic\n"
                    << "the DuReverb was not properly generated";

        return DuMusicPtr();
    }


    uint fileSampleSize = fileSize - MUSIC_SONG_SIZE;

    if (fileSampleSize % MUSIC_SAMPLE_SIZE != 0)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                << "DuMusic::fromDuMusicBinary():\n"
                << "file size (without header) is not a multiple of MUSIC_SAMPLE_SIZE\n"
                << "fileSampleSize =" << fileSampleSize << "\n"
                << "MUSIC_SAMPLE_SIZE =" << MUSIC_SAMPLE_SIZE;
    }

    uint nbSamples = fileSampleSize / MUSIC_SAMPLE_SIZE;

    if (nbSamples != du_music.local_song.s_totalsample)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                << "DuMusic::fromDuMusicBinary():\n"
                << "Nb samples calculated from file size different from s_totalsample value in header\n"
                << "s_totalsample =" << du_music.local_song.s_totalsample << "\n"
                << "nbSamples =" << nbSamples;
    }

    if (nbSamples > RECORD_SAMPLEBUFFERSIZE)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuMusic::fromDuMusicBinary():\n"
                << "Nb samples greater than maximum possible nb samples\n"
                << "RECORD_SAMPLEBUFFERSIZE =" << RECORD_SAMPLEBUFFERSIZE << "\n"
                << "nbSamples =" << nbSamples;

        return DuMusicPtr();
    }


    for (int i = 0; i < MUSIC_MAXTRACK; i++)
    {
        const DuTrackPtr &track =
                DuTrack::fromDuMusicBinary(du_music.local_song.s_track[i],
                                           du_music.local_buffer,
                                           nbSamples);
        if (track == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "DuMusic::fromDuMusicBinary():\n"
                                          << "failed to generate DuMusic\n"
                                          << "a DuTrack was not properly generated";

            return DuMusicPtr();
        }
        if (!music->appendTrack(track))
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "DuMusic::fromDuMusicBinary():\n"
                                          << "failed to generate DuMusic\n"
                                          << "a DuTrack was not properly appended";

            return DuMusicPtr();
        }
    }


    bool verif = music->setTranspose(du_music.local_song.s_transpose);

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                << "DuMusic::fromDuMusicBinary():\n"
                << "KEY_MUSIC_TRANSPOSE was not properly set";
    }


    return music;
}


DuMusicPtr DuMusic::fromBinary(const QByteArray &data)
{
    int dataSize = data.size() ;
    if (dataSize - MUSIC_SONG_SIZE < 0
            || dataSize - MUSIC_SONG_SIZE > RECORD_SAMPLEBUFFERSIZE * MUSIC_SAMPLE_SIZE)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMusic::fromBinary():\n"
                                      << "failed to generate DuMusic\n"
                                      << "this file is not a valid dumusic file";

        return DuMusicPtr();
    }

    QScopedPointer<s_total_buffer> temp_total_buffer(new s_total_buffer);

    std::memcpy((char *)(temp_total_buffer.data()), data.data(), dataSize);

    return DuMusic::fromDuMusicBinary(*temp_total_buffer, dataSize);
}

DuMusicPtr DuMusic::fromBinary(QIODevice *input)
{
    QByteArray array = input->readAll();

    if (array.isEmpty())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMusic::fromBinary():\n"
                                      << "failed to generate DuMusic\n"
                                      << "selected file could not be read";

        return DuMusicPtr();
    }

    return DuMusic::fromBinary(array);
}


DuMusicPtr DuMusic::fromJson(const QJsonObject &jsonMusic)
{
    QJsonValue jsonHeader       = jsonMusic[KEY_MUSIC_HEADER];
    QJsonValue jsonControllers  = jsonMusic[KEY_MUSIC_CONTROLLERS];
    QJsonValue jsonSongInfo     = jsonMusic[KEY_MUSIC_SONGINFO];
    QJsonValue jsonReverb       = jsonMusic[KEY_MUSIC_REVERB];
    QJsonValue jsonTracks       = jsonMusic[KEY_MUSIC_TRACKS];
    QJsonValue jsonTranspose    = jsonMusic[KEY_MUSIC_TRANSPOSE];

    if (        !jsonHeader.isObject()      ||  !jsonControllers.isObject()
            ||  !jsonSongInfo.isObject()    ||  !jsonReverb.isObject()
            ||  !jsonTracks.isArray()       ||  !jsonTranspose.isDouble())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMusic::fromJson():\n"
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
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMusic::fromJson():\n"
                                      << "failed to generate DuMusic\n"
                                      << "the DuHeader was not properly generated";

        return DuMusicPtr();
    }

    const DuControllersPtr &controllers =
            DuControllers::fromJson(jsonControllers.toObject());
    if (controllers != NULL)
        music->setControllers(controllers);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMusic::fromJson():\n"
                    << "failed to generate DuMusic\n"
                    << "the DuControllers was not properly generated";

        return DuMusicPtr();
    }

    const DuSongInfoPtr &songInfo = DuSongInfo::fromJson(jsonSongInfo.toObject());
    if (songInfo != NULL)
        music->setSongInfo(songInfo);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMusic::fromJson():\n"
                                      << "failed to generate DuMusic\n"
                                      << "the DuSongInfo was not properly generated";

        return DuMusicPtr();
    }

    const DuReverbPtr &reverb = DuReverb::fromJson(jsonReverb.toObject());
    if (reverb != NULL)
        music->setReverb(reverb);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMusic::fromJson():\n"
                    << "failed to generate DuMusic\n"
                    << "the DuReverb was not properly generated";

        return DuMusicPtr();
    }

    const QJsonArray &jsonTrackArray = jsonTracks.toArray();
    if (jsonTrackArray.count() != MUSIC_MAXTRACK)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuMusic::fromJson():\n"
                << "failed to generate DuMusic\n"
                << "json file does not contain the proper amount of tracks";

        return DuMusicPtr();
    }

    for (int i = 0; i < jsonTrackArray.count(); i++)
    {
        const DuTrackPtr &track = DuTrack::fromJson(jsonTrackArray[i].toObject());
        if (track == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "DuMusic::fromJson():\n"
                                          << "failed to generate DuMusic\n"
                                          << "a DuTrack was not properly generated";

            return DuMusicPtr();
        }
        if (!music->appendTrack(track))
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "DuMusic::fromJson():\n"
                                          << "failed to generate DuMusic\n"
                                          << "a DuTrack was not properly appended";

            return DuMusicPtr();
        }
    }


    bool verif = music->setTranspose(jsonTranspose.toInt());

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                << "DuMusic::fromJson():\n"
                << "KEY_MUSIC_TRANSPOSE was not properly set";
    }


    return music;
}


DuMusicPtr DuMusic::fromMidi(const MidiConversionHelper &helper)
{
    if (!helper.isValid())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMusic::fromMidi():\n"
                                      << "failed to generate DuMusic\n"
                                      << "invalid conversion helper";

        return DuMusicPtr();
    }

    DuMusicPtr music(new DuMusic);

    const DuSongInfoPtr &songInfo = DuSongInfo::fromMidi(helper);
    if (songInfo != NULL)
        music->setSongInfo(songInfo);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMusic::fromMidi():\n"
                                      << "failed to generate DuMusic\n"
                                      << "the DuSongInfo was not properly generated";

        return DuMusicPtr();
    }

    for (int i = 0; i < MUSIC_MAXTRACK; i++)
    {
        const DuTrackPtr &track = DuTrack::fromMidi(helper, i);
        if (track == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "DuMusic::fromMidi():\n"
                                          << "failed to generate DuMusic\n"
                                          << "a DuTrack was not properly generated";

            return DuMusicPtr();
        }
        if (!music->appendTrack(track))
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "DuMusic::fromMidi():\n"
                                          << "failed to generate DuMusic\n"
                                          << "a DuTrack was not properly appended";

            return DuMusicPtr();
        }
    }

    return music;
}

bool DuMusic::upgrade(s_total_buffer &du_music)
{
    music_song &song = du_music.local_song;

    if (song.s_version_music <= 0)
    {
        return false;
    }
    else if (song.s_version_music > VERSION_MUSIC)
    {
        return false;
    }
    else if (song.s_version_music == VERSION_MUSIC)
    {
        return true;
    }

    if (song.s_version_music == 1)
    {
        int32_t i, j, k;
        song.s_size = MUSIC_SONG_SIZE + (song.s_totalsample * MUSIC_SAMPLE_SIZE);
        song.s_metadata = 0;
        song.s_playhead = 0;
        song.s_transpose = RECORD_TRANSPOSEDEFAULT;

        song.s_reverb_preset = FX_REVERB_PRESET_DEFAULTVALUE;

        song.s_direction_gyro_P = -1;
        song.s_direction_gyro_R = -1;
        song.s_direction_gyro_Y = -1;
        song.s_activ_aftertouch = 0;
        song.s_activ_slider_L = 0;
        song.s_activ_slider_R = 0;
        song.s_activ_gyro_P = 0;
        song.s_activ_gyro_R = 0;
        song.s_activ_gyro_Y = 0;


        for (i = 0; i < MUSIC_MAXTRACK; i++)
        {
            for (j = 0; j < MUSIC_MAXLAYER; j++)
            {
                music_loop& loop = song.s_track[i].t_loop[j];

                if (loop.l_state == REC_EMPTY)
                {
                    // Loop empty
                    continue;
                }

                if (loop.l_adress % MUSIC_SAMPLE_SIZE != 0)
                {
                    qCCritical(LOG_CAT_DU_OBJECT) << "Fucked up du-music:\n"
                                                  << "l_adress is not a multiple of MUSIC_SAMPLE_SIZE\n"
                                                  << "l_adress =" << loop.l_adress << "\n"
                                                  << "MUSIC_SAMPLE_SIZE =" << MUSIC_SAMPLE_SIZE;
                    return false;
                }

                music_sample_p firstSampleIndex = loop.l_adress / MUSIC_SAMPLE_SIZE;
                if ((firstSampleIndex) + loop.l_numsample > RECORD_SAMPLEBUFFERSIZE)
                {
                    qCCritical(LOG_CAT_DU_OBJECT) << "Fucked up du-music:\n"
                                                  << "sample adresses point outside sample array\n"
                                                  << "firstSampleIndex =" << firstSampleIndex << "\n"
                                                  << "l_numsample =" << loop.l_numsample << "\n"
                                                  << "RECORD_SAMPLEBUFFERSIZE =" << RECORD_SAMPLEBUFFERSIZE;
                    return false;
                }

                // PRESETS
                preset_instr& preset = loop.l_instr.i_preset;

                preset.s_arpegiator_type = 0;
                preset.s_arpegiator_beat = 0;

                preset.s_direction_gyro_P = -1;
                preset.s_direction_gyro_R = -1;
                preset.s_direction_gyro_Y = -1;

                preset.s_adsr_onoff = 0;
                preset.s_compressor_onoff = 0;
                preset.s_delay_onoff = 0;
                preset.s_distortion_onoff = 0;
                preset.s_eq_onoff = 0;
                preset.s_chorus_onoff = 0;
                preset.s_vibrato_onoff = 0;
                preset.s_wah_onoff = 0;

                preset.s_autopitch_rate = 0;
                preset.s_autopitch_range = 127;

                preset.s_tremolo_rate = 0;
                preset.s_tremolo_range = 127;

                preset.s_autopan_rate = 0;
                preset.s_autopan_range = 127;

                preset.s_autowah_rate = 0;
                preset.s_autowah_range = 127;

                preset.s_multinote_act = 0;
                preset.s_multinote[0] = 0;
                preset.s_multinote[1] = 0;
                preset.s_multinote[2] = 0;
                preset.s_multinote[3] = 0;

                // INSTR
                s_instr& instr = loop.l_instr.i_instrument;

                if (instr.instr_key_map)
                    instr.instr_type = INSTR_PERCU;
                else
                    instr.instr_type = INSTR_HARMONIC;

                // SAMPLES
                music_sample *played_buffer = &du_music.local_buffer[firstSampleIndex];
                for (k = 0; k < loop.l_numsample; k++)
                {
                    played_buffer[k].canal = played_buffer[k].canal & 0xF7; // remove canals 8 -> 15 not used (and initialized anymore)
                    if (loop.l_instr.i_instrument.instr_type == INSTR_HARMONIC) // harmonic instrument -> remove octave from sample
                    {
                        if (played_buffer[k].control == 0 || played_buffer[k].control == 1)
                        {
                            played_buffer[k].note -= (12 * preset.s_instr_octave) + (song.s_transpose - RECORD_TRANSPOSEMAX);
                        }
                    }
                }
            }
        }
    }

    song.s_version_music = VERSION_MUSIC;

    return true;
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

    const DuControllersConstPtr &controllers = getControllers();
    if (controllers == NULL)
        return QByteArray();
    const QByteArray &controllersArray = controllers->toDuMusicBinary();
    if (controllersArray.isNull())
        return QByteArray();

    const DuSongInfoConstPtr &songInfo = getSongInfo();
    if (songInfo == NULL)
        return QByteArray();
    const QByteArray &songInfoArray = songInfo->toDuMusicBinary();
    if (songInfoArray.isNull())
        return QByteArray();

    const DuReverbConstPtr &reverb = getReverb();
    if (reverb == NULL)
        return QByteArray();
    const QByteArray &reverbArray = reverb->toDuMusicBinary();
    if (reverbArray.isNull())
        return QByteArray();

    const DuArrayConstPtr &tracks = getTracks();
    if (tracks == NULL)
        return QByteArray();
    const QByteArray &tracksArray = tracks->toDuMusicBinary();
    if (tracksArray.isNull())
        return QByteArray();

    tmpLocalSong =
            headerArray + controllersArray + songInfoArray + reverbArray + tracksArray;

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

    int tmpNum = getTranspose();
    if (tmpNum == -1)
        return QByteArray();
    du_music->local_song.s_transpose = tmpNum;


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
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMusic::toMidiBinary():\n"
                    << "could not retrieve track array";

        return QByteArray();
    }


    const QString &songName = songInfo->getSongName();
    int tempo = songInfo->getTempo();
    int timeSig = songInfo->getTimeSignature();
    int tonality = songInfo->getTonality();
    int scale = songInfo->getScale();

    int durationRef = songInfo->getReferenceLoopDuration();
    if (durationRef == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMusic::toMidiBinary():\n"
                    << "this du-music doesn't have a reference loop duration";

        return QByteArray();
    }


    if (songName.isEmpty() || tempo == -1  ||  timeSig == -1
            ||  tonality == -1  || scale == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMusic::toMidiBinary():\n"
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
        qCDebug(LOG_CAT_DU_OBJECT) << "tracks null";
        return -1;
    }

    int count = tracks->count();
    for (int i = 0; i < count; i++)
    {
        const DuTrackConstPtr &track = tracks->at(i).dynamicCast<const DuTrack>();
        if (track == NULL)
        {
            qCDebug(LOG_CAT_DU_OBJECT) << "track" << i << "null";
            return -1;
        }

        tmpSize = track->eventsSize();
        if (tmpSize == -1)
        {
            qCDebug(LOG_CAT_DU_OBJECT) << "track" << i << "size null";
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


int DuMusic::databaseId() const
{
    return m_databaseId;
}

void DuMusic::setDatabaseId(int databaseId)
{
    m_databaseId = databaseId;
}

QStringList DuMusic::lists() const
{
    return m_lists;
}

void DuMusic::setLists(const QStringList &lists)
{
    m_lists = lists;
}

QString DuMusic::getSongName() const
{
    const DuSongInfoConstPtr &songInfo = getChildAs<DuSongInfo>(KEY_MUSIC_SONGINFO);

    if (songInfo == NULL)
    {
        return QString();
    }

    return songInfo->getSongName();
}

bool DuMusic::setSongName(const QString &value)
{
    const DuSongInfoPtr &songInfo = getChildAs<DuSongInfo>(KEY_MUSIC_SONGINFO);

    if (songInfo == NULL)
    {
        return false;
    }

    return songInfo->setSongName(value);
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

DuControllersConstPtr DuMusic::getControllers() const
{
    return getChildAs<DuControllers>(KEY_MUSIC_CONTROLLERS);
}

void DuMusic::setControllers(const DuControllersPtr &controllers)
{
    addChild(KEY_MUSIC_CONTROLLERS, controllers);
}

DuSongInfoConstPtr DuMusic::getSongInfo() const
{
    return getChildAs<DuSongInfo>(KEY_MUSIC_SONGINFO);
}

void DuMusic::setSongInfo(const DuSongInfoPtr &songInfo)
{
    addChild(KEY_MUSIC_SONGINFO, songInfo);
}

DuReverbConstPtr DuMusic::getReverb() const
{
    return getChildAs<DuReverb>(KEY_MUSIC_REVERB);
}

void DuMusic::setReverb(const DuReverbPtr &reverb)
{
    addChild(KEY_MUSIC_REVERB, reverb);
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


int DuMusic::getTranspose() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_MUSIC_TRANSPOSE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuMusic::setTranspose(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_MUSIC_TRANSPOSE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}
