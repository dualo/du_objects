#ifndef DUMIDIARRAY_H
#define DUMIDIARRAY_H

#include "../general/duarray.h"


DU_OBJECT(DuMidiArray)


class DuMidiArray : public DuArray
{
public:
    explicit DuMidiArray(int maxSize = -1);
    ~DuMidiArray();

    virtual DuObjectPtr clone() const;

    virtual QByteArray toDuMusicBinary() const;
    virtual QByteArray toMidiBinary() const;
    virtual QJsonValue toJson() const;

    int size() const;

    QDebug debugPrint(QDebug dbg) const;
};

#endif // DUMIDIARRAY_H
