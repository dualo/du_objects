#ifndef DUMIDISYSEXEVENT_H
#define DUMIDISYSEXEVENT_H

#include "duabstractmidievent.h"
#include "dumididata.h"


#define KEY_MIDISYSEXEV_LENGTH      "Length"
#define KEY_MIDISYSEXEV_DATA        "Data"


DU_OBJECT(DuMidiSysExEvent)

class DuMidiSysExEvent : public DuAbstractMidiEvent
{
public:
    explicit DuMidiSysExEvent();
    virtual ~DuMidiSysExEvent();

    virtual QByteArray toByteArray(bool runningStatusActive = false);
    virtual void setDataBytes(QDataStream &stream);
    virtual void setDataBytes(const QByteArray &array);

    virtual DuObjectPtr clone() const;

    virtual const QByteArray toMidiBinary() const;

    virtual int size() const;

    int getLength() const;
    void setLength(quint32 value);

    const QByteArray getData() const;
    void setData(const QByteArray &value);

private:
    DuMidiVariableLength *length;
    DuMidiData *data;
};

#endif // DUMIDISYSEXEVENT_H
