#include "dumusic.h"

#include "duevent.h"
#include "duloop.h"
#include "DuMusicMetadata.h"
#include "dutrack.h"

#include <cstring>

#include <QtEndian>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonObject>

#include "../DuGameFiles/DuGameMetadata.h"

#include "../dusoundfile/dusound.h"

#include "../general/duarray.h"
#include "../general/dubinarydata.h"
#include "../general/dunumeric.h"
#include "../general/dustring.h"

#include "../instrument/DuSystemSoundIdentifier.h"

#include "../instrument/effects/dumixer.h"
#include "../instrument/effects/dureverb.h"

#include "../midifile/dumidifile.h"
#include "../midifile/dumidimetaevent.h"
#include "../midifile/dumiditrack.h"

#ifndef NO_MIDI_IMPORT
#include "../miditodumusic/midiconversionhelper.h"
#endif

extern "C"
{
#pragma pack(push, 4)
#include "../du-touch/dualo_structs/music_migration.h"
#include "../du-touch/dualo_structs/metadata_structs.h"
#pragma pack(pop)
}


DU_OBJECT_IMPL(DuMusic)

DuMusic::DuMusic() :
    DuContainer(),
    m_databaseId(-1),
    m_indexInDevice(-1),
    m_isFromCommunity(false)
{
    addChild(KeyFileVersion, new DuNumeric(VERSION_MUSIC));

    addChild(KeyOriginalSerialNumber,  new DuString(MUSIC_SONG_OWNER_STR_SIZE));
    addChild(KeyOriginalName,          new DuString(MUSIC_SONG_OWNER_STR_SIZE));
    addChild(KeyOriginalUser,          new DuString(MUSIC_SONG_OWNER_STR_SIZE));
    addChild(KeyOriginalUserId,        new DuString(MUSIC_SONG_OWNER_STR_SIZE));
    addChild(KeyLastModifSerialNumber, new DuString(MUSIC_SONG_OWNER_STR_SIZE));
    addChild(KeyLastModifName,         new DuString(MUSIC_SONG_OWNER_STR_SIZE));
    addChild(KeyLastModifUser,         new DuString(MUSIC_SONG_OWNER_STR_SIZE));
    addChild(KeyLastModifUserId,       new DuString(MUSIC_SONG_OWNER_STR_SIZE));

    addChild(KeyPlayhead,  new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));
    addChild(KeyTranspose,
             new DuNumeric(RECORD_TRANSPOSEDEFAULT, NUMERIC_DEFAULT_SIZE,
                           2 * RECORD_TRANSPOSEDEFAULT, 0));
    addChild(KeyState,     new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));

    const DuNumericPtr& directionGyroP = DuNumericPtr(new DuNumeric(DIRECTIONCONTROLER_MINVALUE, NUMERIC_DEFAULT_SIZE,
                                                                    DIRECTIONCONTROLER_MAXVALUE, DIRECTIONCONTROLER_MINVALUE));
    directionGyroP->setForbiddenValues(QVector<int>() << 0);
    addChild(KeyDirectionGyroP, directionGyroP);


    const DuNumericPtr& directionGyroR = DuNumericPtr(new DuNumeric(DIRECTIONCONTROLER_MINVALUE, NUMERIC_DEFAULT_SIZE,
                                                                    DIRECTIONCONTROLER_MAXVALUE, DIRECTIONCONTROLER_MINVALUE));
    directionGyroR->setForbiddenValues(QVector<int>() << 0);
    addChild(KeyDirectionGyroR, directionGyroR);


    const DuNumericPtr& directionGyroY = DuNumericPtr(new DuNumeric(DIRECTIONCONTROLER_MINVALUE, NUMERIC_DEFAULT_SIZE,
                                                                    DIRECTIONCONTROLER_MAXVALUE, DIRECTIONCONTROLER_MINVALUE));
    directionGyroY->setForbiddenValues(QVector<int>() << 0);
    addChild(KeyDirectionGyroY, directionGyroY);

    addChild(KeyActiveAftertouch, new DuNumeric(0x0000, 2, 0xFFFF, 0x0000));
    addChild(KeyActiveSliderL,    new DuNumeric(0x0000, 2, 0xFFFF, 0x0000));
    addChild(KeyActiveSliderR,    new DuNumeric(0x0000, 2, 0xFFFF, 0x0000));
    addChild(KeyActiveGyroP,      new DuNumeric(0x0000, 2, 0xFFFF, 0x0000));
    addChild(KeyActiveGyroR,      new DuNumeric(0x0000, 2, 0xFFFF, 0x0000));
    addChild(KeyActiveGyroY,      new DuNumeric(0x0000, 2, 0xFFFF, 0x0000));

    addChild(KeySongId,      new DuNumeric(0));
    addChild(KeySongName,    new DuString(MUSIC_SONG_NAME_SIZE));
    addChild(KeySongVersion, new DuNumeric(0));

    addChild(KeyVolume,
             new DuNumeric(MUSIC_VOL_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           MUSIC_VOL_MAXVALUE, MUSIC_VOL_MINVALUE));
    addChild(KeyTempo,
             new DuNumeric(MUSIC_TEMPO_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           MUSIC_TEMPO_MAXVALUE, MUSIC_TEMPO_MINVALUE));
    addChild(KeyClickVolume,
             new DuNumeric(MUSIC_TEMPOVOL_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           MUSIC_TEMPOVOL_MAXVALUE, MUSIC_TEMPOVOL_MINVALUE));
    addChild(KeyReferenceTrack,        new DuNumeric(0, NUMERIC_DEFAULT_SIZE, MUSIC_MAXTRACK, 0));
    addChild(KeyCurrentTrack,          new DuNumeric(0, NUMERIC_DEFAULT_SIZE, MUSIC_MAXTRACK, 0));
    addChild(KeyOffset,                new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));
    addChild(KeyReferenceLoopDuration, new DuNumeric(0));
    addChild(KeyLeds,                  new DuBinaryData(NUM_LED_VALUE));

    addChild(KeyMixer, new DuMixer());

    addChild(KeySwing,          new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));
    addChild(KeyQuantification, new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));
    addChild(KeyScale,
             new DuNumeric(MAJOR_LED_MODE, NUMERIC_DEFAULT_SIZE,
                           NUM_LED_MODE - 1, NONE_LED_MODE));
    addChild(KeyTonality,       new DuNumeric(1, NUMERIC_DEFAULT_SIZE, 23, 0));
    addChild(KeyTimeSignature,
             new DuNumeric(TIME_4_4, NUMERIC_DEFAULT_SIZE,
                           NUM_TIMESIGNATURE - 1, TIME_OFF));

    addChild(KeyReverbPreset, new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0x7F, 0x00));
    addChild(KeyReverb,       new DuReverb);

    DuArrayPtr<DuTrack> trackArray(new DuArray<DuTrack>(MUSIC_MAXTRACK));
    for (int i = 0; i < MUSIC_MAXTRACK; ++i)
        trackArray->append(new DuTrack);
    addChild(KeyTracks, trackArray);

    addChild(KeyMetadata, Q_NULLPTR);
}

DuObjectPtr DuMusic::clone() const
{
    return DuMusicPtr(new DuMusic(*this));
}


DuMusicPtr DuMusic::fromDuMusicBinary(s_total_buffer &du_music, int totalBufferSize, const QByteArray &metadataBin)
{
    // Migration
    MUSIC_MIGRATION_ERROR result = static_cast<MUSIC_MIGRATION_ERROR>(migrate_music(&du_music));
    switch (result)
    {
        case MUSIC_MIGRATION_SUCCESS:
            break;
        case MUSIC_MIGRATION_FAIL:
        {
            qCWarning(LOG_CAT_DU_OBJECT)
                    << "Failed to generate DuMusic\n"
                    << "upgrade failed for unknown reason";
            break;
        }
        case MUSIC_MIGRATION_NULL_PTR:
        {
            qCWarning(LOG_CAT_DU_OBJECT)
                    << "Failed to generate DuMusic\n"
                    << "upgrade failed: null pointer found";
            break;
        }
        case MUSIC_MIGRATION_INVALID_VERSION:
        {
            qCWarning(LOG_CAT_DU_OBJECT)
                    << "Failed to generate DuMusic\n"
                    << "upgrade failed: invalid version";
            break;
        }
        case MUSIC_MIGRATION_TOO_RECENT:
        {
            qCWarning(LOG_CAT_DU_OBJECT)
                    << "Failed to generate DuMusic\n"
                    << "upgrade failed: version greater than" << VERSION_MUSIC;
            break;
        }
        case MUSIC_MIGRATION_BAD_LADRESS:
        {
            qCWarning(LOG_CAT_DU_OBJECT)
                    << "Failed to generate DuMusic\n"
                    << "upgrade failed: l_adress is not a multiple of MUSIC_SAMPLE_SIZE";
            break;
        }
        case MUSIC_MIGRATION_BAD_SAMPLE_ADRESS:
        {
            qCWarning(LOG_CAT_DU_OBJECT)
                    << "Failed to generate DuMusic\n"
                    << "upgrade failed: sample adresses point outside sample array";
            break;
        }
    }

    const music_song& local_song = du_music.local_song;

    // Check total buffer struct
    uint totalSampleSize = static_cast<uint>(totalBufferSize - MUSIC_SONG_SIZE);

    if (totalSampleSize != local_song.s_totalsample * MUSIC_SAMPLE_SIZE)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to generate DuMusic\n"
                << "invalid total event size :"
                << totalSampleSize << "!="
                << local_song.s_totalsample * MUSIC_SAMPLE_SIZE;

        return DuMusicPtr();
    }

    if (totalSampleSize % MUSIC_SAMPLE_SIZE != 0)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                << "File size (without header) is"
                << "not a multiple of MUSIC_SAMPLE_SIZE\n"
                << "fileSampleSize =" << totalSampleSize << "\n"
                << "MUSIC_SAMPLE_SIZE =" << MUSIC_SAMPLE_SIZE;
    }

    uint nbSamples = totalSampleSize / MUSIC_SAMPLE_SIZE;

    if (nbSamples != local_song.s_totalsample)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                << "Nb samples calculated from file size"
                << "different from s_totalsample value in header\n"
                << "s_totalsample =" << local_song.s_totalsample << "\n"
                << "nbSamples =" << nbSamples;
    }

    if (nbSamples > RECORD_SAMPLEBUFFERSIZE)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Nb samples greater than maximum possible nb samples\n"
                << "RECORD_SAMPLEBUFFERSIZE =" << RECORD_SAMPLEBUFFERSIZE << "\n"
                << "nbSamples =" << nbSamples;

        return DuMusicPtr();
    }


    DuMusicPtr music(new DuMusic);

    const DuMixerPtr &mixer = DuMixer::fromDuMusicBinary(local_song.s_mix);
    if (mixer == Q_NULLPTR)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to generate DuMusic\n"
                << "the DuMixer was not properly generated";

        return DuMusicPtr();
    }
    music->setMixer(mixer);

    const DuReverbPtr &reverb = DuReverb::fromDuMusicBinary(local_song.s_reverb);
    if (reverb == Q_NULLPTR)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to generate DuMusic\n"
                << "the DuReverb was not properly generated";

        return DuMusicPtr();
    }
    music->setReverb(reverb);


    DuArrayPtr<DuTrack> trackArray(new DuArray<DuTrack>(MUSIC_MAXTRACK));
    for (int i = 0; i < MUSIC_MAXTRACK; ++i)
    {
        const DuTrackPtr &track = DuTrack::fromDuMusicBinary(local_song.s_track[i],
                                                             du_music.local_buffer,
                                                             nbSamples);
        if (track == Q_NULLPTR)
        {
            qCCritical(LOG_CAT_DU_OBJECT)
                    << "Failed to generate DuMusic\n"
                    << "the DuTrack" << i << "was not properly generated";

            return DuMusicPtr();
        }
        if (!trackArray->append(track))
        {
            qCCritical(LOG_CAT_DU_OBJECT)
                    << "Failed to generate DuMusic\n"
                    << "the DuTrack" << i << "was not properly appended";

            return DuMusicPtr();
        }
    }
    music->setTracks(trackArray);


    if (!metadataBin.isEmpty())
    {
        const DuMusicMetadataPtr& metadata = DuMusicMetadata::fromBinary(metadataBin);
        if (metadata == Q_NULLPTR)
        {
            qCCritical(LOG_CAT_DU_OBJECT)
                    << "Failed to generate DuMusic\n"
                    << "the DuMusicMetadata was not properly generated";

            return DuMusicPtr();
        }
        music->setMetadata(metadata);
    }


    bool verif = true;

    verif = music->setFileVersion(static_cast<int>(local_song.s_version_music)) ? verif : false;

    verif = music->setOriginalSerialNumber(DuString::fromStruct(local_song.s_original_sn, MUSIC_SONG_OWNER_STR_SIZE)) ? verif : false;
    verif = music->setOriginalName(DuString::fromStruct(local_song.s_original_name, MUSIC_SONG_OWNER_STR_SIZE)) ? verif : false;
    verif = music->setOriginalUser(DuString::fromStruct(local_song.s_original_user, MUSIC_SONG_OWNER_STR_SIZE)) ? verif : false;
    verif = music->setOriginalUserId(DuString::fromStruct(local_song.s_original_userid, MUSIC_SONG_OWNER_STR_SIZE)) ? verif : false;

    verif = music->setLastModifSerialNumber(DuString::fromStruct(local_song.s_modif_sn, MUSIC_SONG_OWNER_STR_SIZE)) ? verif : false;
    verif = music->setLastModifName(DuString::fromStruct(local_song.s_modif_name, MUSIC_SONG_OWNER_STR_SIZE)) ? verif : false;
    verif = music->setLastModifUser(DuString::fromStruct(local_song.s_modif_user, MUSIC_SONG_OWNER_STR_SIZE)) ? verif : false;
    verif = music->setLastModifUserId(DuString::fromStruct(local_song.s_modif_userid, MUSIC_SONG_OWNER_STR_SIZE)) ? verif : false;

    verif = music->setPlayhead(local_song.s_playhead) ? verif : false;
    verif = music->setTranspose(local_song.s_transpose) ? verif : false;
    verif = music->setState(local_song.s_state) ? verif : false;

    verif = music->setDirectionGyroP(local_song.s_direction_gyro_P) ? verif : false;
    verif = music->setDirectionGyroR(local_song.s_direction_gyro_R) ? verif : false;
    verif = music->setDirectionGyroY(local_song.s_direction_gyro_Y) ? verif : false;
    verif = music->setActiveAftertouch(local_song.s_activ_aftertouch) ? verif : false;
    verif = music->setActiveSliderL(local_song.s_activ_slider_L) ? verif : false;
    verif = music->setActiveSliderR(local_song.s_activ_slider_R) ? verif : false;
    verif = music->setActiveGyroP(local_song.s_activ_gyro_P) ? verif : false;
    verif = music->setActiveGyroR(local_song.s_activ_gyro_R) ? verif : false;
    verif = music->setActiveGyroY(local_song.s_activ_gyro_Y) ? verif : false;

    verif = music->setSongId(local_song.s_id & 0x7FFFFFFF) ? verif : false;
    verif = music->setSongName(DuString::fromStruct(local_song.s_name, MUSIC_SONG_NAME_SIZE)) ? verif : false;

    // The MSB of s_version_song is used by the du-touch to know if the du-music has been read by du-station.
    // The version number is incremented by the du-touch ONLY IF the du-music
    // has been read by du-station since the last time the du-music was modified.
    // After the du-music has been read by du-station, the MSB is set to 1.
    // After the du-music has been modified, the MSB is set to 0.
    // Therefore, du-station ignores the MSB, hence the 0x7FFFFFFF mask.
    verif = music->setSongVersion(local_song.s_version_song & 0x7FFFFFFF) ? verif : false;

    verif = music->setVolume(local_song.s_volume) ? verif : false;
    verif = music->setTempo(local_song.s_tempo) ? verif : false;
    verif = music->setClickVolume(local_song.s_voltempo) ? verif : false;
    verif = music->setReferenceTrack(local_song.s_reftrack) ? verif : false;
    verif = music->setCurrentTrack(local_song.s_currenttrack) ? verif : false;
    verif = music->setOffset(local_song.s_decaltempo) ? verif : false;
    verif = music->setReferenceLoopDuration(static_cast<int>(local_song.s_looptimer)) ? verif : false;
    verif = music->setLeds(QByteArray(reinterpret_cast<const char*>(local_song.s_leds), NUM_LED_VALUE)) ? verif : false;

    verif = music->setSwing(local_song.s_swing) ? verif : false;
    verif = music->setQuantification(local_song.s_quantification) ? verif : false;
    verif = music->setScale(local_song.s_displaynote) ? verif : false;
    verif = music->setTonality(local_song.s_scaletonality) ? verif : false;
    verif = music->setTimeSignature(local_song.s_timesignature) ? verif : false;

    verif = music->setReverbPreset(local_song.s_reverb_preset) ? verif : false;


    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "An attribute was not properly set";
    }


    return music;
}


DuMusicPtr DuMusic::fromBinary(const QByteArray &data, QVector<DuSoundPtr> &outIntegratedSounds)
{
    QByteArray musicData;

    if (data.startsWith("DUMB"))
    {
        QDataStream stream(data);
        stream.setByteOrder(QDataStream::LittleEndian);
        stream.skipRawData(4);

        quint32 version;
        stream >> version;

        if (qFromBigEndian<quint32>(version) == 1)
        {
            stream.setByteOrder(QDataStream::BigEndian);
        }
        else if (version < DU_MUSIC_BUNDLE_STRUCT_CURRENT_VERSION)
        {
            // migrate
        }

        while (!stream.atEnd())
        {
            char* chunkTypeBuffer = new char[4];
            stream.readRawData(chunkTypeBuffer, 4);
            QByteArray chunkType(chunkTypeBuffer, 4);
            delete[] chunkTypeBuffer;

            quint32 chunkLength;
            stream >> chunkLength;

            char* chunkDataBuffer = new char[chunkLength];
            stream.readRawData(chunkDataBuffer, static_cast<int>(chunkLength));
            QByteArray chunkData(chunkDataBuffer, static_cast<int>(chunkLength));
            delete[] chunkDataBuffer;

            quint16 chunkCRC;
            stream >> chunkCRC;

            if (qChecksum(chunkData.constData(), chunkLength) != chunkCRC)
            {
                qCCritical(LOG_CAT_DU_OBJECT) << "Failed to parse DuMusic bundle: a chunk is corrupted (type:" << chunkType << ")";
                return DuMusicPtr();
            }

            if (chunkType == "DMSC")
            {
                musicData = chunkData;
            }
            else if (chunkType == "DSND")
            {
                const DuSoundPtr& sound = DuSound::fromBinary(chunkData);
                if (sound == Q_NULLPTR)
                    continue;

                outIntegratedSounds << sound;
            }
            else
            {
                qCCritical(LOG_CAT_DU_OBJECT) << "Failed to parse DuMusic bundle: chunk type unknown:" << chunkType;
                return DuMusicPtr();
            }
        }
    }
    else
    {
        musicData = data;
    }

    int dataSize = musicData.size();
    int totalSampleAndMetadataSize = dataSize - MUSIC_SONG_SIZE;

    if (totalSampleAndMetadataSize < 0)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to generate DuMusic\n"
                << "this file is not a valid dumusic file\n"
                << "total size too small:"
                << dataSize << "when should be >" << MUSIC_SONG_SIZE;

        return DuMusicPtr();
    }

    QScopedPointer<s_total_buffer> temp_total_buffer(new s_total_buffer);

    std::memcpy(reinterpret_cast<char*>(&(temp_total_buffer->local_song)), musicData.constData(), static_cast<size_t>(MUSIC_SONG_SIZE));

    QByteArray metadata;
    int totalSampleSize;
    if (temp_total_buffer->local_song.s_metadata != 0)
    {
        totalSampleSize = temp_total_buffer->local_song.s_totalsample * MUSIC_SAMPLE_SIZE;

        if (static_cast<int>(temp_total_buffer->local_song.s_metadata) != MUSIC_SONG_SIZE + totalSampleSize)
        {
            qCCritical(LOG_CAT_DU_OBJECT)
                    << "Failed to generate DuMusic\n"
                    << "this file is not a valid dumusic file\n"
                    << "s_metadata != size of du-music data without metadata:"
                    << temp_total_buffer->local_song.s_metadata << "!=" << MUSIC_SONG_SIZE + temp_total_buffer->local_song.s_totalsample * MUSIC_SAMPLE_SIZE;

            return DuMusicPtr();
        }

        metadata = musicData.mid(static_cast<int>(temp_total_buffer->local_song.s_metadata));
    }
    else
    {
        totalSampleSize = totalSampleAndMetadataSize;
    }

    if (totalSampleSize > RECORD_SAMPLEBUFFERSIZE * MUSIC_SAMPLE_SIZE)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to generate DuMusic\n"
                << "this file is not a valid dumusic file\n"
                << "total sample data size > max size:"
                << totalSampleAndMetadataSize << ">" << RECORD_SAMPLEBUFFERSIZE * MUSIC_SAMPLE_SIZE;

        return DuMusicPtr();
    }

    std::memcpy(temp_total_buffer->local_buffer, &(musicData.constData()[MUSIC_SONG_SIZE]), static_cast<size_t>(totalSampleSize));

    return DuMusic::fromDuMusicBinary(*temp_total_buffer, MUSIC_SONG_SIZE + totalSampleSize, metadata);
}

DuMusicPtr DuMusic::fromBinary(QIODevice *input, QVector<DuSoundPtr>& outIntegratedSounds)
{
    QByteArray array = input->readAll();

    if (array.isEmpty())
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to generate DuMusic\n"
                << "selected file could not be read";

        return DuMusicPtr();
    }

    return DuMusic::fromBinary(array, outIntegratedSounds);
}

DuMusicPtr DuMusic::fromDuGameBinary(const QByteArray &data, QVector<DuSoundPtr> &outIntegratedSounds, QVector<DuSoundPtr> &outSystemSounds)
{
    QByteArray musicData;

    if (!data.startsWith("DUGM"))
        return {};

    QDataStream stream(data);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream.skipRawData(4);

    quint32 version;
    stream >> version;

    if (qFromBigEndian<quint32>(version) == 1)
    {
        stream.setByteOrder(QDataStream::BigEndian);
    }
    else if (version < DU_GAME_STRUCT_CURRENT_VERSION)
    {
        // migrate
    }

    while (!stream.atEnd())
    {
        char* chunkTypeBuffer = new char[4];
        stream.readRawData(chunkTypeBuffer, 4);
        QByteArray chunkType(chunkTypeBuffer, 4);
        delete[] chunkTypeBuffer;

        quint32 chunkLength;
        stream >> chunkLength;

        char* chunkDataBuffer = new char[chunkLength];
        stream.readRawData(chunkDataBuffer, static_cast<int>(chunkLength));
        QByteArray chunkData(chunkDataBuffer, static_cast<int>(chunkLength));
        delete[] chunkDataBuffer;

        quint16 chunkCRC;
        stream >> chunkCRC;

        if (qChecksum(chunkData.constData(), chunkLength) != chunkCRC)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to parse DuMusic bundle: a chunk is corrupted (type:" << chunkType << ")";
            return {};
        }

        if (chunkType == "DMSC")
        {
            musicData = chunkData;
        }
        else if (chunkType == "DSND")
        {
            const DuSoundPtr& sound = DuSound::fromBinary(chunkData);
            if (sound == Q_NULLPTR)
                continue;

            outIntegratedSounds << sound;
        }
        else if (chunkType == "SDSM")
        {
            const DuSoundPtr& sound = DuSound::fromBinary(chunkData);
            if (sound == Q_NULLPTR)
                continue;

            outSystemSounds << sound;
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to parse DuMusic bundle: chunk type unknown:" << chunkType;
            return {};
        }
    }

    int dataSize = musicData.size();
    int totalSampleAndMetadataSize = dataSize - MUSIC_SONG_SIZE;

    if (totalSampleAndMetadataSize < 0)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to generate DuMusic\n"
                << "this file is not a valid dumusic file\n"
                << "total size too small:"
                << dataSize << "when should be >" << MUSIC_SONG_SIZE;

        return {};
    }

    QScopedPointer<s_total_buffer> temp_total_buffer(new s_total_buffer);

    std::memcpy(reinterpret_cast<char*>(&(temp_total_buffer->local_song)), musicData.constData(), static_cast<size_t>(MUSIC_SONG_SIZE));

    if (temp_total_buffer->local_song.s_metadata == 0)
        return {};

    int totalSampleSize = temp_total_buffer->local_song.s_totalsample * MUSIC_SAMPLE_SIZE;

    if (totalSampleSize > RECORD_SAMPLEBUFFERSIZE * MUSIC_SAMPLE_SIZE)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to generate DuMusic\n"
                << "this file is not a valid dumusic file\n"
                << "total sample data size > max size:"
                << totalSampleAndMetadataSize << ">" << RECORD_SAMPLEBUFFERSIZE * MUSIC_SAMPLE_SIZE;

        return {};
    }

    if (static_cast<int>(temp_total_buffer->local_song.s_metadata) != MUSIC_SONG_SIZE + totalSampleSize)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to generate DuMusic\n"
                << "this file is not a valid dumusic file\n"
                << "s_metadata != size of du-music data without metadata:"
                << temp_total_buffer->local_song.s_metadata << "!=" << MUSIC_SONG_SIZE + temp_total_buffer->local_song.s_totalsample * MUSIC_SAMPLE_SIZE;

        return {};
    }

    QByteArray metadata = musicData.mid(static_cast<int>(temp_total_buffer->local_song.s_metadata));

    std::memcpy(temp_total_buffer->local_buffer, &(musicData.constData()[MUSIC_SONG_SIZE]), static_cast<size_t>(totalSampleSize));

    return DuMusic::fromDuMusicBinary(*temp_total_buffer, MUSIC_SONG_SIZE + totalSampleSize, metadata);
}

DuMusicPtr DuMusic::fromDuGameBinary(QIODevice *input, QVector<DuSoundPtr> &outIntegratedSounds, QVector<DuSoundPtr> &outSystemSounds)
{
    QByteArray array = input->readAll();

    if (array.isEmpty())
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to generate DuMusic\n"
                << "selected file could not be read";

        return {};
    }

    return DuMusic::fromDuGameBinary(array, outIntegratedSounds, outSystemSounds);
}


DuMusicPtr DuMusic::fromJson(const QJsonObject &jsonMusic)
{
    DuMusicPtr music(new DuMusic);

    if (!music->parseJson(jsonMusic))
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to parse json:" << jsonMusic;
        return DuMusicPtr();
    }

    return music;
}


#ifndef NO_MIDI_IMPORT
DuMusicPtr DuMusic::fromMidi(const MidiConversionHelper &helper)
{
    if (!helper.isValid())
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to generate DuMusic\n"
                << "invalid conversion helper";

        return DuMusicPtr();
    }

    DuMusicPtr music(new DuMusic);

    bool verif = true;

    verif = music->setSongName(helper.getTitle()) ? verif : false;

    verif = music->setReferenceTrack(helper.getIndexes(0).first) ? verif : false;
    verif = music->setReferenceLoopDuration(helper.getDuration()) ? verif : false;
    verif = music->setCurrentTrack(helper.getIndexes(0).first) ? verif : false;

//    verif = music->setVolume() ? verif : false;
    verif = music->setTempo(helper.getTempo()) ? verif : false;
//    verif = music->setClickVolume() ? verif : false;
//    verif = music->setOffset(jsonOffset.toInt()) ? verif : false;

    verif = music->setScale(helper.getDutouchScale()) ? verif : false;
    verif = music->setTonality(helper.getTonality()) ? verif : false;
    verif = music->setTimeSignature(helper.getTimeSig()) ? verif : false;

//    verif = music->setLeds() ? verif : false;

    verif = music->setSwing(0) ? verif : false;
    verif = music->setQuantification(0) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                << "Failed to generate DuMusic\n"
                << "an attribute was not properly set";
    }

    //TODO: generate DuHeader (maybe)

    DuArrayPtr<DuTrack> trackArray(new DuArray<DuTrack>(MUSIC_MAXTRACK));
    for (int i = 0; i < MUSIC_MAXTRACK; i++)
    {
        const DuTrackPtr &track = DuTrack::fromMidi(helper, i);
        if (track == Q_NULLPTR)
        {
            qCCritical(LOG_CAT_DU_OBJECT)
                    << "DuMusic::fromMidi():\n"
                    << "failed to generate DuMusic\n"
                    << "a DuTrack was not properly generated";

            return DuMusicPtr();
        }
        if (!trackArray->append(track))
        {
            qCCritical(LOG_CAT_DU_OBJECT)
                    << "DuMusic::fromMidi():\n"
                    << "failed to generate DuMusic\n"
                    << "a DuTrack was not properly appended";

            return DuMusicPtr();
        }
    }
    music->setTracks(trackArray);

    if (music->size() > MUSIC_SONG_SIZE + RECORD_SAMPLEBUFFERSIZE * MUSIC_SAMPLE_SIZE)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuMusic::fromMidiBinary():\n"
                << "du-music size above max possible size"
                << music->size()
                << MUSIC_SONG_SIZE + RECORD_SAMPLEBUFFERSIZE * MUSIC_SAMPLE_SIZE;

        return DuMusicPtr();
    }

    return music;
}
#endif

QByteArray DuMusic::toDuMusicBinary() const
{
    QScopedPointer<s_total_buffer> du_music(new s_total_buffer);

    QByteArray tmpArray;
    QString tmpStr;
    int tmpNum = 0;

    int musicSize = size();
    if (musicSize == -1 || musicSize > MUSIC_SONG_SIZE + RECORD_SAMPLEBUFFERSIZE * MUSIC_SAMPLE_SIZE)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Invalid music size :" << musicSize
                << ", MUSIC_SONG_SIZE=" << MUSIC_SONG_SIZE;
        return QByteArray();
    }

    void* res = std::memset(du_music.data(), 0, MUSIC_SONG_SIZE + RECORD_SAMPLEBUFFERSIZE * MUSIC_SAMPLE_SIZE);
    Q_ASSERT(res != Q_NULLPTR);


    music_song& local_song = du_music->local_song;


    tmpNum = getFileVersion();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_version_music = static_cast<quint32>(tmpNum);


    tmpArray.fill(0x00, MUSIC_SONG_OWNER_STR_SIZE);
    tmpStr = getOriginalSerialNumber();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toLatin1());
    std::memcpy(local_song.s_original_sn, tmpArray.constData(), MUSIC_SONG_OWNER_STR_SIZE);

    tmpArray.fill(0x00, MUSIC_SONG_OWNER_STR_SIZE);
    tmpStr = getOriginalName();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toLatin1());
    std::memcpy(local_song.s_original_name, tmpArray.constData(), MUSIC_SONG_OWNER_STR_SIZE);

    tmpArray.fill(0x00, MUSIC_SONG_OWNER_STR_SIZE);
    tmpStr = getOriginalUser();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toLatin1());
    std::memcpy(local_song.s_original_user, tmpArray.constData(), MUSIC_SONG_OWNER_STR_SIZE);

    tmpArray.fill(0x00, MUSIC_SONG_OWNER_STR_SIZE);
    tmpStr = getOriginalUserId();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toLatin1());
    std::memcpy(local_song.s_original_userid, tmpArray.constData(), MUSIC_SONG_OWNER_STR_SIZE);

    tmpArray.fill(0x00, MUSIC_SONG_OWNER_STR_SIZE);
    tmpStr = getLastModifSerialNumber();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toLatin1());
    std::memcpy(local_song.s_modif_sn, tmpArray.constData(), MUSIC_SONG_OWNER_STR_SIZE);

    tmpArray.fill(0x00, MUSIC_SONG_OWNER_STR_SIZE);
    tmpStr = getLastModifName();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toLatin1());
    std::memcpy(local_song.s_modif_name, tmpArray.constData(), MUSIC_SONG_OWNER_STR_SIZE);

    tmpArray.fill(0x00, MUSIC_SONG_OWNER_STR_SIZE);
    tmpStr = getLastModifUser();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toLatin1());
    std::memcpy(local_song.s_modif_user, tmpArray.constData(), MUSIC_SONG_OWNER_STR_SIZE);

    tmpArray.fill(0x00, MUSIC_SONG_OWNER_STR_SIZE);
    tmpStr = getLastModifUserId();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toLatin1());
    std::memcpy(local_song.s_modif_userid, tmpArray.constData(), MUSIC_SONG_OWNER_STR_SIZE);


    tmpNum = size();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_size = static_cast<quint32>(tmpNum);


    tmpNum = getPlayhead();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_playhead = static_cast<quint8>(tmpNum);

    tmpNum = getTranspose();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_transpose = static_cast<quint8>(tmpNum);

    tmpNum = getState();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_state = static_cast<quint8>(tmpNum);


    tmpNum = getDirectionGyroP();
    if (tmpNum == 0)
        return QByteArray();
    local_song.s_direction_gyro_P = static_cast<qint8>(tmpNum);

    tmpNum = getDirectionGyroR();
    if (tmpNum == 0)
        return QByteArray();
    local_song.s_direction_gyro_R = static_cast<qint8>(tmpNum);

    tmpNum = getDirectionGyroY();
    if (tmpNum == 0)
        return QByteArray();
    local_song.s_direction_gyro_Y = static_cast<qint8>(tmpNum);

    tmpNum = getActiveAftertouch();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_activ_aftertouch = static_cast<quint16>(tmpNum);

    tmpNum = getActiveSliderL();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_activ_slider_L = static_cast<quint16>(tmpNum);

    tmpNum = getActiveSliderR();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_activ_slider_R = static_cast<quint16>(tmpNum);

    tmpNum = getActiveGyroP();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_activ_gyro_P = static_cast<quint16>(tmpNum);

    tmpNum = getActiveGyroR();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_activ_gyro_R = static_cast<quint16>(tmpNum);

    tmpNum = getActiveGyroY();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_activ_gyro_Y = static_cast<quint16>(tmpNum);


    tmpNum = getSongId();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_id = static_cast<quint32>(tmpNum);

    tmpArray.fill(0x00, MUSIC_SONG_NAME_SIZE);
    tmpStr = getSongName();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toLatin1());
    std::memcpy(local_song.s_name, tmpArray.constData(), MUSIC_SONG_NAME_SIZE);

    tmpNum = getSongVersion();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_version_song = static_cast<quint32>(tmpNum);


    tmpNum = getVolume();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_volume = static_cast<quint8>(tmpNum);

    tmpNum = getTempo();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_tempo = static_cast<quint8>(tmpNum);

    tmpNum = getClickVolume();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_voltempo = static_cast<quint8>(tmpNum);

    tmpNum = getReferenceTrack();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_reftrack = static_cast<quint8>(tmpNum);

    tmpNum = getCurrentTrack();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_currenttrack = static_cast<quint8>(tmpNum);

    tmpNum = getOffset();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_decaltempo = static_cast<quint8>(tmpNum);

    tmpNum = getReferenceLoopDuration();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_looptimer = static_cast<quint32>(tmpNum);

    tmpArray = getLeds();
    if (tmpArray.isNull())
        return QByteArray();
    std::memcpy(local_song.s_leds, tmpArray.constData(), NUM_LED_VALUE);


    const DuMixerConstPtr &mixer = getMixer();
    if (mixer == Q_NULLPTR)
        return QByteArray();
    const QByteArray &mixerArray = mixer->toDuMusicBinary();
    if (mixerArray.isNull())
        return QByteArray();
    std::memcpy(&(local_song.s_mix), mixerArray.constData(), static_cast<size_t>(mixerArray.size()));


    tmpNum = getSwing();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_swing = static_cast<quint8>(tmpNum);

    tmpNum = getQuantification();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_quantification = static_cast<quint8>(tmpNum);

    tmpNum = getScale();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_displaynote = static_cast<quint8>(tmpNum);

    tmpNum = getTonality();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_scaletonality = static_cast<quint8>(tmpNum);

    tmpNum = getTimeSignature();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_timesignature = static_cast<quint8>(tmpNum);


    tmpNum = getReverbPreset();
    if (tmpNum == -1)
        return QByteArray();
    local_song.s_reverb_preset = static_cast<quint8>(tmpNum);

    const DuReverbConstPtr &reverb = getReverb();
    if (reverb == Q_NULLPTR)
        return QByteArray();
    const QByteArray &reverbArray = reverb->toDuMusicBinary();
    if (reverbArray.isNull())
        return QByteArray();
    std::memcpy(&(local_song.s_reverb), reverbArray.constData(), static_cast<size_t>(reverbArray.size()));


    const DuArrayConstPtr<DuTrack> &tracks = getTracks();
    if (tracks == Q_NULLPTR)
        return QByteArray();
    const QByteArray &tracksArray = tracks->toDuMusicBinary();
    if (tracksArray.isNull())
        return QByteArray();
    std::memcpy(&(local_song.s_track), tracksArray.constData(), static_cast<size_t>(tracksArray.size()));


    QByteArray tmpLocalBuffer;
    tmpLocalBuffer.clear();
    int eventTotal = 0;

    int trackCount = tracks->count();
    for (int i = 0; i < trackCount; i++)
    {
        const DuTrackConstPtr &track =
                tracks->at(i).dynamicCast<const DuTrack>();
        if (track == Q_NULLPTR)
            return QByteArray();

        const DuArrayConstPtr<DuLoop> &loops = track->getLoops();
        if (loops == Q_NULLPTR)
            return QByteArray();

        int loopCount = loops->count();
        for (int j = 0; j < loopCount; j++)
        {
            const DuLoopConstPtr &loop =
                    loops->at(j).dynamicCast<const DuLoop>();
            if (loop == Q_NULLPTR)
                return QByteArray();

            int tmp = loop->countEvents();
            if (tmp == -1)
                return QByteArray();

            music_loop *tmp_loop = &(local_song.s_track[i].t_loop[j]);
            tmp_loop->l_numsample = static_cast<quint16>(tmp);

            if (tmp > 0)
                tmp_loop->l_adress = static_cast<quint32>(eventTotal) * MUSIC_SAMPLE_SIZE;
            else
                tmp_loop->l_adress = 0;

            const DuArrayConstPtr<DuEvent> &events = loop->getEvents();
            if (events == Q_NULLPTR)
                return QByteArray();
            tmpLocalBuffer.append(events->toDuMusicBinary());

            eventTotal += tmp;
        }
    }

    local_song.s_totalsample = static_cast<quint16>(eventTotal);

    const DuMusicMetadataConstPtr& metadata = getMetadata();
    QByteArray metadataBin;
    if (metadata == Q_NULLPTR)
    {
        local_song.s_metadata = 0;
    }
    else
    {
        local_song.s_metadata = MUSIC_SONG_SIZE + eventTotal * MUSIC_SAMPLE_SIZE;
        metadataBin = metadata->toDuMusicBinary();
    }

    std::memcpy(du_music->local_buffer, tmpLocalBuffer.constData(), static_cast<size_t>(eventTotal) * MUSIC_SAMPLE_SIZE);


    return QByteArray(reinterpret_cast<char*>(du_music.data()), MUSIC_SONG_SIZE + eventTotal * MUSIC_SAMPLE_SIZE) + metadataBin;
}

QByteArray DuMusic::toDuMusicBundleBinary(const QVector<DuSoundConstPtr> &integratedSounds) const
{
    QByteArray musicBinaryData;

    QDataStream stream(&musicBinaryData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream.writeRawData("DUMB", 4);
    stream << static_cast<quint32>(DU_MUSIC_BUNDLE_STRUCT_CURRENT_VERSION);

    stream.writeRawData("DMSC", 4);

    QByteArray musicData = toDuMusicBinary();
    if (musicData.isEmpty())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Error converting du-music to binary";
        return QByteArray();
    }

    int musicSize = musicData.size();
    stream << static_cast<quint32>(musicSize);
    stream.writeRawData(musicData.constData(), musicSize);
    stream << qChecksum(musicData.constData(), static_cast<uint>(musicSize));

    for (const DuSoundConstPtr& sound : integratedSounds)
    {
        stream.writeRawData("DSND", 4);

        const QByteArray& soundData = sound->toBinary(false);
        if (soundData.isEmpty())
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Error converting du-sound to binary";
            return QByteArray();
        }

        int soundSize = soundData.size();
        stream << static_cast<quint32>(soundSize);
        stream.writeRawData(soundData.constData(), soundSize);
        stream << qChecksum(soundData.constData(), static_cast<uint>(soundSize));
    }

    return musicBinaryData;
}

QByteArray DuMusic::toDuGameBinary(const QVector<DuSoundConstPtr> &integratedSounds, const QVector<DuSoundConstPtr> &systemSounds) const
{
    QByteArray musicBinaryData;

    QDataStream stream(&musicBinaryData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream.writeRawData("DUGM", 4);
    stream << static_cast<quint32>(DU_GAME_STRUCT_CURRENT_VERSION);

    stream.writeRawData("DMSC", 4);

    QByteArray musicData = toDuMusicBinary();
    if (musicData.isEmpty())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Error converting du-music to binary";
        return QByteArray();
    }

    int musicSize = musicData.size();
    stream << static_cast<quint32>(musicSize);
    stream.writeRawData(musicData.constData(), musicSize);
    stream << qChecksum(musicData.constData(), static_cast<uint>(musicSize));

    for (const DuSoundConstPtr& sound : integratedSounds)
    {
        stream.writeRawData("DSND", 4);

        const QByteArray& soundData = sound->toBinary(false);
        if (soundData.isEmpty())
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Error converting du-sound to binary";
            return QByteArray();
        }

        int soundSize = soundData.size();
        stream << static_cast<quint32>(soundSize);
        stream.writeRawData(soundData.constData(), soundSize);
        stream << qChecksum(soundData.constData(), static_cast<uint>(soundSize));
    }

    for (const DuSoundConstPtr& sound : systemSounds)
    {
        stream.writeRawData("SDSM", 4);

        const QByteArray& soundData = sound->toBinary(false);
        if (soundData.isEmpty())
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Error converting du-sound to binary";
            return QByteArray();
        }

        int soundSize = soundData.size();
        stream << static_cast<quint32>(soundSize);
        stream.writeRawData(soundData.constData(), soundSize);
        stream << qChecksum(soundData.constData(), static_cast<uint>(soundSize));
    }

    return musicBinaryData;
}


QByteArray DuMusic::toMidiBinary() const
{
    const DuArrayConstPtr<DuTrack> &tracks = getTracks();
    if (tracks == Q_NULLPTR)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
               << "DuMusic::toMidiBinary():\n"
               << "KEY_MUSIC_TRACKS is NULL";

        return QByteArray();
    }

    int transpose = getTranspose();
    if (transpose == -1)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                << "DuMusic::toMidiBinary():\n"
                << "incorrect KEY_MUSIC_TRANSPOSE value:" << transpose;

        transpose = RECORD_TRANSPOSEDEFAULT;
    }

    int durationRef = getReferenceLoopDuration();
    if (durationRef == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuMusic::toMidiBinary():\n"
                << "invalid reference loop duration";

        return QByteArray();
    }


    DuMidiTrackPtr tempoTrack = getTempoTrack();
    if (tempoTrack == Q_NULLPTR)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "tempo track error";
        return QByteArray();
    }

    DuMidiFilePtr midiFile(new DuMidiFile());
    midiFile->appendTrack(tempoTrack);


    for (int i = 0; i < MUSIC_MAXTRACK; i++)
    {
        const DuTrackConstPtr &track =
                tracks->at(i).dynamicCast<const DuTrack>();
        if (track == Q_NULLPTR)
        {
            qCCritical(LOG_CAT_DU_OBJECT)
                    << "DuMusic::toMidiBinary():\n"
                    << "DuTrack" << i << "is NULL";

            return QByteArray();
        }

        if (!midiFile->appendTracks(track->toDuMidiTrackArray(durationRef,
                                                              transpose)))
        {
            qCCritical(LOG_CAT_DU_OBJECT)
                    << "DuMusic::toMidiBinary():\n"
                    << "DuTrack" << i
                    << "was not successfully converted/appended";

            return QByteArray();
        }
    }

    return midiFile->toMidiBinary();
}

DuMidiTrackPtr DuMusic::getTempoTrack() const
{
    const QString &songName = getSongName();
    int tempo = getTempo();
    int timeSig = getTimeSignature();
    int tonality = getTonality();
    int scale = getScale();

    if (songName.isEmpty() || tempo == -1  ||  timeSig == -1
            ||  tonality == -1  || scale == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "invalid song data";
        return {};
    }

    DuMidiTrackPtr tempoTrack(new DuMidiTrack);

    DuMidiMetaEventPtr titleEvent(new DuMidiMetaEvent());

    titleEvent->setTitle(songName);
    tempoTrack->appendEvent(titleEvent);

    DuMidiMetaEventPtr tempoEvent(new DuMidiMetaEvent());

    tempoEvent->setTempo(static_cast<quint8>(tempo));
    tempoTrack->appendEvent(tempoEvent);

    if (timeSig > 0 && timeSig < 5)
    {
        DuMidiMetaEventPtr timeSigEvent(new DuMidiMetaEvent());

        timeSigEvent->setTimeSignature(static_cast<quint8>(timeSig) + 1);
        tempoTrack->appendEvent(timeSigEvent);
    }

    if (scale > 0 && scale < 3)
    {
        DuMidiMetaEventPtr keySigEvent(new DuMidiMetaEvent());

        keySigEvent->setKeySignature((static_cast<quint8>(tonality) + 11) % 12, (scale == 2));
        tempoTrack->appendEvent(keySigEvent);
    }

    DuMidiMetaEventPtr eotEvent(new DuMidiMetaEvent());

    eotEvent->setEndOfTrack();
    tempoTrack->appendEvent(eotEvent);

    return tempoTrack;
}

int DuMusic::size() const
{
    int eventsSize = 0;
    int tmpSize = 0;

    const DuArrayConstPtr<DuTrack> &tracks = getTracks();
    if (tracks == Q_NULLPTR)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "KEY_MUSIC_TRACKS is NULL";
        return -1;
    }

    int count = tracks->count();
    for (int i = 0; i < count; i++)
    {
        const DuTrackConstPtr &track = tracks->at(i);
        if (track == Q_NULLPTR)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "track" << i << "is NULL";
            return -1;
        }

        tmpSize = track->eventsSize();
        if (tmpSize == -1)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "track" << i << "size null";
            return -1;
        }

        eventsSize += tmpSize;
    }

    return eventsSize + MUSIC_SONG_SIZE + (getMetadata() != Q_NULLPTR ? METADATA_HEADER_SIZE + getMetadata()->size() : 0);
}

bool DuMusic::isEmpty() const
{
    return size() <= MUSIC_SONG_SIZE;
}

bool DuMusic::getIsDuGame() const
{
    const DuMusicMetadataConstPtr& metadata = getMetadata();
    if (metadata == Q_NULLPTR)
        return false;

    const DuGameMetadataConstPtr& game = metadata->getGameMetadata();

    return game != Q_NULLPTR;
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

bool DuMusic::appendTrack(const DuTrackPtr &track)
{
    DuArrayPtr<DuTrack> tmp = getChildAs< DuArray<DuTrack> >(KeyTracks);

    if (tmp == Q_NULLPTR)
        return false;

    return tmp->append(track);
}

QSet<InstrumentIdentifier> DuMusic::getUsedInstrumentsIdentifiers() const
{
    QSet<InstrumentIdentifier> returnedList;

    const DuArrayConstPtr<DuTrack>& tracks = getTracks();
    if (tracks == Q_NULLPTR)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Can't get du-music tracks";
        return QSet<InstrumentIdentifier>();
    }

    for (const DuTrackConstPtr& track : *(tracks))
    {
        const DuArrayConstPtr<DuLoop>& loops = track->getLoops();
        if (loops == Q_NULLPTR)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Can't get track's loops";
            continue;
        }

        for (const DuLoopConstPtr& loop : *(loops))
        {
            if (loop->getState() == REC_EMPTY)
            {
                continue;
            }

            int id = loop->getID();
            if (id == -1)
            {
                qCCritical(LOG_CAT_DU_OBJECT) << "Can't get loop's id";
                continue;
            }

            int userId = loop->getUserID();
            if (userId == -1)
            {
                qCCritical(LOG_CAT_DU_OBJECT) << "Can't get loop's user id";
                continue;
            }

            const QString& name = loop->getNameForDevice();
            if (name.isNull())
            {
                qCCritical(LOG_CAT_DU_OBJECT) << "Can't get loop's name";
                continue;
            }

            int version = loop->getInstrVersion();
            if (version == -1)
            {
                qCCritical(LOG_CAT_DU_OBJECT) << "Can't get loop's version";
                continue;
            }

            returnedList << InstrumentIdentifier{id, userId, name, version};
        }
    }

    return returnedList;
}

QSet<InstrumentIdentifier> DuMusic::getUsedSystemSoundsIdentifiers() const
{
    const DuMusicMetadataConstPtr& metadata = getMetadata();
    if (metadata == Q_NULLPTR)
    {
        return {};
    }

    const DuGameMetadataConstPtr& game = metadata->getGameMetadata();
    if (game == Q_NULLPTR)
    {
        return {};
    }

    const DuArrayConstPtr<DuSystemSoundIdentifier>& systemSounds = game->getSounds();
    if (systemSounds == Q_NULLPTR)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "game sounds array null";
        return {};
    }

    QSet<InstrumentIdentifier> returnedList;
    for (const DuSystemSoundIdentifierConstPtr& sound : *systemSounds)
    {
        if (sound == Q_NULLPTR)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "system sound identifier null";
            continue;
        }

        returnedList << InstrumentIdentifier{sound->getID(), sound->getUserID(), QString(), 0};
    }

    return returnedList;
}

int DuMusic::indexInDevice() const
{
    return m_indexInDevice;
}

void DuMusic::setIndexInDevice(int indexInDevice)
{
    m_indexInDevice = indexInDevice;
}

QString DuMusic::deviceSerialNumber() const
{
    return m_deviceSerialNumber;
}

void DuMusic::setDeviceSerialNumber(const QString &deviceSerialNumber)
{
    m_deviceSerialNumber = deviceSerialNumber;
}

bool DuMusic::getIsFromCommunity() const
{
    return m_isFromCommunity;
}

void DuMusic::setIsFromCommunity(bool isFromCommunity)
{
    m_isFromCommunity = isFromCommunity;
}

#define X(key, dutype, type, defaultReturn) DU_KEY_ACCESSORS_IMPL(DuMusic, key, dutype, type, defaultReturn)
#define X_OBJECT(key, dutype) DU_KEY_ACCESSORS_OBJECT_IMPL(DuMusic, key, dutype)
#define X_OBJECT_TEMPLATE(key, dutype, tpltype) DU_KEY_ACCESSORS_OBJECT_TEMPLATE_IMPL(DuMusic, key, dutype, tpltype)
DuMusic_Children
#undef X_OBJECT_TEMPLATE
#undef X_OBJECT
#undef X
