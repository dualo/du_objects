#ifndef DUMIDIVARIABLELENGTH_H
#define DUMIDIVARIABLELENGTH_H

#include <QDataStream>
#include <QByteArray>

#include "../duvalue.h"


#define ONE_BYTE_MAX_UINT_VALUE 0x7F
#define TWO_BYTES_MAX_UINT_VALUE 0x3FFF
#define THREE_BYTES_MAX_UINT_VALUE 0x1FFFFF
#define FOUR_BYTES_MAX_UINT_VALUE 0x0FFFFFFF


DU_OBJECT(DuMidiVariableLength)

class DuMidiVariableLength : public DuValue
{
public:
    explicit DuMidiVariableLength(int offset = 0);
    virtual ~DuMidiVariableLength();

    QByteArray toDuMusicFile() const;
    QJsonValue toJson() const;

    int size() const;

    quint32 getTime(QDataStream &stream);
    quint32 getLength(QDataStream &stream);

    quint32 getAbsolute() const;
    quint32 getRelative() const;
    void setDelta(quint32 value);

    quint32 getOffset() const;
    void setOffset(quint32 value);

    const QByteArray toMidiFile() const;

private:
    quint32 offset;
};

#endif // DUMIDIVARIABLELENGTH_H
