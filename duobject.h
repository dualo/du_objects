#ifndef DUOBJECT_H
#define DUOBJECT_H

#include <QMetaType>

#include <QString>
#include <QByteArray>

#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

#include "du-touch/parameters/music_parameters_mng.h"

class DuObject
{
public:
    explicit DuObject();
    virtual ~DuObject();

    //virtual QByteArray toDuMusicFile() const = 0;
    virtual QJsonValue toJson() const = 0;
    virtual QDebug debugPrint(QDebug dbg) const;

    virtual int size() const = 0;
};

QDebug operator<<(QDebug dbg, const DuObject *obj);

#endif // DUOBJECT_H
