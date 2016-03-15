#ifndef DUMUSIC_H
#define DUMUSIC_H

#include "../general/ducontainer.h"


#define DuMusic_Children \
    X(FileVersion,           Numeric, int, -1) \
    \
    X(OriginalSerialNumber,  String, QString, QString()) \
    X(OriginalName,          String, QString, QString()) \
    X(OriginalUser,          String, QString, QString()) \
    X(OriginalUserId,        String, QString, QString()) \
    \
    X(LastModifSerialNumber, String, QString, QString()) \
    X(LastModifName,         String, QString, QString()) \
    X(LastModifUser,         String, QString, QString()) \
    X(LastModifUserId,       String, QString, QString()) \
    \
    X(Size,                  Numeric, int, -1) \
    X(MetaData,              Numeric, int, -1) \
    \
    X(Playhead,              Numeric, int, -1) \
    X(Transpose,             Numeric, int, -1) \
    X(State,                 Numeric, int, -1) \
    \
    X(DirectionGyroP,        Numeric, int, -1) \
    X(DirectionGyroR,        Numeric, int, -1) \
    X(DirectionGyroY,        Numeric, int, -1) \
    X(ActiveAftertouch,      Numeric, int, -1) \
    X(ActiveSliderL,         Numeric, int, -1) \
    X(ActiveSliderR,         Numeric, int, -1) \
    X(ActiveGyroP,           Numeric, int, -1) \
    X(ActiveGyroR,           Numeric, int, -1) \
    X(ActiveGyroY,           Numeric, int, -1) \
    \
    X(SongId,                Numeric, int, -1) \
    X(SongName,              String, QString, QString()) \
    X(SongVersion,           Numeric, int, -1) \
    \
    X(Volume,                Numeric, int, -1) \
    X(Tempo,                 Numeric, int, -1) \
    X(ClickVolume,           Numeric, int, -1) \
    X(ReferenceTrack,        Numeric, int, -1) \
    X(CurrentTrack,          Numeric, int, -1) \
    X(Offset,                Numeric, int, -1) \
    X(ReferenceLoopDuration, Numeric, int, -1) \
    X(Leds,                  BinaryData, QByteArray, QByteArray()) \
    \
    X_OBJECT(Mixer, DuMixer) \
    \
    X(Swing,                 Numeric, int, -1) \
    X(Quantification,        Numeric, int, -1) \
    X(Scale,                 Numeric, int, -1) \
    X(Tonality,              Numeric, int, -1) \
    X(TimeSignature,         Numeric, int, -1) \
    \
    X(ReverbPreset,          Numeric, int, -1) \
    X_OBJECT(Reverb, DuReverb) \
    \
    X_OBJECT_TEMPLATE(Tracks, DuArray, DuTrack)


class MidiConversionHelper;
class QIODevice;
#ifdef Q_OS_WIN
#include "../general/duarray.h"
#else
DU_OBJECT_TEMPLATE(DuArray)
#endif
DU_OBJECT(DuMixer)
DU_OBJECT(DuReverb)
DU_OBJECT(DuTrack)

DU_OBJECT(DuMusic)

class DuMusic : public DuContainer
{
public:
    DuMusic();

    virtual DuObjectPtr clone() const override;

    static DuMusicPtr fromDuMusicBinary(s_total_buffer &du_music, int fileSize);
    static DuMusicPtr fromBinary(const QByteArray &data);
    static DuMusicPtr fromBinary(QIODevice *input);
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

    bool appendTrack(const DuTrackPtr &track);

#define X(key, dutype, type, defaultReturn) DU_KEY_ACCESSORS(key, type)
#define X_OBJECT(key, dutype) DU_KEY_ACCESSORS_OBJECT(key, dutype)
#define X_OBJECT_TEMPLATE(key, dutype, tpltype) DU_KEY_ACCESSORS_OBJECT_TEMPLATE(key, dutype, tpltype)
    DuMusic_Children
#undef X_OBJECT_TEMPLATE
#undef X_OBJECT
#undef X

private:
    int m_databaseId;
    QStringList m_lists;
};

Q_DECLARE_METATYPE(DuMusicPtr)
Q_DECLARE_METATYPE(DuMusicConstPtr)

#endif // DUMUSIC_H
