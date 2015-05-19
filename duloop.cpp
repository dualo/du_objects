#include "duloop.h"

DuLoop::DuLoop() :
    DuContainer()
{
    addChild(KEY_LOOP_STATE,
             new DuNumeric(0, NUMERIC_DEFAULT_SIZE,
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
    DuLoop *loop = new DuLoop;
    bool verif = true;

    verif = verif && loop->setState(du_loop.l_state);
    verif = verif && loop->setDurationModifier(du_loop.l_loopmod);
    verif = verif && loop->setMidiOutChannel(du_loop.l_midioutchannel);

    if (!verif)
    {
        delete loop;
        return NULL;
    }

    DuInstrument *instrument = DuInstrument::fromDuMusicFile(du_loop.l_instr);
    if (instrument != NULL)
        loop->setInstrument(instrument);
    else
    {
        delete loop;
        return NULL;
    }

    if (du_loop.l_state == 0)
        return loop;

    for (int i = 0; i < du_loop.l_numsample; i++)
    {
        DuEvent *event = DuEvent::fromDuMusicFile(du_sample[i]);
        if (event == NULL)
        {
            delete loop;
            return NULL;
        }
        if (!loop->appendEvent(event))
        {
            delete loop;
            return NULL;
        }
    }

    return loop;
}


DuLoop *DuLoop::fromJson(const QJsonObject &jsonLoop)
{
    QJsonValue jsonState        = jsonLoop[KEY_LOOP_STATE];
    QJsonValue jsonDurationMod  = jsonLoop[KEY_LOOP_DURATIONMODIFIER];
    QJsonValue jsonOutChannel   = jsonLoop[KEY_LOOP_MIDIOUTCHANNEL];
    QJsonValue jsonInstrument   = jsonLoop[KEY_LOOP_INSTRUMENT];
    QJsonValue jsonEvents       = jsonLoop[KEY_LOOP_EVENTS];

    if (        !jsonState.isDouble()       ||  !jsonDurationMod.isDouble()
            ||  !jsonOutChannel.isDouble()  ||  !jsonInstrument.isObject()
            ||  !jsonEvents.isArray())

        return NULL;


    DuLoop *loop = new DuLoop;
    bool verif = true;

    verif = verif && loop->setState(jsonState.toInt());
    verif = verif && loop->setDurationModifier(jsonDurationMod.toInt());
    verif = verif && loop->setMidiOutChannel(jsonOutChannel.toInt());

    if (!verif)
    {
        delete loop;
        return NULL;
    }

    DuInstrument *instrument = DuInstrument::fromJson(jsonInstrument.toObject());
    if (instrument != NULL)
        loop->setInstrument(instrument);
    else
    {
        delete loop;
        return NULL;
    }

    const QJsonArray &jsonEventArray = jsonEvents.toArray();
    for (int i = 0; i < jsonEventArray.count(); i++)
    {
        DuEvent *event = DuEvent::fromJson(jsonEventArray[i].toObject());
        if (event == NULL)
        {
            delete loop;
            return NULL;
        }
        if (!loop->appendEvent(event))
        {
            delete loop;
            return NULL;
        }
    }

    return loop;
}


QByteArray DuLoop::toDuMusicFile() const
{
    music_loop du_loop;

    int tmpNum = 0;

    QByteArray tmpClear(size(), (char)0x00);
#ifdef Q_OS_WIN
    memcpy_s((char *)&(du_loop), size(), tmpClear.data(), size());
#else
    memcpy((char *)&(du_loop), tmpClear.data(), size());
#endif


    DuInstrument *instrument = getInstrument();
    if (instrument == NULL)
        return QByteArray();
    QByteArray &instrumentArray = instrument->toDuMusicFile();
    if (instrumentArray.isNull())
        return QByteArray();

#ifdef Q_OS_WIN
    memcpy_s(&(du_loop.l_instr), instrument->size(),
             instrumentArray.data(), instrument->size());
#else
    memcpy(du_loop.l_instr, instrumentArray.data(), instrument->size());
#endif


    tmpNum = getState();
    if (tmpNum == -1)
        return QByteArray();
    du_loop.l_state = tmpNum;

    tmpNum = getDurationModifier();
    if (tmpNum == -1)
        return QByteArray();
    du_loop.l_loopmod = tmpNum;

    tmpNum = getMidiOutChannel();
    if (tmpNum == -1)
        return QByteArray();
    du_loop.l_midioutchannel = tmpNum;

    tmpNum = countEvents();
    if(tmpNum == -1)
        return QByteArray();
    du_loop.l_numsample = tmpNum;


    return QByteArray((char *)&(du_loop), size());
}


int DuLoop::size() const
{
    return MUSIC_LOOP_SIZE;
}


int DuLoop::getState() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_LOOP_STATE));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuLoop::setState(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_LOOP_STATE));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuLoop::getDurationModifier() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_LOOP_DURATIONMODIFIER));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuLoop::setDurationModifier(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_LOOP_DURATIONMODIFIER));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuLoop::getMidiOutChannel() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_LOOP_MIDIOUTCHANNEL));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuLoop::setMidiOutChannel(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_LOOP_MIDIOUTCHANNEL));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


DuInstrument *DuLoop::getInstrument() const
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


int DuLoop::eventsSize() const
{
    int numEvents = countEvents();

    if (numEvents == -1)
        return -1;

    return MUSIC_SAMPLE_SIZE * numEvents;
}

int DuLoop::countEvents() const
{
    DuArray *tmp = dynamic_cast<DuArray *>(getChild(KEY_LOOP_EVENTS));

    if (tmp == NULL)
        return -1;

    return tmp->count();
}

bool DuLoop::appendEvent(DuEvent *event)
{
    DuArray *tmp = dynamic_cast<DuArray *>(getChild(KEY_LOOP_EVENTS));

    if (tmp == NULL)
        return false;

    tmp->append(event);
    return true;
}
