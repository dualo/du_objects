#ifndef DUMIDISYSEXEVENT_H
#define DUMIDISYSEXEVENT_H

#include "dumidibasicevent.h"


#define KEY_MIDISYSEXEVENT_LENGTH   QStringLiteral("Length")
#define KEY_MIDISYSEXEVENT_DATA     QStringLiteral("Data")


DU_OBJECT(DuMidiSysExEvent);

class DuMidiSysExEvent : public DuMidiBasicEvent
{
public:
    explicit DuMidiSysExEvent(quint32 time = 0, quint8 status = 0xF0);
    virtual ~DuMidiSysExEvent();

    virtual DuObjectPtr clone() const;

    static DuMidiSysExEventPtr fromMidiBinary(QDataStream &stream, quint8 status);

    virtual QByteArray toMidiBinary() const;

    int getLength() const;
    void setLength(quint32 value);
    void setLength(QDataStream &stream);

    const QByteArray getData() const;
    void setData(const QByteArray &value);
    void setData(QDataStream &stream);
};

#endif // DUMIDISYSEXEVENT_H
