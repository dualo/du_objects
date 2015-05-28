#ifndef DUMIDINUMERIC_H
#define DUMIDINUMERIC_H

#include "dumidivalue.h"


#define MIDINUMERIC_DEFAULT_SIZE    1


DU_OBJECT(DuMidiNumeric)

class DuMidiNumeric : public DuMidiValue
{
public:
    explicit DuMidiNumeric(int value, int byteSize, int maxValue, int minValue);
    ~DuMidiNumeric();

    virtual DuObjectPtr clone() const;

    const QByteArray toMidiBinary() const;

    QDebug debugPrint(QDebug dbg) const;

    int getNumeric() const;
    bool setNumeric(int value);

    int getMax() const;
    bool setMax(int value);

    int getMin() const;
    bool setMin(int value);

private:
    int maxValue;
    int minValue;
};

#endif // DUMIDINUMERIC_H
