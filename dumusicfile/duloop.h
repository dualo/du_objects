#ifndef DULOOP_H
#define DULOOP_H

#include "../general/ducontainer.h"


#define KEY_LOOP_STATE              "State"
#define KEY_LOOP_DURATIONMODIFIER   "DurationModifier"
#define KEY_LOOP_MIDIOUTCHANNEL     "MidiOutChannel"
#define KEY_LOOP_INSTRUMENT         "Instrument"
#define KEY_LOOP_EVENTS             "Events"


class MidiConversionHelper;
DU_OBJECT(DuArray)
DU_OBJECT(DuEvent)
DU_OBJECT(DuInstrument)
DU_OBJECT(DuMidiTrack)

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
    static DuLoopPtr fromMidi(const MidiConversionHelper &helper, int loopIndex);

    QByteArray toDuMusicBinary() const;
    DuMidiTrackPtr toDuMidiTrack(int durationRef, int channel) const;

    int size() const;

    int getState() const;
    bool setState(int value);

    int getDurationModifier() const;
    bool setDurationModifier(int value);

    int getMidiOutChannel() const;
    bool setMidiOutChannel(int value);

    DuInstrumentConstPtr getInstrument() const;
    void setInstrument(const DuInstrumentPtr &instrument);

    DuArrayConstPtr getEvents() const;
    void setEvents(const DuArrayPtr& array);

    int eventsSize() const;

    int countEvents() const;
    bool appendEvent(const DuEventPtr &event);
};

#endif // DULOOP_H
