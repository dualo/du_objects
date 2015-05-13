#ifndef DUSONGINFO_H
#define DUSONGINFO_H

#include "ducontainer.h"
#include "dunumeric.h"


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


class DuSongInfo : public DuContainer
{
public:
    DuSongInfo();
    ~DuSongInfo();

    static DuSongInfo *fromDuMusicFile(const music_song &du_song);
    static DuSongInfo *fromJson(const QJsonObject &jsonSongInfo);

    int getReferenceTrack() const;
    bool setReferenceTrack(int value);

    int getReferenceLoopDuration() const;
    bool setReferenceLoopDuration(int value);

    int getVolume() const;
    bool setVolume(int value);

    int getTempo() const;
    bool setTempo(int value);

    int getOffset() const;
    bool setOffset(int value);

    int getClickVolume() const;
    bool setClickVolume(int value);

    int getGain() const;
    bool setGain(int value);

    int getLowCutFilterFrequency() const;
    bool setLowCutFilterFrequency(int value);

    int getHighCutFilterFrequency() const;
    bool setHighCutFilterFrequency(int value);

    int getTimeSignature() const;
    bool setTimeSignature(int value);

    int getScale() const;
    bool setScale(int value);

    int getTonality() const;
    bool setTonality(int value);
};

#endif // DUSONGINFO_H
