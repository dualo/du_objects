#include "dumidistatus.h"



DU_OBJECT_IMPL(DuMidiStatus)

DuMidiStatus::DuMidiStatus(int value, bool runningStatus) :
    DuNumeric(value, NUMERIC_DEFAULT_SIZE, 0xFF, 0x80),
    runningStatus(runningStatus)
{
}

DuMidiStatus::~DuMidiStatus()
{
}


DuObjectPtr DuMidiStatus::clone() const
{
    return DuMidiStatusPtr(new DuMidiStatus(*this));
}


QByteArray DuMidiStatus::toMidiBinary() const
{
    if (runningStatus)
        return QByteArray("");

    QByteArray retArray;
    retArray.clear();

    int num = getNumeric();
    retArray.append((quint8)(num & 0xFF));

    return retArray;
}


int DuMidiStatus::size() const
{
    return runningStatus ? 0 : 1;
}


QDebug DuMidiStatus::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuMidiStatus(" << getNumeric() << ")";

    return dbg.space();
}


bool DuMidiStatus::getRunningStatus() const
{
    return runningStatus;
}

void DuMidiStatus::setRunningStatus(bool value)
{
    runningStatus = value;
}
