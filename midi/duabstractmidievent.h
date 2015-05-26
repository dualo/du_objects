#ifndef DUABSTRACTMIDIEVENT_H
#define DUABSTRACTMIDIEVENT_H


#include "dumidivariablelength.h"
#include "../dunumeric.h"


class DuAbstractMidiEvent
{
public:
    explicit DuAbstractMidiEvent(quint32 time = 0, quint8 status = 0x80);
    virtual ~DuAbstractMidiEvent();

    virtual QByteArray toByteArray(bool runningStatusActive = false) = 0;
    virtual void setDataBytes(QDataStream &stream) = 0;
    virtual void setDataBytes(const QByteArray &array) = 0;
    virtual quint32 size() const = 0;

    quint32 getTime() const;
    quint8 getStatus() const;
    void setTime(quint32 offset, quint32 delta);
    void setStatus(quint8 value);

protected:
    DuMidiVariableLength *time;
    DuNumeric *status;              // Status bytes are > 0x7F, data bytes are < 0x80
};

#endif // DUABSTRACTMIDIEVENT_H
