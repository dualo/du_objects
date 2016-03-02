#ifndef DUBOOLEAN_H
#define DUBOOLEAN_H

#include "duvalue.h"

DU_OBJECT(DuBoolean);

class DuBoolean : public DuValue
{
public:
    explicit DuBoolean(bool value);

    bool getBoolean() const;
    bool setBoolean(bool value);

    // DuObject interface
public:
    virtual DuObjectPtr clone() const override;
    virtual QByteArray toDuMusicBinary() const override;
    virtual QByteArray toMidiBinary() const override;
    virtual QJsonValue toJson() const override;
    virtual QHttpPart toHttpPart(const QString &name) const override;
    virtual QDebug debugPrint(QDebug dbg) const override;
};

#endif // DUBOOLEAN_H
