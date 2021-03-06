#ifndef DUTRACK_H
#define DUTRACK_H

#include "../general/ducontainer.h"


#define TRACK_DUMMY_SIZE        2


#ifndef NO_MIDI_IMPORT
class MidiConversionHelper;
#endif

#ifdef Q_OS_WIN
#include "../general/duarray.h"
#else
DU_OBJECT_TEMPLATE(DuArray);
#endif
DU_OBJECT(DuLoop);
DU_OBJECT(DuMidiTrack);

DU_OBJECT(DuTrack);

class DuTrack : public DuContainer
{
public:
    DuTrack();
    ~DuTrack() = default;

    virtual DuObjectPtr clone() const;

    static DuTrackPtr fromDuMusicBinary(const music_track &du_track,
                                        const music_sample *du_sample_start,
                                        uint totalNbSamples);

#ifndef NO_MIDI_IMPORT
    static DuTrackPtr fromMidi(const MidiConversionHelper &helper, int trackIndex);
#endif

    QByteArray toDuMusicBinary() const;

    QVector<DuMidiTrackPtr> toDuMidiTrackArray(int durationRef, int transpose, int trackId) const;

    int size() const;


    DU_KEY_ACCESSORS(Channel, int)
    DU_KEY_ACCESSORS(CurrentLoop, int)
    DU_KEY_ACCESSORS_OBJECT_TEMPLATE(Loops, DuArray, DuLoop)

public:
    bool appendLoop(const DuLoopPtr &loop);

    int eventsSize() const;
};

#endif // DUTRACK_H
