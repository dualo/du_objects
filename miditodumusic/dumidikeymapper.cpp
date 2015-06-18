#include "dumidikeymapper.h"

#include <QJsonArray>


DU_OBJECT_IMPL(DuMidiKeyMapper)

DuMidiKeyMapper::DuMidiKeyMapper() :
    DuObject(),
    m_scale(SCALE_NONE),
    m_tonality(0)
{
}

DuMidiKeyMapper::~DuMidiKeyMapper()
{
}


DuObjectPtr DuMidiKeyMapper::clone() const
{
    return DuMidiKeyMapperPtr(new DuMidiKeyMapper(*this));
}


QByteArray DuMidiKeyMapper::toDuMusicBinary() const
{
    Q_UNIMPLEMENTED();
    return QByteArray();
}

QJsonValue DuMidiKeyMapper::toJson() const
{
    return QJsonValue(m_maps);
}


QDebug DuMidiKeyMapper::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuMidiKeyMapper(";

    dbg.nospace() << "scale=" << m_scale;
    dbg.nospace() << "tonality=" << m_tonality;

    dbg.nospace() << ")";

    return dbg.space();
}


int DuMidiKeyMapper::size() const
{
    Q_UNIMPLEMENTED();
    return -1;
}


void DuMidiKeyMapper::importMaps(QJsonObject value)
{
    m_maps = value;
}


QStringList DuMidiKeyMapper::mapList() const
{
    return m_maps.keys();
}

void DuMidiKeyMapper::chooseMap(QString scale, quint8 tonality)
{
    m_scale = scale;
    m_tonality = tonality % 12;
}


int DuMidiKeyMapper::fetchKeyboard(quint8 octave, quint8 key)
{
    if (!m_maps.contains(m_scale))
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMidiKeyMapper::fetchKeyboard():\n"
                    << "operation aborted\n"
                    << "the scale could not be found";
        return -1;
    }

    QJsonValue chosenScaleValue = m_maps.value(m_scale);
    if (!chosenScaleValue.isArray())
    {
        //TODO: message
        return -1;
    }

    QJsonArray chosenScale = chosenScaleValue.toArray();
    QJsonValue chosenMapValue = chosenScale.at(m_tonality);
    if (!chosenMapValue.isArray())
    {
        //TODO: message
        return -1;
    }

    QJsonArray chosenMap = chosenMapValue.toArray();
    if (chosenMap.count() != 12)
    {
        //TODO: message
        return -1;
    }

    QJsonValue chosenKeyboardValue = chosenMap.at(key % 12);
    if (!chosenKeyboardValue.isDouble())
    {
        //TODO: message
        return -1;
    }

    int chosenKeyboard = chosenKeyboardValue.toInt();

    // The first possible instrument octave is the second octave in MIDI.
    int parity = (key / 12 - octave + 1) % 2;

    if (parity != 0)
        chosenKeyboard ^= 0x08;

    return chosenKeyboard;
}
