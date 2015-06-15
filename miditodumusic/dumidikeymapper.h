#ifndef DUMIDIKEYMAPPER_H
#define DUMIDIKEYMAPPER_H

#include "../general/duobject.h"

#include <QJsonArray>


DU_OBJECT(DuMidiKeyMapper)

class DuMidiKeyMapper : public DuObject
{
public:
    explicit DuMidiKeyMapper();
    ~DuMidiKeyMapper();

    DuObjectPtr clone() const;

    QByteArray toDuMusicBinary() const;
    QJsonValue toJson() const;

    QDebug debugPrint(QDebug dbg) const;

    int size() const;

    void setMaps(QJsonArray value);
    void chooseMap(quint8 octave, quint8 scale, quint8 tonality);

    quint8 fetchKeyboard(quint8 key);

private:
    QJsonArray maps;
    quint8 octave;
    quint8 scale;
    quint8 tonality;
};

#endif // DUMIDIKEYMAPPER_H
