#ifndef DUMIDISTATUS_H
#define DUMIDISTATUS_H

#include "../general/dunumeric.h"


DU_OBJECT(DuMidiStatus)

class DuMidiStatus : public DuNumeric
{
public:
    explicit DuMidiStatus(int value, bool runningStatus = false);
    ~DuMidiStatus();

    virtual DuObjectPtr clone() const;

    QByteArray toMidiBinary() const;

    QDebug debugPrint(QDebug dbg) const;

    bool getRunningStatus() const;
    void setRunningStatus(bool value);

private:
    bool runningStatus;
};

#endif // DUMIDISTATUS_H
