#ifndef DUMETADATACHUNK_H
#define DUMETADATACHUNK_H

#include <QByteArray>
#include <QLoggingCategory>

#pragma pack(push, 4)
#include "../du-touch/dualo_structs/metadata_structs.h"
#pragma pack(pop)

Q_DECLARE_LOGGING_CATEGORY(LOG_CAT_DU_OBJECT)

class DuMetadataChunk
{
public:
    DuMetadataChunk() = default;
    DuMetadataChunk(quint32 signature, quint32 version, const QByteArray& data);

    static QMultiMap<quint32, DuMetadataChunk> parse(const QByteArray& data);

    quint32 signature() const;
    quint32 version() const;
    QByteArray data() const;

private:
    quint32 m_signature;
    quint32 m_version;
    QByteArray m_data;
};

#endif // DUMETADATACHUNK_H
