#ifndef DUNUMERIC_H
#define DUNUMERIC_H

#include "duvalue.h"


#define NUMERIC_DEFAULT_SIZE    1


DU_OBJECT(DuNumeric);

class DuNumeric : public DuValue
{
public:
    explicit DuNumeric(int value = 0);
    explicit DuNumeric(int defaultValue, int byteSize, int maxValue, int minValue);
    explicit DuNumeric(int value, int byteSize, int maxValue,
                       int minValue, int defaultValue);
    virtual ~DuNumeric() = default;

    int getNumeric() const;
    bool setNumeric(int value);

    int getDefault() const;
    bool setDefault(int value);

    int getMax() const;
    bool setMax(int value);

    int getMin() const;
    bool setMin(int value);

    QVector<int> forbiddenValues() const;
    bool setForbiddenValues(const QVector<int>& forbiddenValues);

    // DuObject interface
public:
    virtual DuObjectPtr clone() const override;

    virtual QByteArray toDuMusicBinary() const override;
    virtual QByteArray toMidiBinary() const override;
    virtual QJsonValue toJson() const override;
    virtual QHttpPart toHttpPart(const QString &name) const override;

    virtual QDebug debugPrint(QDebug dbg) const override;

protected:
    virtual bool equals(const DuObject &other) const Q_DECL_OVERRIDE;

    // DuValue interface
protected:
    virtual QVariant checkValue(const QVariant &value, bool &success) override;

private:
    int m_defaultValue;
    int m_maxValue;
    int m_minValue;
    QVector<int> m_forbiddenValues;
};

#endif // DUNUMERIC_H
