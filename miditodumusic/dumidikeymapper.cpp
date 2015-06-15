#include "dumidikeymapper.h"

#include <QDebug>


DU_OBJECT_IMPL(DuMidiKeyMapper)

DuMidiKeyMapper::DuMidiKeyMapper() :
    octave(0),
    scale(MAJOR_LED_MODE - 1),
    tonality(0),
    maps(QJsonArray()),
    DuObject()
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
    return QJsonValue(maps);
}


QDebug DuMidiKeyMapper::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuMidiKeyMapper(";

    dbg.nospace() << "octave=" << octave;
    dbg.nospace() << "scale=" << scale;
    dbg.nospace() << "tonality=" << tonality;

    dbg.nospace() << ")";

    return dbg.space();
}


int DuMidiKeyMapper::size() const
{
    Q_UNIMPLEMENTED();
    return -1;
}


void DuMidiKeyMapper::setMaps(QJsonArray value)
{
    maps = value;
}

void DuMidiKeyMapper::chooseMap(quint8 octave, quint8 scale, quint8 tonality)
{
    //TODO: implementation
}


quint8 DuMidiKeyMapper::fetchKeyboard(quint8 key)
{
    //TODO: implementation
    return 0;
}
