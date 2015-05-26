#include "dumidivariablelength.h"

DuMidiVariableLength::DuMidiVariableLength(int offset) :
    offset(offset)
{
}

DuMidiVariableLength::~DuMidiVariableLength()
{
}


QByteArray DuMidiVariableLength::toDuMusicFile() const
{
    return QByteArray();
}

QJsonValue DuMidiVariableLength::toJson() const
{
    return QJsonValue();
}


int DuMidiVariableLength::size() const
{
    quint32 value = getValue().toUInt();

    if (value > FOUR_BYTES_MAX_UINT_VALUE)
        return -1;

    if (value > THREE_BYTES_MAX_UINT_VALUE)
        return 4;

    if (value > TWO_BYTES_MAX_UINT_VALUE)
        return 3;

    if (value > ONE_BYTE_MAX_UINT_VALUE)
        return 2;

    return 1;
}

/*
quint32 DuMidiVariableLength::getTime(QDataStream &stream)
{
    quint32 value = 0;
    quint8 tmp;

    quint32 value1 = 0;
    quint32 value2 = 0;
    quint32 value3 = 0;
    quint32 value4 = 0;

    stream >> tmp;
    value4 = tmp;

    if (tmp > 0x7F)
    {
        stream >> tmp;
        value3 = value4;
        value4 = tmp;

        if (tmp > 0x7F)
        {
            stream >> tmp;
            value2 = value3;
            value3 = value4;
            value4 = tmp;

            if (tmp > 0x7F)
            {
                stream >> tmp;
                value1 = value2;
                value2 = value3;
                value3 = value4;
                value4 = tmp;
            }
        }
    }

    value = ((value1 << 21) & 0x0FE00000) + ((value2 << 14) & 0x001FC000)
            + ((value3 << 7) & 0x00003F80) + value4;

    offset += value;

    setValue(offset);

    return offset;
}


quint32 DuMidiVariableLength::getLength(QDataStream &stream)
{
    quint32 value = 0;
    quint8 tmp;

    quint32 value1 = 0;
    quint32 value2 = 0;
    quint32 value3 = 0;
    quint32 value4 = 0;

    stream >> tmp;
    value4 = tmp;

    if (tmp > 0x7F)
    {
        stream >> tmp;
        value3 = value4;
        value4 = tmp;

        if (tmp > 0x7F)
        {
            stream >> tmp;
            value2 = value3;
            value3 = value4;
            value4 = tmp;

            if (tmp > 0x7F)
            {
                stream >> tmp;
                value1 = value2;
                value2 = value3;
                value3 = value4;
                value4 = tmp;
            }
        }
    }

    value = ((value1 << 21) & 0x0FE00000) + ((value2 << 14) & 0x001FC000)
            + ((value3 << 7) & 0x00003F80) + value4;

    setValue(value);

    return value;
}
*/

quint32 DuMidiVariableLength::getAbsolute() const
{
    return getOffset() + getValue().toInt();
}

quint32 DuMidiVariableLength::getRelative() const
{
    return getValue().toInt();
}

void DuMidiVariableLength::setDelta(quint32 value)
{
    setValue(value);
}


quint32 DuMidiVariableLength::getOffset() const
{
    return offset;
}

void DuMidiVariableLength::setOffset(quint32 value)
{
    offset = value;
}


const QByteArray DuMidiVariableLength::toMidiFile() const
{
    QByteArray array;
    array.clear();

    quint32 value = getValue().toInt();

    if (value > FOUR_BYTES_MAX_UINT_VALUE)
        return QByteArray();

    array.prepend((quint8)(value & 0x7F));

    if (value > ONE_BYTE_MAX_UINT_VALUE)
        array.prepend((quint8)(((value >> 7) & 0x7F) + 0x80));

    if (value > TWO_BYTES_MAX_UINT_VALUE)
        array.prepend((quint8)(((value >> 14) & 0x7F) + 0x80));

    if (value > THREE_BYTES_MAX_UINT_VALUE)
        array.prepend((quint8)(((value >> 21) & 0x7F) + 0x80));

    return array;
}

/*
QByteArray DuMidiVariableLength::formattedTimeArray(quint32 value)
{
    QByteArray array;
    array.clear();

    quint32 tmpValue = value - offset;
    offset = value;

    if (tmpValue > FOUR_BYTES_MAX_UINT_VALUE)
        return array;

    array.prepend((quint8)(tmpValue & 0x7F));

    if (tmpValue > ONE_BYTE_MAX_UINT_VALUE)
        array.prepend((quint8)(((tmpValue >> 7) & 0x7F) + 0x80));

    if (tmpValue > TWO_BYTES_MAX_UINT_VALUE)
        array.prepend((quint8)(((tmpValue >> 14) & 0x7F) + 0x80));

    if (tmpValue > THREE_BYTES_MAX_UINT_VALUE)
        array.prepend((quint8)(((tmpValue >> 21) & 0x7F) + 0x80));

    return array;
}


QByteArray DuMidiVariableLength::formattedLengthArray(quint32 value)
{
    QByteArray array;
    array.clear();

    if (value > FOUR_BYTES_MAX_UINT_VALUE)
        return array;

    array.prepend((quint8)(value & 0x7F));

    if (value > ONE_BYTE_MAX_UINT_VALUE)
        array.prepend((quint8)(((value >> 7) & 0x7F) + 0x80));

    if (value > TWO_BYTES_MAX_UINT_VALUE)
        array.prepend((quint8)(((value >> 14) & 0x7F) + 0x80));

    if (value > THREE_BYTES_MAX_UINT_VALUE)
        array.prepend((quint8)(((value >> 21) & 0x7F) + 0x80));

    return array;
}


quint32 DuMidiVariableLength::formattedSize(quint32 value)
{
    if (value > FOUR_BYTES_MAX_UINT_VALUE)
        return 0;

    if (value > THREE_BYTES_MAX_UINT_VALUE)
        return 4;

    if (value > TWO_BYTES_MAX_UINT_VALUE)
        return 3;

    if (value > ONE_BYTE_MAX_UINT_VALUE)
        return 2;

    return 1;
}
*/
