#include "DuMetadataChunk.h"

#include <QDataStream>
#include <QDebug>

#include "Util/Util.h"

DuMetadataChunk::DuMetadataChunk(quint32 signature, quint32 version, const QByteArray& data) :
    m_signature(signature),
    m_version(version),
    m_data(data)
{
}

QMultiMap<quint32, DuMetadataChunk> DuMetadataChunk::parse(const QByteArray &data)
{
    QDataStream stream(data);
    stream.setByteOrder(QDataStream::LittleEndian);

    s_metadata_header generalHeader;
    stream.readRawData(reinterpret_cast<char*>(&generalHeader), METADATA_HEADER_SIZE);

    if (generalHeader.meta_signature != METADATA_SIGNATURE)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Metadata format error: doesn't start with expected root signature (" << Util::intToByteArray(METADATA_SIGNATURE) << ")";
        return {};
    }

    if (generalHeader.meta_version < METADATA_CURRENT_VERSION)
    {
        // migrate
    }

    if (generalHeader.meta_size != static_cast<quint32>(data.size() - METADATA_HEADER_SIZE))
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Metadata format error: size in general header incorrect"
                                      << "(total data size is" << data.size() << "while in header" << generalHeader.meta_size << "was read)";
        return {};
    }

    QMultiMap<quint32, DuMetadataChunk> chunks;
    while (!stream.atEnd())
    {
        s_metadata_header header;
        stream.readRawData(reinterpret_cast<char*>(&header), METADATA_HEADER_SIZE);

        QByteArray chunkData;
        chunkData.resize(header.meta_size);
        stream.readRawData(chunkData.data(), header.meta_size);

        chunks.insert(header.meta_signature, DuMetadataChunk(header.meta_signature, header.meta_version, chunkData));
    }

    return chunks;
}

quint32 DuMetadataChunk::signature() const
{
    return m_signature;
}

quint32 DuMetadataChunk::version() const
{
    return m_version;
}

QByteArray DuMetadataChunk::data() const
{
    return m_data;
}
