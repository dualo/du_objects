#ifndef DUTRACK_H
#define DUTRACK_H

#include "duloop.h"


#define KEY_TRACK_CHANNEL       "Channel"
#define KEY_TRACK_CURRENTLOOP   "CurrentLoop"
#define KEY_TRACK_LOOPS         "Loops"

#define TRACK_DUMMY_SIZE        2


DU_OBJECT(DuTrack)

class DuTrack : public DuContainer
{
public:
    explicit DuTrack();
    ~DuTrack();

    virtual DuObjectPtr clone() const;

    static DuTrackPtr fromDuMusicFile(const music_track &du_track,
                                    const music_sample *du_sample);
    static DuTrackPtr fromJson(const QJsonObject &jsonTrack);

    QByteArray toDuMusicFile() const;

    int size() const;

    int getChannel() const;
    bool setChannel(int value);

    int getCurrentLoop() const;
    bool setCurrentLoop(int value);

    DuArrayConstPtr getLoops() const;
    void setLoops(const DuArrayPtr &array);

    bool appendLoop(const DuLoopPtr& loop);

    int eventsSize() const;
};

#endif // DUTRACK_H
