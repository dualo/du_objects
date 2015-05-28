#ifndef DUMIDIABSTRACTEVENT_H
#define DUMIDIABSTRACTEVENT_H

#include "../general/ducontainer.h"
#include "../general/dunumeric.h"
#include "dumidivariablelength.h"


#define KEY_MIDIEVENT_TIME      "Time"
#define KEY_MIDIEVENT_STATUS    "Status"


DU_OBJECT(DuMidiAbstractEvent)

class DuMidiAbstractEvent : public DuContainer
{
public:
    explicit DuMidiAbstractEvent(quint32 time = 0, quint8 status = 0x80);
    virtual ~DuMidiAbstractEvent();

    virtual QByteArray toByteArray(bool runningStatusActive = false) = 0;
    virtual void setDataBytes(QDataStream &stream) = 0;
    virtual void setDataBytes(const QByteArray &array) = 0;

    virtual QByteArray toDuMusicBinary() const;
    virtual QJsonValue toJson() const;

    quint32 getTime() const;
    void setTime(quint32 delta, quint32 offset);

    quint8 getStatus() const;
    void setStatus(quint8 value);   // Status bytes are > 0x7F, data bytes are < 0x80
};

#endif // DUMIDIABSTRACTEVENT_H
