#ifndef DUSTRING_H
#define DUSTRING_H

#include "duvalue.h"


DU_OBJECT(DuString);

class DuString : public DuValue
{
public:
    explicit DuString(const QString &value, int maxSize = -1);
    explicit DuString(int maxSize = -1);
    virtual ~DuString();

    QString getString() const;
    bool setString(const QString &value);

    static QString fromStruct(const quint8 *str, uint maxSize);

    // DuObject interface
public:
    virtual DuObjectPtr clone() const override;

    virtual QByteArray toDuMusicBinary() const override;
    virtual QByteArray toMidiBinary() const override;
    virtual QJsonValue toJson() const override;
    virtual QHttpPart toHttpPart(const QString &name) const override;

    virtual QDebug debugPrint(QDebug dbg) const override;

    // DuValue interface
protected:
    virtual QVariant checkValue(const QVariant &value, bool &success) override;
};

#endif // DUSTRING_H
