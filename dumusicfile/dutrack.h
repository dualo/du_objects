#ifndef DUTRACK_H
#define DUTRACK_H

#include "../general/ducontainer.h"


#define TRACK_DUMMY_SIZE        2


class MidiConversionHelper;
DU_OBJECT(DuArray)
DU_OBJECT(DuLoop)
DU_OBJECT(DuMidiTrack)

DU_OBJECT(DuTrack)

class DuTrack : public DuContainer
{
public:
    explicit DuTrack();
    ~DuTrack();

    virtual DuObjectPtr clone() const;

    static DuTrackPtr fromDuMusicBinary(const music_track &du_track,
                                        const music_sample *du_sample_start,
                                        uint totalNbSamples);
    static DuTrackPtr fromJson(const QJsonObject &jsonTrack);
    static DuTrackPtr fromMidi(const MidiConversionHelper &helper, int trackIndex);

    QByteArray toDuMusicBinary() const;
    QVector<DuMidiTrackPtr> toDuMidiTrackArray(int durationRef, int transpose) const;

    int size() const;


    DU_KEY_ACCESSORS(Channel, int)
    DU_KEY_ACCESSORS(CurrentLoop, int)
    DU_KEY_ACCESSORS_OBJECT(Loops, DuArray)

public:
    bool appendLoop(const DuLoopPtr &loop);

    int eventsSize() const;
};

#endif // DUTRACK_H
