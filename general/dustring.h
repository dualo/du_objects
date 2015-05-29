#ifndef DUSTRING_H
#define DUSTRING_H

#include "duvalue.h"


DU_OBJECT(DuString)

class DuString : public DuValue
{
public:
    explicit DuString(const QString &value, int maxSize = -1);
    explicit DuString(int maxSize = -1);
    virtual ~DuString();

    virtual DuObjectPtr clone() const;

    QByteArray toDuMusicBinary() const;
    QByteArray toMidiBinary() const;
    QJsonValue toJson() const;

    QHttpPart toHttpPart(const QString &name) const;
    virtual QDebug debugPrint(QDebug dbg) const;

    QString getString() const;
    bool setString(const QString &value);
};

#endif // DUSTRING_H
