#ifndef DUNUMERIC_H
#define DUNUMERIC_H


#include "duvalue.h"


#define NUMERIC_DEFAULT_SIZE    1


class DuNumeric : public DuValue
{
public:
    explicit DuNumeric(int value);
    explicit DuNumeric(int defaultValue, int byteSize, int maxValue, int minValue);
    explicit DuNumeric(int value, int byteSize, int maxValue,
                       int minValue, int defaultValue);
    ~DuNumeric();

    QByteArray toDuMusicFile() const;
    QJsonValue toJson() const;
    QHttpPart toHttpPart(const QString &name) const;
    QDebug debugPrint(QDebug dbg) const;

    int getNumeric() const;
    bool setNumeric(int value);

    int getDefault() const;
    bool setDefault(int value);

    int getMax() const;
    bool setMax(int value);

    int getMin() const;
    bool setMin(int value);

private:
    int defaultValue;
    int maxValue;
    int minValue;
};

#endif // DUNUMERIC_H
