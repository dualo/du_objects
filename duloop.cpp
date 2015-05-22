#include "duloop.h"

DuLoop::DuLoop() :
    DuContainer()
{
    addChild(KEY_LOOP_STATE,
             new DuNumeric(0, NUMERIC_DEFAULT_SIZE,
                           3, 0));

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
    if (instrument == NULL)
    {
        delete loop;
        return NULL;
    }
    loop->setInstrument(instrument);

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
    std::memcpy((char *)&(du_loop), tmpClear.data(), size());


    QSharedPointer<DuInstrument> instrument = getInstrument();
    if (instrument == NULL)
        return QByteArray();
    const QByteArray &instrumentArray = instrument->toDuMusicFile();
    if (instrumentArray.isNull())
        return QByteArray();

    std::memcpy(&(du_loop.l_instr), instrumentArray.data(), instrument->size());


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
    QSharedPointer<DuNumeric> tmp = getChildAs<DuNumeric>(KEY_LOOP_STATE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuLoop::setState(int value)
{
    QSharedPointer<DuNumeric> tmp = getChildAs<DuNumeric>(KEY_LOOP_STATE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuLoop::getDurationModifier() const
{
    QSharedPointer<DuNumeric> tmp = getChildAs<DuNumeric>(KEY_LOOP_DURATIONMODIFIER);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuLoop::setDurationModifier(int value)
{
    QSharedPointer<DuNumeric> tmp = getChildAs<DuNumeric>(KEY_LOOP_DURATIONMODIFIER);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuLoop::getMidiOutChannel() const
{
    QSharedPointer<DuNumeric> tmp = getChildAs<DuNumeric>(KEY_LOOP_MIDIOUTCHANNEL);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuLoop::setMidiOutChannel(int value)
{
    QSharedPointer<DuNumeric> tmp = getChildAs<DuNumeric>(KEY_LOOP_MIDIOUTCHANNEL);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


QSharedPointer<DuInstrument> DuLoop::getInstrument() const
{
    return getChildAs<DuInstrument>(KEY_LOOP_INSTRUMENT);
}

void DuLoop::setInstrument(DuInstrument *instrument)
{
    addChild(KEY_LOOP_INSTRUMENT, instrument);
}

QSharedPointer<DuArray> DuLoop::getEvents()
{
    return getChildAs<DuArray>(KEY_LOOP_EVENTS);
}

void DuLoop::setEvents(DuArray *array)
{
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
    QSharedPointer<DuArray> tmp = getChildAs<DuArray>(KEY_LOOP_EVENTS);

    if (tmp == NULL)
        return -1;

    return tmp->count();
}

bool DuLoop::appendEvent(DuEvent *event)
{
    QSharedPointer<DuArray> tmp = getChildAs<DuArray>(KEY_LOOP_EVENTS);

    if (tmp == NULL)
        return false;

    tmp->append(event);
    return true;
}
