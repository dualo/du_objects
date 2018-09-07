#ifndef DULOOP_H
#define DULOOP_H

#include "../general/ducontainer.h"


#ifndef NO_MIDI_IMPORT
class MidiConversionHelper;
#endif

#ifdef Q_OS_WIN
#include "../general/duarray.h"
#else
DU_OBJECT_TEMPLATE(DuArray);
#endif
DU_OBJECT(DuEvent);
DU_OBJECT(DuMidiTrack);
DU_OBJECT(DuMusicInstrument);

DU_OBJECT(DuLoop);

class DuLoop : public DuContainer
{
public:
    DuLoop();
    virtual ~DuLoop() = default;

    virtual DuObjectPtr clone() const override;

    static DuLoopPtr fromDuMusicBinary(const music_loop &du_loop,
                                       const music_sample *du_sample);

#ifndef NO_MIDI_IMPORT
    static DuLoopPtr fromMidi(const MidiConversionHelper &helper, int midiTrackIndex);
#endif

    QByteArray toDuMusicBinary() const override;

    DuMidiTrackPtr toDuMidiTrack(int durationRef, int channel, int transpose, int trackId, int loopId) const;
    QByteArray toMidiOneLoopBinary(const DuMidiTrackPtr &tempoTrack, int durationRef, int transpose, int trackId, int loopId) const;

    int size() const override;

    virtual DuObjectPtr getChild(const QString &key) override;
    virtual DuObjectConstPtr getChild(const QString &key) const override;

    DU_KEY_ACCESSORS_IN_CHILD(NameForDevice, QString)
    DU_KEY_ACCESSORS_IN_CHILD(UserID,        int)
    DU_KEY_ACCESSORS_IN_CHILD(ID,            int)
    DU_KEY_ACCESSORS_IN_CHILD(InstrVersion,  int)

    DU_KEY_ACCESSORS(State,            int)
    DU_KEY_ACCESSORS(DurationModifier, int)
    DU_KEY_ACCESSORS(ScoreDisplay,     int)
    DU_KEY_ACCESSORS(MidiOutChannel,   int)

    DU_KEY_ACCESSORS_OBJECT(Instrument, DuMusicInstrument)

    DU_KEY_ACCESSORS_OBJECT_TEMPLATE(Events, DuArray, DuEvent)

public:
    int eventsSize() const;

    int countEvents() const;
    bool appendEvent(const DuEventPtr &event);
};

#endif // DULOOP_H
