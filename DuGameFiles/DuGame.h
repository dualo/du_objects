#ifndef DUGAME_H
#define DUGAME_H

#include "../dumusicfile/dumusic.h"

#define DU_GAME_STRUCT_CURRENT_VERSION 1

DU_OBJECT(DuGame);

class DuGame : public DuMusic
{
public:
    DuGame();
    virtual ~DuGame() = default;

protected:
    explicit DuGame(const DuMusic &base);
    explicit DuGame(const DuGame &other) = default;

public:
    virtual DuObjectPtr clone() const override;

    static DuGamePtr fromDuGameBinary(const QByteArray &data);
    static DuGamePtr fromDuGameBinary(QIODevice *input);
    static DuGamePtr fromDuMusicAndSounds(const DuMusicConstPtr& music, const QVector<DuSoundPtr> &integratedSounds, const QVector<DuSoundPtr> &systemSounds);

    QByteArray toDuGameBinary() const;

    QVector<DuSoundPtr> integratedSounds() const;
    QVector<DuSoundPtr> systemSounds() const;

private:
    QVector<DuSoundPtr> m_integratedSounds;
    QVector<DuSoundPtr> m_systemSounds;
};

Q_DECLARE_METATYPE(DuGamePtr)
Q_DECLARE_METATYPE(DuGameConstPtr)

#endif // DUGAME_H
