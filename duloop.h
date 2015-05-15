#ifndef DULOOP_H
#define DULOOP_H

#include "duinstrument.h"
#include "duevent.h"
#include "duarray.h"


#define KEY_LOOP_STATE              "State"
#define KEY_LOOP_DURATIONMODIFIER   "DurationModifier"
#define KEY_LOOP_MIDIOUTCHANNEL     "MidiOutChannel"
#define KEY_LOOP_INSTRUMENT         "Instrument"
#define KEY_LOOP_EVENTS             "Events"


class DuLoop : public DuContainer
{
public:
    explicit DuLoop();
    ~DuLoop();

    static DuLoop *fromDuMusicFile(const music_loop &du_loop,
                                   const music_sample *du_sample);
    static DuLoop *fromJson(const QJsonObject &jsonLoop);

    int size() const;

    int getState() const;
    bool setState(int value);

    int getDurationModifier() const;
    bool setDurationModifier(int value);

    int getMidiOutChannel() const;
    bool setMidiOutChannel(int value);

    DuInstrument *getInstrument();
    void setInstrument(DuInstrument *instrument);

    DuArray *getEvents();
    void setEvents(DuArray *array);

    int countEvents() const;
};

#endif // DULOOP_H
