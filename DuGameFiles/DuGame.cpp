#include "DuGame.h"

#include <cstring>

#include <QDataStream>

#include <QtEndian>

#include "../dusoundfile/dusound.h"

DuGame::DuGame() : DuMusic()
{
}

DuGame::DuGame(const DuMusic &base) : DuMusic(base)
{
}

DuObjectPtr DuGame::clone() const
{
    return DuGamePtr(new DuGame(*this));
}

DuGamePtr DuGame::fromDuGameBinary(const QByteArray &data)
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

    QVector<DuSoundPtr> integratedSounds;
    QVector<DuSoundPtr> systemSounds;
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

            integratedSounds << sound;
        }
        else if (chunkType == "SDSM")
        {
            const DuSoundPtr& sound = DuSound::fromBinary(chunkData);
            if (sound == Q_NULLPTR)
                continue;

            systemSounds << sound;
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to parse DuMusic bundle: chunk type unknown:" << chunkType;
            return {};
        }
    }

    const int dataSize = musicData.size();
    const int totalSampleAndMetadataSize = dataSize - MUSIC_SONG_SIZE;

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

    const int totalSampleSize = temp_total_buffer->local_song.s_totalsample * MUSIC_SAMPLE_SIZE;

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

    const QByteArray& metadata = musicData.mid(static_cast<int>(temp_total_buffer->local_song.s_metadata));

    std::memcpy(temp_total_buffer->local_buffer, &(musicData.constData()[MUSIC_SONG_SIZE]), static_cast<size_t>(totalSampleSize));

    DuGamePtr newGame(new DuGame(*DuMusic::fromDuMusicBinary(*temp_total_buffer, MUSIC_SONG_SIZE + totalSampleSize, metadata)));
    newGame->m_integratedSounds = integratedSounds;
    newGame->m_systemSounds = systemSounds;

    return newGame;
}

DuGamePtr DuGame::fromDuGameBinary(QIODevice *input)
{
    const QByteArray& array = input->readAll();

    if (array.isEmpty())
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to generate DuGame\n"
                << "selected file could not be read";

        return {};
    }

    return DuGame::fromDuGameBinary(array);
}

QByteArray DuGame::toDuGameBinary() const
{
    QByteArray gameBinaryData;

    QDataStream stream(&gameBinaryData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream.writeRawData("DUGM", 4);
    stream << static_cast<quint32>(DU_GAME_STRUCT_CURRENT_VERSION);

    stream.writeRawData("DMSC", 4);

    const QByteArray& musicData = toDuMusicBinary();
    if (musicData.isEmpty())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Error converting du-music to binary";
        return QByteArray();
    }

    const int musicSize = musicData.size();
    stream << static_cast<quint32>(musicSize);
    stream.writeRawData(musicData.constData(), musicSize);
    stream << qChecksum(musicData.constData(), static_cast<uint>(musicSize));

    for (const DuSoundConstPtr& sound : m_integratedSounds)
    {
        stream.writeRawData("DSND", 4);

        const QByteArray& soundData = sound->toBinary(false);
        if (soundData.isEmpty())
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Error converting integrated du-sound to binary";
            return QByteArray();
        }

        const int soundSize = soundData.size();
        stream << static_cast<quint32>(soundSize);
        stream.writeRawData(soundData.constData(), soundSize);
        stream << qChecksum(soundData.constData(), static_cast<uint>(soundSize));
    }

    for (const DuSoundConstPtr& sound : m_systemSounds)
    {
        stream.writeRawData("SDSM", 4);

        const QByteArray& soundData = sound->toBinary(false);
        if (soundData.isEmpty())
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Error converting system du-sound to binary";
            return QByteArray();
        }

        const int soundSize = soundData.size();
        stream << static_cast<quint32>(soundSize);
        stream.writeRawData(soundData.constData(), soundSize);
        stream << qChecksum(soundData.constData(), static_cast<uint>(soundSize));
    }

    return gameBinaryData;
}

QVector<DuSoundPtr> DuGame::integratedSounds() const
{
    return m_integratedSounds;
}

QVector<DuSoundPtr> DuGame::systemSounds() const
{
    return m_systemSounds;
}
