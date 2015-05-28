#ifndef DUABSTRACTMIDIEVENT_H
#define DUABSTRACTMIDIEVENT_H

#include "dumidicontainer.h"
#include "dumidivariablelength.h"
#include "dumidinumeric.h"


#define KEY_MIDIEVENT_TIME      "Time"
#define KEY_MIDIEVENT_STATUS    "Status"


DU_OBJECT(DuAbstractMidiEvent)

class DuAbstractMidiEvent : public DuMidiContainer
{
public:
    explicit DuAbstractMidiEvent(quint32 time = 0, quint8 status = 0x80);
    virtual ~DuAbstractMidiEvent();

    virtual QByteArray toByteArray(bool runningStatusActive = false) = 0;
    virtual void setDataBytes(QDataStream &stream) = 0;
    virtual void setDataBytes(const QByteArray &array) = 0;

    quint32 getTime() const;
    void setTime(quint32 delta, quint32 offset);

    quint8 getStatus() const;
    void setStatus(quint8 value);   // Status bytes are > 0x7F, data bytes are < 0x80
};

#endif // DUABSTRACTMIDIEVENT_H
