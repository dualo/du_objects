#ifndef DUMIDISYSEXEVENT_H
#define DUMIDISYSEXEVENT_H

#include "duabstractmidievent.h"


class DuMidiSysExEvent : public DuAbstractMidiEvent
{
public:
    explicit DuMidiSysExEvent();
    ~DuMidiSysExEvent();

    QByteArray toByteArray(bool runningStatusActive = false);
    void setDataBytes(QDataStream &stream);
    void setDataBytes(const QByteArray &array);
    quint32 size();

    void setLength(quint32 value);

private:
    QByteArray data;
};

#endif // DUMIDISYSEXEVENT_H
