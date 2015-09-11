#ifndef DULOOP_H
#define DULOOP_H

#include "../general/ducontainer.h"


class MidiConversionHelper;
DU_OBJECT(DuArray);
DU_OBJECT(DuEvent);
DU_OBJECT(DuInstrument);
DU_OBJECT(DuMidiTrack);

DU_OBJECT(DuLoop);

class DuLoop : public DuContainer
{
public:
    explicit DuLoop();
    ~DuLoop();

    virtual DuObjectPtr clone() const;

    static DuLoopPtr fromDuMusicBinary(const music_loop &du_loop,
                                       const music_sample *du_sample);
    static DuLoopPtr fromJson(const QJsonObject &jsonLoop);
    static DuLoopPtr fromMidi(const MidiConversionHelper &helper, int midiTrackIndex);

    QByteArray toDuMusicBinary() const;
    DuMidiTrackPtr toDuMidiTrack(int durationRef, int channel, int transpose) const;

    int size() const;

DU_KEY_ACCESSORS(State,            int)
DU_KEY_ACCESSORS(DurationModifier, int)
DU_KEY_ACCESSORS(ScoreDisplay,     int)
DU_KEY_ACCESSORS(MidiOutChannel,   int)

DU_KEY_ACCESSORS(SaveLoopTimer,    int)

DU_KEY_ACCESSORS_OBJECT(Instrument, DuInstrument)
DU_KEY_ACCESSORS_OBJECT(Events,     DuArray)

public:
    int eventsSize() const;

    int countEvents() const;
    bool appendEvent(const DuEventPtr &event);
};

#endif // DULOOP_H
