#include "duloop.h"

DuLoop::DuLoop() :
    DuContainer()
{
    addChild(KEY_LOOP_STATE,
             new DuNumeric(1, NUMERIC_DEFAULT_SIZE,
                           2, 0));

    addChild(KEY_LOOP_DURATIONMODIFIER,
             new DuNumeric(MUSIC_LOOPMOD_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           MUSIC_LOOPMOD_MAXVALUE, MUSIC_LOOPMOD_MINVALUE));

    addChild(KEY_LOOP_MIDIOUTCHANNEL,
             new DuNumeric(0, NUMERIC_DEFAULT_SIZE,
                           15, 0));

    addChild(KEY_LOOP_INSTRUMENT, new DuInstrument());

    addChild(KEY_LOOP_EVENTS, new DuArray(RECORD_SAMPLEBUFFERSIZE));
}

DuLoop::~DuLoop()
{
}


DuLoop *DuLoop::fromDuMusicFile(const music_loop &du_loop,
                                const music_sample *du_sample)
{
    if (du_loop.l_state == 0)
        return NULL;

    DuLoop *loop = new DuLoop();

    loop->setState(du_loop.l_state);
    loop->setDurationModifier(du_loop.l_loopmod);
    loop->setMidiOutChannel(du_loop.l_midioutchannel);

    loop->setInstrument(DuInstrument::fromDuMusicFile(du_loop.l_instr));

    DuArray *events = loop->getEvents();

    for (int i = 0; i < du_loop.l_numsample; i++)
    {
        events->append(DuEvent::fromDuMusicFile(du_sample[i]));
    }

    return loop;
}


DuLoop *DuLoop::fromJson(const QJsonObject &jsonLoop)
{
    DuLoop *loop = new DuLoop();
    QStringList &keyList = loop->keys();

    bool test = true;
    for (int i = 0; i < keyList.count(); i++)
    {
        test = test && jsonLoop.contains(keyList[i]);
    }

    if (!test)
        return NULL;

    loop->setState(jsonLoop[KEY_LOOP_STATE].toInt());
    loop->setDurationModifier(jsonLoop[KEY_LOOP_DURATIONMODIFIER].toInt());
    loop->setMidiOutChannel(jsonLoop[KEY_LOOP_MIDIOUTCHANNEL].toInt());

    loop->setInstrument(
                DuInstrument::fromJson(jsonLoop[KEY_LOOP_INSTRUMENT].toObject()));

    DuArray *events = loop->getEvents();
    QJsonArray &jsonEvents = jsonLoop[KEY_LOOP_EVENTS].toArray();

    for (int i = 0; i < jsonEvents.count(); i++)
    {
        DuEvent *event = DuEvent::fromJson(jsonEvents[i].toObject());
        if (event != NULL)
            events->append(event);
    }

    return loop;
}


int DuLoop::getState() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_LOOP_STATE));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuLoop::setState(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_LOOP_STATE));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuLoop::getDurationModifier() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_LOOP_DURATIONMODIFIER));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuLoop::setDurationModifier(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_LOOP_DURATIONMODIFIER));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuLoop::getMidiOutChannel() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_LOOP_MIDIOUTCHANNEL));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuLoop::setMidiOutChannel(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_LOOP_MIDIOUTCHANNEL));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


DuInstrument *DuLoop::getInstrument()
{
    return dynamic_cast<DuInstrument *>(getChild(KEY_LOOP_INSTRUMENT));
}

void DuLoop::setInstrument(DuInstrument *instrument)
{
    if (getChild(KEY_LOOP_INSTRUMENT) != NULL)
        delete getChild(KEY_LOOP_INSTRUMENT);

    addChild(KEY_LOOP_INSTRUMENT, instrument);
}

DuArray *DuLoop::getEvents()
{
    return dynamic_cast<DuArray *>(getChild(KEY_LOOP_EVENTS));
}

void DuLoop::setEvents(DuArray *array)
{
    if (getChild(KEY_LOOP_EVENTS) != NULL)
        delete getChild(KEY_LOOP_EVENTS);

    addChild(KEY_LOOP_EVENTS, array);
}
