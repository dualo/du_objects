#ifndef DUNUMERIC_H
#define DUNUMERIC_H

#include "duvalue.h"


#define NUMERIC_DEFAULT_SIZE    1


DU_OBJECT(DuNumeric)

class DuNumeric : public DuValue
{
public:
    explicit DuNumeric(int value);
    explicit DuNumeric(int defaultValue, int byteSize, int maxValue, int minValue);
    explicit DuNumeric(int value, int byteSize, int maxValue,
                       int minValue, int defaultValue);
    virtual ~DuNumeric();

    int getNumeric() const;
    bool setNumeric(int value);

    int getDefault() const;
    bool setDefault(int value);

    int getMax() const;
    bool setMax(int value);

    int getMin() const;
    bool setMin(int value);

    // DuObject interface
public:
    virtual DuObjectPtr clone() const override;

    virtual QByteArray toDuMusicBinary() const override;
    virtual QByteArray toMidiBinary() const override;
    virtual QJsonValue toJson() const override;
    virtual QHttpPart toHttpPart(const QString &name) const override;

    virtual QDebug debugPrint(QDebug dbg) const override;

    // DuValue interface
public:
    virtual QVariant checkValue(const QVariant &value, bool &success) override;

private:
    int defaultValue;
    int maxValue;
    int minValue;
};

#endif // DUNUMERIC_H
