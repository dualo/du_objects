#ifndef DUMIDISYSEXEVENT_H
#define DUMIDISYSEXEVENT_H

#include "duabstractmidievent.h"
#include "dumididata.h"


class DuMidiSysExEvent : public DuAbstractMidiEvent
{
public:
    explicit DuMidiSysExEvent();
    virtual ~DuMidiSysExEvent();

    QByteArray toByteArray(bool runningStatusActive = false);
    void setDataBytes(QDataStream &stream);
    void setDataBytes(const QByteArray &array);
    quint32 size() const;

    const QByteArray getData() const;
    void setData(const QByteArray &value);

    void setLength(quint32 value);

private:
    DuMidiData *data;
};

#endif // DUMIDISYSEXEVENT_H
