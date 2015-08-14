#ifndef DUMIDIVARIABLELENGTH_H
#define DUMIDIVARIABLELENGTH_H

#include "../general/duvalue.h"


#define ONE_BYTE_MAX_UINT_VALUE 0x7F
#define TWO_BYTES_MAX_UINT_VALUE 0x3FFF
#define THREE_BYTES_MAX_UINT_VALUE 0x1FFFFF
#define FOUR_BYTES_MAX_UINT_VALUE 0x0FFFFFFF


DU_OBJECT(DuMidiVariableLength)

class DuMidiVariableLength : public DuValue
{
public:
    explicit DuMidiVariableLength(int offset = 0);
    ~DuMidiVariableLength();

    virtual DuObjectPtr clone() const;

    QByteArray toDuMusicBinary() const;
    QByteArray toMidiBinary() const;
    QJsonValue toJson() const;

    int size() const;

    quint32 getTime(QDataStream &stream);
    quint32 getLength(QDataStream &stream);

    quint32 getAbsolute() const;
    void setAbsolute(quint32 delta, quint32 offset = 0);
    void setAbsolute(QDataStream &stream, quint32 offset = 0);

    quint32 getRelative() const;
    void setRelative(quint32 value);

    quint32 getOffset() const;
    void setOffset(quint32 value);

private:
    quint32 offset;
};

#endif // DUMIDIVARIABLELENGTH_H
