#ifndef DUABSTRACTMIDIEVENT_H
#define DUABSTRACTMIDIEVENT_H

#include "dumidivariablelength.h"


class DuAbstractMidiEvent
{
public:
    explicit DuAbstractMidiEvent();
    virtual ~DuAbstractMidiEvent();

    virtual QByteArray toByteArray(bool runningStatusActive = false) = 0;
    virtual void setDataBytes(QDataStream &stream) = 0;
    virtual void setDataBytes(QByteArray &array) = 0;
    virtual quint32 size() = 0;

    quint32 getTime();
    quint8 getStatus();
    void setTime(quint32 value);
    void setStatus(quint8 value);

protected:
    quint32 time;
    quint8 status;                  // Status bytes are > 0x7F, data bytes are < 0x80
};

#endif // DUABSTRACTMIDIEVENT_H
