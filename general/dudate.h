#ifndef DUDATE_H
#define DUDATE_H

#include "duvalue.h"

#include <QDateTime>


DU_OBJECT(DuDate);

class DuDate : public DuValue
{
public:
    explicit DuDate(const QDateTime& date = QDateTime());
    virtual ~DuDate() = default;

    QDateTime getDate() const;
    bool setDate(const QDateTime& date);

    // DuObject interface
public:
    virtual DuObjectPtr clone() const override;

    virtual QByteArray toDuMusicBinary() const override;
    virtual QJsonValue toJson() const override;

    virtual QDebug debugPrint(QDebug dbg) const override;

    // DuValue interface
protected:
    virtual QVariant checkValue(const QVariant &value, bool &success) override;
};

#endif // DUDATE_H
