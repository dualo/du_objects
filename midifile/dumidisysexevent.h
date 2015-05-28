#ifndef DUMIDISYSEXEVENT_H
#define DUMIDISYSEXEVENT_H

#include "dumidiabstractevent.h"
#include "../general/dubinarydata.h"


#define KEY_MIDISYSEXEVENT_LENGTH      "Length"
#define KEY_MIDISYSEXEVENT_DATA        "Data"


DU_OBJECT(DuMidiSysExEvent)

class DuMidiSysExEvent : public DuMidiAbstractEvent
{
public:
    explicit DuMidiSysExEvent();
    virtual ~DuMidiSysExEvent();

    virtual QByteArray toByteArray(bool runningStatusActive = false);
    virtual void setDataBytes(QDataStream &stream);
    virtual void setDataBytes(const QByteArray &array);

    virtual DuObjectPtr clone() const;

    virtual QByteArray toMidiBinary() const;

    int getLength() const;
    void setLength(quint32 value);

    const QByteArray getData() const;
    void setData(const QByteArray &value);
    void setData(QDataStream &stream);
};

#endif // DUMIDISYSEXEVENT_H
