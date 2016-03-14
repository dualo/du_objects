#ifndef DUSONGINFO_H
#define DUSONGINFO_H

#include "../general/ducontainer.h"


#define SONGINFO_SIZE       8 + MUSIC_SONG_NAME_SIZE + 20 + NUM_LED_VALUE + FX_MIX_SIZE
#define SONGINFO_OFFSET     4 + 8 * 16 + 10 + 3 + 15

//NOTE: the 2 #define above could be replaced if dummy sizes were defined

class MidiConversionHelper;
DU_OBJECT(DuMixer)

DU_OBJECT(DuSongInfo)

class DuSongInfo : public DuContainer
{
public:
    DuSongInfo();
    ~DuSongInfo();

    virtual DuObjectPtr clone() const;

    static DuSongInfoPtr fromDuMusicBinary(const music_song &du_song);
    static DuSongInfoPtr fromJson(const QJsonObject &jsonSongInfo);
    static DuSongInfoPtr fromMidi(const MidiConversionHelper &helper);

    QByteArray toDuMusicBinary() const;

    int size() const;

    DU_KEY_ACCESSORS(SongId,                int)
    DU_KEY_ACCESSORS(SongName,              QString)
    DU_KEY_ACCESSORS(SongVersion,           int)

    DU_KEY_ACCESSORS(ReferenceTrack,        int)
    DU_KEY_ACCESSORS(ReferenceLoopDuration, int)
    DU_KEY_ACCESSORS(CurrentTrack,          int)

    DU_KEY_ACCESSORS(Volume,                int)
    DU_KEY_ACCESSORS(Tempo,                 int)
    DU_KEY_ACCESSORS(Offset,                int)
    DU_KEY_ACCESSORS(ClickVolume,           int)

    DU_KEY_ACCESSORS_OBJECT(Mixer, DuMixer)

    DU_KEY_ACCESSORS(TimeSignature,         int)
    DU_KEY_ACCESSORS(Scale,                 int)
    DU_KEY_ACCESSORS(Tonality,              int)

    DU_KEY_ACCESSORS(ReverbPreset,          int)

    DU_KEY_ACCESSORS(Leds,                  QByteArray)

    DU_KEY_ACCESSORS(Swing,                 int)
    DU_KEY_ACCESSORS(Quantification,        int)
};

#endif // DUSONGINFO_H
