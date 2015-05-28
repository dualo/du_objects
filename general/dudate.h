#ifndef DUDATE_H
#define DUDATE_H

#include "duvalue.h"

#include <QDateTime>


DU_OBJECT(DuDate)

class DuDate : public DuValue
{
public:
    DuDate(const QDateTime& date = QDateTime());

    virtual DuObjectPtr clone() const;

    QByteArray toDuMusicBinary() const;
    QJsonValue toJson() const;
    QDebug debugPrint(QDebug dbg) const;

    QDateTime getDate() const;
    void setDate(const QDateTime& date);
};

#endif // DUDATE_H
