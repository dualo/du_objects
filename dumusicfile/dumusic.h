#ifndef DUMUSIC_H
#define DUMUSIC_H

#include "duheader.h"
#include "dusonginfo.h"
#include "dutrack.h"
#include "instrument/ducontrollers.h"
#include "instrument/effects/dureverb.h"


class QIODevice;

DU_OBJECT(DuMusic)

class DuMusic : public DuContainer
{
public:
    DuMusic();

    virtual DuObjectPtr clone() const override;

    static DuMusicPtr fromDuMusicBinary(s_total_buffer &du_music, int fileSize);
    static DuMusicPtr fromBinary(const QByteArray &data);
    static DuMusicPtr fromBinary(QIODevice *input);

    static bool upgrade(s_total_buffer &du_music);

    static DuMusicPtr fromJson(const QJsonObject &jsonMusic);

    static DuMusicPtr fromMidi(const MidiConversionHelper &helper);

    QByteArray toDuMusicBinary() const override;
    QByteArray toMidiBinary() const override;

    int size() const override;
    bool isEmpty() const;

    int databaseId() const;
    void setDatabaseId(int databaseId);

    QStringList lists() const;
    void setLists(const QStringList &lists);

    QString getSongName() const;
    bool setSongName(const QString &value);

    int getFileVersion() const;
    bool setFileVersion(int value);

    DU_KEY_ACCESSORS_OBJECT(Header,      DuHeader)
    DU_KEY_ACCESSORS_OBJECT(Controllers, DuControllers)
    DU_KEY_ACCESSORS_OBJECT(SongInfo,    DuSongInfo)
    DU_KEY_ACCESSORS_OBJECT(Reverb,      DuReverb)
    DU_KEY_ACCESSORS_OBJECT(Tracks,      DuArray)

public:
    bool appendTrack(const DuTrackPtr &track);

    DU_KEY_ACCESSORS(Transpose, int)

    DU_KEY_ACCESSORS(Playhead,  int)
    DU_KEY_ACCESSORS(State,     int)

private:
    int m_databaseId;
    QStringList m_lists;
};

Q_DECLARE_METATYPE(DuMusicPtr)
Q_DECLARE_METATYPE(DuMusicConstPtr)

#endif // DUMUSIC_H
