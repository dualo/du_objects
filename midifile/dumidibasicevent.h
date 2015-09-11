#ifndef DUMIDIBASICEVENT_H
#define DUMIDIBASICEVENT_H

#include "../general/ducontainer.h"


#define KEY_MIDIEVENT_TIME      QStringLiteral("Time")
#define KEY_MIDIEVENT_STATUS    QStringLiteral("Status")


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

    int getTime() const;
    void setTime(quint32 delta, quint32 offset);
    void setTime(QDataStream &stream, quint32 offset);

    int getStatus() const;
    void setStatus(quint8 value);   // Status bytes are > 0x7F, data bytes are < 0x80

    bool runningStatus() const;
    void setRunningStatus(bool value);
};

#endif // DUMIDIBASICEVENT_H
