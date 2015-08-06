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
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuMidiKeyMapper::fetchKeyboard():\n"
                << "operation aborted\n"
                << "invalid scale";

        return -1;
    }

    QJsonValue chosenScaleValue = m_maps.value(m_scale);
    if (!chosenScaleValue.isArray())
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuMidiKeyMapper::fetchKeyboard():\n"
                << "operation aborted\n"
                << "invalid scale key map";

        return -1;
    }

    QJsonArray chosenScale = chosenScaleValue.toArray();
    int count = chosenScale.count();
    if (m_tonality >= count)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuMidiKeyMapper::fetchKeyboard():\n"
                << "operation aborted\n"
                << "invalid tonality";

        return -1;
    }

    QJsonValue chosenMapValue = chosenScale.at(m_tonality);
    if (!chosenMapValue.isArray())
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuMidiKeyMapper::fetchKeyboard():\n"
                << "operation aborted\n"
                << "invalid tonality key map";

        return -1;
    }

    QJsonArray chosenMap = chosenMapValue.toArray();
    if (chosenMap.count() != 12)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuMidiKeyMapper::fetchKeyboard():\n"
                << "operation aborted\n"
                << "invalid tonality key map";

        return -1;
    }

    QJsonValue chosenKeyboardValue = chosenMap.at(key % 12);
    if (!chosenKeyboardValue.isDouble())
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuMidiKeyMapper::fetchKeyboard():\n"
                << "operation aborted\n"
                << "invalid map value";

        return -1;
    }

    int chosenKeyboard = chosenKeyboardValue.toInt();

    // The first possible instrument octave is the second octave in MIDI.
    int parity = (key / 12 - octave + 1) % 2;

    if (parity != 0)
        chosenKeyboard ^= 0x08;

    // Return result in a form that can be directly added to a DuEvent note
    //(8th bit coding whether the note is played on the left or the right keyboard)
    return ((chosenKeyboard << 8) & 0x80);
}
