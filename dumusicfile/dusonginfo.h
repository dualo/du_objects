#ifndef DUSONGINFO_H
#define DUSONGINFO_H

#include "../general/ducontainer.h"
#include "../general/dunumeric.h"
#include "../general/dustring.h"

#include "../miditodumusic/midiconversionhelper.h"


#define KEY_SONG_SONGID                 "SongID"
#define KEY_SONG_SONGNAME               "SongName"
#define KEY_SONG_SONGVERSION            "SongVersion"

#define KEY_SONG_REFERENCETRACK         "ReferenceTrack"
#define KEY_SONG_REFERENCELOOPDURATION  "ReferenceLoopDuration"
#define KEY_SONG_CURRENTTRACK           "CurrentTrack"

#define KEY_SONG_VOLUME                 "Volume"
#define KEY_SONG_TEMPO                  "Tempo"
#define KEY_SONG_OFFSET                 "Offset"
#define KEY_SONG_CLICKVOLUME            "ClickVolume"

#define KEY_SONG_MIXER                  "Mixer"

#define KEY_SONG_TIMESIGNATURE          "TimeSignature"
#define KEY_SONG_SCALE                  "Scale"
#define KEY_SONG_TONALITY               "Tonality"

#define KEY_SONG_REVERBPRESET           "ReverbPreset"

#define KEY_SONG_LEDS                   "LEDs"

#define KEY_SONG_QUANTIFICATION         "Quantification"

#define SONGINFO_SIZE       8 + MUSIC_SONG_NAME_SIZE + 20 + NUM_LED_VALUE + FX_MIX_SIZE
#define SONGINFO_OFFSET     4 + 8 * 16 + 10 + 3 + 15

//NOTE: the 2 #define above could be replaced if dummy sizes were defined


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

    int getSongId() const;
    bool setSongId(int value);

    QString getSongName() const;
    bool setSongName(const QString value);

    int getSongVersion() const;
    bool setSongVersion(int value);

    int getReferenceTrack() const;
    bool setReferenceTrack(int value);

    int getReferenceLoopDuration() const;
    bool setReferenceLoopDuration(int value);

    int getCurrentTrack() const;
    bool setCurrentTrack(int value);

    int getVolume() const;
    bool setVolume(int value);

    int getTempo() const;
    bool setTempo(int value);

    int getClickVolume() const;
    bool setClickVolume(int value);

    int getOffset() const;
    bool setOffset(int value);

    DuMixerConstPtr getMixer() const;
    void setMixer(const DuMixerPtr &mixer);

    int getScale() const;
    bool setScale(int value);

    int getTonality() const;
    bool setTonality(int value);

    int getTimeSignature() const;
    bool setTimeSignature(int value);

    int getReverbPreset() const;
    bool setReverbPreset(int value);

    QByteArray getLeds() const;
    bool setLeds(const QByteArray &value);

    int getQuantification() const;
    bool setQuantification(int value);
};

#endif // DUSONGINFO_H
