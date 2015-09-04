#ifndef DULOOP_H
#define DULOOP_H

#include "../general/duarray.h"
#include "instrument/duinstrument.h"
#include "duevent.h"


#define KEY_LOOP_STATE              "State"
#define KEY_LOOP_DURATIONMODIFIER   "DurationModifier"
#define KEY_LOOP_SCOREDISPLAY       "ScoreDisplay"
#define KEY_LOOP_MIDIOUTCHANNEL     "MidiOutChannel"

#define KEY_LOOP_SAVELOOPTIMER      "SaveLoopTimer"

#define KEY_LOOP_INSTRUMENT         "Instrument"
#define KEY_LOOP_EVENTS             "Events"


DU_OBJECT(DuLoop)

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

    int getState() const;
    bool setState(int value);

    int getDurationModifier() const;
    bool setDurationModifier(int value);

    int getScoreDisplay() const;
    bool setScoreDisplay(int value);

    int getMidiOutChannel() const;
    bool setMidiOutChannel(int value);

    int getSaveLoopTimer() const;
    bool setSaveLoopTimer(int value);

    DuInstrumentConstPtr getInstrument() const;
    void setInstrument(const DuInstrumentPtr &instrument);

    DuArrayConstPtr getEvents() const;
    void setEvents(const DuArrayPtr& array);

    int eventsSize() const;

    int countEvents() const;
    bool appendEvent(const DuEventPtr &event);
};

#endif // DULOOP_H
