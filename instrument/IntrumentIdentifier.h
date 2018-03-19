#ifndef INTRUMENTIDENTIFIER_H
#define INTRUMENTIDENTIFIER_H

#include <QHash>
#include <QMetaType>

struct InstrumentIdentifier
{
    // ID and user ID identify the instrument.
    // If two instruments have the same id and user id, then they represent the same instrument.
    int id;
    int userId;

    // Following data may change between different versions of the same instrument.
    QString name;
    int version;
};

inline bool operator==(const InstrumentIdentifier &i1, const InstrumentIdentifier &i2)
{
    return i1.id == i2.id && i1.userId == i2.userId;
}

inline uint qHash(const InstrumentIdentifier &key, uint seed)
{
    return qHash(qMakePair(key.id, key.userId), seed);
}

Q_DECLARE_METATYPE(InstrumentIdentifier)

#endif // INTRUMENTIDENTIFIER_H
