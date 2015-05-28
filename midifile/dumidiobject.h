#ifndef DUMIDIOBJECT_H
#define DUMIDIOBJECT_H

#include "../general/duobject.h"


DU_OBJECT(DuMidiObject)

class DuMidiObject : public DuObject
{
public:
    explicit DuMidiObject();
    virtual ~DuMidiObject();

protected:
    explicit DuMidiObject(const DuMidiObject &other);

public:
    virtual QByteArray toDuMusicBinary() const;
    virtual QJsonValue toJson() const;
    virtual const QByteArray toMidiBinary() const = 0;

    virtual int size() const = 0;

    virtual QDebug debugPrint(QDebug dbg) const;
};

#endif // DUMIDIOBJECT_H
