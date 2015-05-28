#ifndef DUSONGINFO_H
#define DUSONGINFO_H

#include "../general/ducontainer.h"
#include "../general/dunumeric.h"


#define KEY_SONG_REFERENCETRACK         "ReferenceTrack"
#define KEY_SONG_REFERENCELOOPDURATION  "ReferenceLoopDuration"

#define KEY_SONG_VOLUME                 "Volume"
#define KEY_SONG_TEMPO                  "Tempo"
#define KEY_SONG_OFFSET                 "Offset"
#define KEY_SONG_CLICKVOLUME            "ClickVolume"

#define KEY_SONG_GAIN                   "Gain"
#define KEY_SONG_LOWCUTFILTERFREQ       "LowCutFilterFrequency"
#define KEY_SONG_HIGHCUTFILTERFREQ      "HighCutFilterFrequency"

#define KEY_SONG_TIMESIGNATURE          "TimeSignature"
#define KEY_SONG_SCALE                  "Scale"
#define KEY_SONG_TONALITY               "Tonality"

#define SONGINFO_SIZE                   19 + NUM_LED_VALUE + FX_MIX_SIZE + 25


DU_OBJECT(DuSongInfo)

class DuSongInfo : public DuContainer
{
public:
    DuSongInfo();
    ~DuSongInfo();

    virtual DuObjectPtr clone() const;

    static DuSongInfoPtr fromDuMusicBinary(const music_song &du_song);
    static DuSongInfoPtr fromJson(const QJsonObject &jsonSongInfo);

    QByteArray toDuMusicBinary() const;

    int size() const;

    int getReferenceTrack() const;
    bool setReferenceTrack(int value);

    int getReferenceLoopDuration() const;
    bool setReferenceLoopDuration(int value);

    int getVolume() const;
    bool setVolume(int value);

    int getTempo() const;
    bool setTempo(int value);

    int getClickVolume() const;
    bool setClickVolume(int value);

    int getOffset() const;
    bool setOffset(int value);

    int getGain() const;
    bool setGain(int value);

    int getLowCutFilterFrequency() const;
    bool setLowCutFilterFrequency(int value);

    int getHighCutFilterFrequency() const;
    bool setHighCutFilterFrequency(int value);

    int getScale() const;
    bool setScale(int value);

    int getTonality() const;
    bool setTonality(int value);

    int getTimeSignature() const;
    bool setTimeSignature(int value);
};

#endif // DUSONGINFO_H
