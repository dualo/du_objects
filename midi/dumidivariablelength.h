#ifndef DUMIDIVARIABLELENGTH_H
#define DUMIDIVARIABLELENGTH_H

#include <QDataStream>
#include <QByteArray>


#define ONE_BYTE_MAX_UINT_VALUE 0x7F
#define TWO_BYTES_MAX_UINT_VALUE 0x3FFF
#define THREE_BYTES_MAX_UINT_VALUE 0x1FFFFF
#define FOUR_BYTES_MAX_UINT_VALUE 0x0FFFFFFF


class DuMidiVariableLength
{
public:
    static DuMidiVariableLength* getInstance();

    void resetTime(quint32 time = 0);

    quint32 getTime(QDataStream &stream);
    quint32 getLength(QDataStream &stream);

    QByteArray formattedTimeArray(quint32 value);
    QByteArray formattedLengthArray(quint32 value);
    quint32 formattedSize(quint32 value);

private:
    explicit DuMidiVariableLength();
    ~DuMidiVariableLength();

    quint32 previousTime;
    static DuMidiVariableLength *_instance;
};

#endif // DUMIDIVARIABLELENGTH_H
