#include "duloop.h"

#include <cstring>

#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

DU_OBJECT_IMPL(DuLoop)

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

DuObjectPtr DuLoop::clone() const
{
    return DuLoopPtr(new DuLoop(*this));
}


DuLoopPtr DuLoop::fromDuMusicBinary(const music_loop &du_loop,
                                const music_sample *du_sample)
{
    if (du_loop.l_state == 0)
    {
        return DuLoopPtr(new DuLoop);
    }

    DuLoopPtr loop(new DuLoop);
    bool verif = true;

    verif = verif && loop->setState(du_loop.l_state);
    verif = verif && loop->setDurationModifier(du_loop.l_loopmod);
    verif = verif && loop->setMidiOutChannel(du_loop.l_midioutchannel);

    if (!verif)
    {
        qCritical() << "DuLoop::fromDuMusicBinary():\n"
                    << "an attribute was not properly set";

        return DuLoopPtr();
    }

    const DuInstrumentPtr &instrument =
            DuInstrument::fromDuMusicBinary(du_loop.l_instr);
    if (instrument == NULL)
    {
        qCritical() << "DuLoop::fromDuMusicBinary():\n"
                    << "failed to generate DuLoop\n"
                    << "the DuInstrument was not properly generated";

        return DuLoopPtr();
    }
    loop->setInstrument(instrument);

    if (du_loop.l_state == 0)
        return loop;

    for (int i = 0; i < du_loop.l_numsample; i++)
    {
        const DuEventPtr &event = DuEvent::fromDuMusicBinary(du_sample[i]);
        if (event == NULL)
        {
            qCritical() << "DuLoop::fromDuMusicBinary():\n"
                        << "failed to generate DuLoop\n"
                        << "a DuEvent was not properly generated";

            return DuLoopPtr();
        }
        if (!loop->appendEvent(event))
        {
            qCritical() << "DuLoop::fromDuMusicBinary():\n"
                        << "failed to generate DuLoop\n"
                        << "a DuEvent was not properly appended";

            return DuLoopPtr();
        }
    }

    return loop;
}


DuLoopPtr DuLoop::fromJson(const QJsonObject &jsonLoop)
{
    QJsonValue jsonState        = jsonLoop[KEY_LOOP_STATE];
    QJsonValue jsonDurationMod  = jsonLoop[KEY_LOOP_DURATIONMODIFIER];
    QJsonValue jsonOutChannel   = jsonLoop[KEY_LOOP_MIDIOUTCHANNEL];
    QJsonValue jsonInstrument   = jsonLoop[KEY_LOOP_INSTRUMENT];
    QJsonValue jsonEvents       = jsonLoop[KEY_LOOP_EVENTS];

    if (        !jsonState.isDouble()       ||  !jsonDurationMod.isDouble()
            ||  !jsonOutChannel.isDouble()  ||  !jsonInstrument.isObject()
            ||  !jsonEvents.isArray())
    {
        qCritical() << "DuLoop::fromJson():\n"
                    << "failed to generate DuLoop\n"
                    << "a json key did not contain the proper type";

        return DuLoopPtr();
    }


    DuLoopPtr loop(new DuLoop);
    bool verif = true;

    verif = verif && loop->setState(jsonState.toInt());
    verif = verif && loop->setDurationModifier(jsonDurationMod.toInt());
    verif = verif && loop->setMidiOutChannel(jsonOutChannel.toInt());

    if (!verif)
    {
        qWarning() << "DuLoop::fromJson():\n"
                   << "an attribute was not properly set";

        return DuLoopPtr();
    }

    const DuInstrumentPtr &instrument =
            DuInstrument::fromJson(jsonInstrument.toObject());
    if (instrument != NULL)
        loop->setInstrument(instrument);
    else
    {
        qCritical() << "DuLoop::fromJson():\n"
                    << "failed to generate DuLoop\n"
                    << "the DuInstrument was not properly generated";

        return DuLoopPtr();
    }

    const QJsonArray &jsonEventArray = jsonEvents.toArray();
    for (int i = 0; i < jsonEventArray.count(); i++)
    {
        const DuEventPtr &event =
                DuEvent::fromJson(jsonEventArray[i].toObject());
        if (event == NULL)
        {
            qCritical() << "DuLoop::fromJson():\n"
                        << "failed to generate DuLoop\n"
                        << "a DuEvent was not properly generated";

            return DuLoopPtr();
        }
        if (!loop->appendEvent(event))
        {
            qCritical() << "DuLoop::fromJson():\n"
                        << "failed to generate DuLoop\n"
                        << "a DuEvent was not properly appended";

            return DuLoopPtr();
        }
    }

    return loop;
}


QByteArray DuLoop::toDuMusicBinary() const
{
    music_loop du_loop;

    int tmpNum = 0;

    QByteArray tmpClear(size(), (char)0x00);
    std::memcpy((char *)&(du_loop), tmpClear.data(), size());


    const DuInstrumentConstPtr& instrument = getInstrument();
    if (instrument == NULL)
        return QByteArray();
    const QByteArray &instrumentArray = instrument->toDuMusicBinary();
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
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_LOOP_STATE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuLoop::setState(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_LOOP_STATE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuLoop::getDurationModifier() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_LOOP_DURATIONMODIFIER);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuLoop::setDurationModifier(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_LOOP_DURATIONMODIFIER);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuLoop::getMidiOutChannel() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_LOOP_MIDIOUTCHANNEL);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuLoop::setMidiOutChannel(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_LOOP_MIDIOUTCHANNEL);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


DuInstrumentConstPtr DuLoop::getInstrument() const
{
    return getChildAs<DuInstrument>(KEY_LOOP_INSTRUMENT);
}

void DuLoop::setInstrument(const DuInstrumentPtr &instrument)
{
    addChild(KEY_LOOP_INSTRUMENT, instrument);
}

DuArrayConstPtr DuLoop::getEvents() const
{
    return getChildAs<DuArray>(KEY_LOOP_EVENTS);
}

void DuLoop::setEvents(const DuArrayPtr &array)
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
    const DuArrayConstPtr &tmp = getChildAs<DuArray>(KEY_LOOP_EVENTS);

    if (tmp == NULL)
        return -1;

    return tmp->count();
}

bool DuLoop::appendEvent(const DuEventPtr &event)
{
    DuArrayPtr tmp = getChildAs<DuArray>(KEY_LOOP_EVENTS);

    if (tmp == NULL)
        return false;

    tmp->append(event);
    return true;
}
