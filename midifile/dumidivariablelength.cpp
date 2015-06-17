#include "dumidivariablelength.h"



DU_OBJECT_IMPL(DuMidiVariableLength)

DuMidiVariableLength::DuMidiVariableLength(int offset) :
    DuValue(),
    offset(offset)
{
}

DuMidiVariableLength::~DuMidiVariableLength()
{
}


DuObjectPtr DuMidiVariableLength::clone() const
{
    return DuMidiVariableLengthPtr(new DuMidiVariableLength(*this));
}


QByteArray DuMidiVariableLength::toDuMusicBinary() const
{
    Q_UNIMPLEMENTED();
    return QByteArray();
}

QByteArray DuMidiVariableLength::toMidiBinary() const
{
    QByteArray array;
    array.clear();

    quint32 value = getRelative();

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

QJsonValue DuMidiVariableLength::toJson() const
{
    Q_UNIMPLEMENTED();
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


quint32 DuMidiVariableLength::getAbsolute() const
{
    return getOffset() + getValue().toInt();
}

void DuMidiVariableLength::setAbsolute(quint32 delta, quint32 offset)
{
    setOffset(offset);
    setRelative(delta);
}

void DuMidiVariableLength::setAbsolute(QDataStream &stream, quint32 offset)
{
    setOffset(offset);

    quint32 delta = 0;
    quint8 tmp;

    quint32 part1 = 0;
    quint32 part2 = 0;
    quint32 part3 = 0;
    quint32 part4 = 0;

    stream >> tmp;
    part4 = tmp;

    if (tmp > ONE_BYTE_MAX_UINT_VALUE)
    {
        stream >> tmp;
        part3 = part4;
        part4 = tmp;

        if (tmp > ONE_BYTE_MAX_UINT_VALUE)
        {
            stream >> tmp;
            part2 = part3;
            part3 = part4;
            part4 = tmp;

            if (tmp > ONE_BYTE_MAX_UINT_VALUE)
            {
                stream >> tmp;
                part1 = part2;
                part2 = part3;
                part3 = part4;
                part4 = tmp;
            }
        }
    }

    delta = ((part1 << 21) & 0x0FE00000) + ((part2 << 14) & 0x001FC000)
            + ((part3 << 7) & 0x00003F80) + part4;

    setRelative(delta);
}


quint32 DuMidiVariableLength::getRelative() const
{
    return getValue().toInt();
}

void DuMidiVariableLength::setRelative(quint32 value)
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
