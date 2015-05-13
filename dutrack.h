#ifndef DUTRACK_H
#define DUTRACK_H

#include "duloop.h"


#define KEY_TRACK_CHANNEL       "Channel"
#define KEY_TRACK_CURRENTLOOP   "CurrentLoop"
#define KEY_TRACK_LOOPS         "Loops"


class DuTrack : public DuContainer
{
public:
    explicit DuTrack();
    ~DuTrack();

    static DuTrack *fromDuMusicFile(const music_track &du_track,
                                    const music_sample *du_sample);
    static DuTrack *fromJson(const QJsonObject &jsonTrack);

    int getChannel() const;
    bool setChannel(int value);

    int getCurrentLoop() const;
    bool setCurrentLoop(int value);

    DuArray *getLoops();
    void setLoops(DuArray *array);
};

#endif // DUTRACK_H
