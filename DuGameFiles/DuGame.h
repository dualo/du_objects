#ifndef DUGAME_H
#define DUGAME_H

#include "../general/ducontainer.h"

#ifdef Q_OS_WIN
#include "../general/duarray.h"
#else
DU_OBJECT_TEMPLATE(DuArray);
#endif
DU_OBJECT(DuGameEvent);

DU_OBJECT(DuGame);
DU_OBJECT(DuSound);

class DuGame : public DuContainer
{
public:
    DuGame();
    virtual ~DuGame() = default;

    static DuGamePtr fromBinary(const QByteArray& data, quint32 version);

    QByteArray toDuGameBinary(const QVector<DuSoundConstPtr> &systemSounds) const;

    // DuObject interface
public:
    virtual DuObjectPtr clone() const Q_DECL_OVERRIDE;
    virtual int size() const Q_DECL_OVERRIDE;

    DU_KEY_ACCESSORS(Grade, int)
    DU_KEY_ACCESSORS_OBJECT_TEMPLATE(Events, DuArray, DuGameEvent)
};

#endif // DUGAME_H
