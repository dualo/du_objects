#include "DuMusicMetadata.h"

#include "../DuGameFiles/DuGameMetadata.h"
#include "../metadata/DuMetadataChunk.h"

#include <QDataStream>

DU_OBJECT_IMPL(DuMusicMetadata)

DuMusicMetadata::DuMusicMetadata() : DuContainer()
{
    addChild(KeyGameMetadata, Q_NULLPTR);
}

DuMusicMetadataPtr DuMusicMetadata::fromBinary(const QByteArray &data)
{
    const QMultiMap<quint32, DuMetadataChunk>& chunks = DuMetadataChunk::parse(data);

    DuMusicMetadataPtr metadata(new DuMusicMetadata);

    if (chunks.contains(MUSICMETADATA_GAME_SIGNATURE))
    {
        if (chunks.count(MUSICMETADATA_GAME_SIGNATURE) > 1)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Several GAME metadata chunks found";
            return {};
        }

        const DuMetadataChunk& gameChunk = chunks.value(MUSICMETADATA_GAME_SIGNATURE);
        DuGameMetadataPtr game = DuGameMetadata::fromBinary(gameChunk.data(), gameChunk.version());
        if (game == Q_NULLPTR)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Can't parse music metadata: game is corrupted";
            return {};
        }

        metadata->setGameMetadata(game);
    }

    return metadata;
}

DuObjectPtr DuMusicMetadata::clone() const
{
    return DuMusicMetadataPtr(new DuMusicMetadata(*this));
}

QByteArray DuMusicMetadata::toDuMusicBinary() const
{
    if (size() == 0)
        return QByteArray();

    QByteArray data;

    s_metadata_header generalHeader{METADATA_SIGNATURE, METADATA_CURRENT_VERSION, static_cast<quint32>(size())};

    data += QByteArray(reinterpret_cast<const char*>(&generalHeader), METADATA_HEADER_SIZE);

    const DuGameMetadataConstPtr& game = getGameMetadata();
    if (game != Q_NULLPTR)
    {
        s_metadata_header header{MUSICMETADATA_GAME_SIGNATURE, MUSICMETADATA_GAME_CURRENT_VERSION, static_cast<quint32>(game->size())};
        data += QByteArray(reinterpret_cast<const char*>(&header), METADATA_HEADER_SIZE);
        data += game->toDuMusicBinary();
    }

    return data;
}

int DuMusicMetadata::size() const
{
    int size = 0;

    const DuGameMetadataConstPtr& game = getGameMetadata();
    if (game != Q_NULLPTR)
    {
        size += METADATA_HEADER_SIZE + game->size();
    }

    return size;
}

DU_KEY_ACCESSORS_OBJECT_IMPL(DuMusicMetadata, GameMetadata, DuGameMetadata)
