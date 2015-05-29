#ifndef DUMIDIBASICEVENT_H
#define DUMIDIBASICEVENT_H

#include "../general/ducontainer.h"
#include "dumidivariablelength.h"
#include "dumidistatus.h"


#define KEY_MIDIEVENT_TIME      "Time"
#define KEY_MIDIEVENT_STATUS    "Status"


DU_OBJECT(DuMidiBasicEvent)

class DuMidiBasicEvent : public DuContainer
{
public:
    explicit DuMidiBasicEvent(quint32 time = 0, quint8 status = 0x80);
    virtual ~DuMidiBasicEvent();

    virtual DuObjectPtr clone() const;

    QByteArray toDuMusicBinary() const;
    virtual QByteArray toMidiBinary() const;
    QJsonValue toJson() const;

    quint32 getTime() const;
    void setTime(quint32 delta, quint32 offset);

    quint8 getStatus() const;
    void setStatus(quint8 value);   // Status bytes are > 0x7F, data bytes are < 0x80

    bool runningStatus() const;
    void setRunningStatus(bool value);
};

#endif // DUMIDIBASICEVENT_H
