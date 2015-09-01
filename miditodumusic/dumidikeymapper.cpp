#include "dumidikeymapper.h"

#pragma pack(push, 4)
#include "../du-touch/parameters/g_parameters_mng.h"
#pragma pack(pop)

#include <QJsonArray>
#include <QDebug>


DuMidiKeyMapper::DuMidiKeyMapper(QObject *parent) :
    QObject(parent),
    m_scale(SCALE_NONE),
    m_tonality(0)
{
}

DuMidiKeyMapper::~DuMidiKeyMapper()
{
}


QJsonValue DuMidiKeyMapper::toJson() const
{
    return QJsonValue(m_maps);
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

QPair<QString, QString> DuMidiKeyMapper::scaleIds(const QString &scale) const
{
    QPair<QString, QString> ids = qMakePair(QString(SCALE_NONE), tr(SCALE_NONE));

    //TODO: update following tests each time the scales in defaultmaps.json change

    if (scale == SCALE_MAJOR)
    {
        ids = qMakePair(QString(SCALE_MAJOR), tr(SCALE_MAJOR));
    }
    else if (scale == SCALE_MINOR)
    {
        ids = qMakePair(QString(SCALE_MINOR), tr(SCALE_MINOR));
    }
    else if (scale == SCALE_HARMONIC_MINOR)
    {
        ids = qMakePair(QString(SCALE_HARMONIC_MINOR), tr(SCALE_HARMONIC_MINOR));
    }
    else if (scale == SCALE_MELODIC_MINOR)
    {
        ids = qMakePair(QString(SCALE_MELODIC_MINOR), tr(SCALE_MELODIC_MINOR));
    }
    else if (scale == SCALE_PHRYGIAN)
    {
        ids = qMakePair(QString(SCALE_PHRYGIAN), tr(SCALE_PHRYGIAN));
    }
    else if (scale == SCALE_DORIAN)
    {
        ids = qMakePair(QString(SCALE_DORIAN), tr(SCALE_DORIAN));
    }
    else if (scale == SCALE_LYDIAN)
    {
        ids = qMakePair(QString(SCALE_LYDIAN), tr(SCALE_LYDIAN));
    }
    else if (scale == SCALE_MIXOLYDIAN)
    {
        ids = qMakePair(QString(SCALE_MIXOLYDIAN), tr(SCALE_MIXOLYDIAN));
    }
    else if (scale == SCALE_DOUBLE_HARMONIC)
    {
        ids = qMakePair(QString(SCALE_DOUBLE_HARMONIC), tr(SCALE_DOUBLE_HARMONIC));
    }
    else if (scale == SCALE_BARTOK)
    {
        ids = qMakePair(QString(SCALE_BARTOK), tr(SCALE_BARTOK));
    }
    else if (scale == SCALE_HUNGARIAN_MINOR)
    {
        ids = qMakePair(QString(SCALE_HUNGARIAN_MINOR), tr(SCALE_HUNGARIAN_MINOR));
    }
    else if (scale == SCALE_PHRYGIAN_DOMINANT)
    {
        ids = qMakePair(QString(SCALE_PHRYGIAN_DOMINANT), tr(SCALE_PHRYGIAN_DOMINANT));
    }
    else if (scale == SCALE_UKRAINIAN_DORIAN)
    {
        ids = qMakePair(QString(SCALE_UKRAINIAN_DORIAN), tr(SCALE_UKRAINIAN_DORIAN));
    }

    return ids;
}

int DuMidiKeyMapper::dutouchScale(const QString &scale) const
{
    int dutouchScale = NONE_LED_MODE;

    //TODO: update following tests each time the scales in the du-touch change

    if (scale == SCALE_MAJOR)
    {
        dutouchScale = MAJOR_LED_MODE;
    }
    else if (scale == SCALE_MINOR)
    {
        dutouchScale = MINOR_NAT_LED_MODE;
    }
    else if (scale == SCALE_HARMONIC_MINOR)
    {
        dutouchScale = MINOR_HARM_LED_MODE;
    }
    else if (scale == SCALE_DORIAN)
    {
        dutouchScale = DORIEN_MODE;
    }
    else if (scale == SCALE_MIXOLYDIAN)
    {
        dutouchScale = MIXOLIDIEN_MODE;
    }

    return dutouchScale;
}


int DuMidiKeyMapper::keyboardFromMidi(quint8 key)
{
    if (!m_maps.contains(m_scale))
    {
        qCritical()
                << "DuMidiKeyMapper::fetchKeyboard():\n"
                << "operation aborted\n"
                << "invalid scale";

        return -1;
    }

    QJsonValue chosenScaleValue = m_maps.value(m_scale);
    if (!chosenScaleValue.isArray())
    {
        qCritical()
                << "DuMidiKeyMapper::fetchKeyboard():\n"
                << "operation aborted\n"
                << "invalid scale key map";

        return -1;
    }

    QJsonArray chosenScale = chosenScaleValue.toArray();
    int count = chosenScale.count();
    if (m_tonality >= count)
    {
        qCritical()
                << "DuMidiKeyMapper::fetchKeyboard():\n"
                << "operation aborted\n"
                << "invalid tonality";

        return -1;
    }

    QJsonValue chosenMapValue = chosenScale.at(m_tonality);
    if (!chosenMapValue.isArray())
    {
        qCritical()
                << "DuMidiKeyMapper::fetchKeyboard():\n"
                << "operation aborted\n"
                << "invalid tonality key map";

        return -1;
    }

    QJsonArray chosenMap = chosenMapValue.toArray();
    if (chosenMap.count() != 12)
    {
        qCritical()
                << "DuMidiKeyMapper::fetchKeyboard():\n"
                << "operation aborted\n"
                << "invalid tonality key map";

        return -1;
    }

    QJsonValue chosenKeyboardValue = chosenMap.at(key % 12);
    if (!chosenKeyboardValue.isDouble())
    {
        qCritical()
                << "DuMidiKeyMapper::fetchKeyboard():\n"
                << "operation aborted\n"
                << "invalid map value";

        return -1;
    }

    int chosenKeyboard = chosenKeyboardValue.toInt();

    // The first possible instrument octave is the second octave in MIDI.
    int parity = (key / 12 + 1) % 2;

    if (parity != 0)
        chosenKeyboard = (chosenKeyboard + 1) % 2;

    //Return result in a form that can be directly added to a DuEvent note
    //(8th bit coding whether the note is played on the left or the right keyboard)
    return 0x80 * chosenKeyboard;
}
