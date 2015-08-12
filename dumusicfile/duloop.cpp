#include "duloop.h"

#include <cstring>

#include <QJsonArray>
#include <QJsonObject>


DU_OBJECT_IMPL(DuLoop)

DuLoop::DuLoop() :
    DuContainer()
{
    addChild(KEY_LOOP_STATE,
             new DuNumeric(REC_EMPTY, NUMERIC_DEFAULT_SIZE,
                           REC_PAUSE, REC_EMPTY));

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
    if (du_loop.l_state == REC_EMPTY)
    {
        return DuLoopPtr(new DuLoop);
    }

    DuLoopPtr loop(new DuLoop);
    bool verif = true;

    const DuInstrumentPtr &instrument =
            DuInstrument::fromDuMusicBinary(du_loop.l_instr);
    if (instrument != NULL)
        loop->setInstrument(instrument);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuLoop::fromDuMusicBinary():\n"
                    << "failed to generate DuLoop\n"
                    << "the DuInstrument was not properly generated";

        return DuLoopPtr();
    }

    verif = loop->setState(du_loop.l_state) ? verif : false;
    verif = loop->setDurationModifier(du_loop.l_loopmod) ? verif : false;
    verif = loop->setMidiOutChannel(du_loop.l_midioutchannel) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuLoop::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    for (int i = 0; i < du_loop.l_numsample; i++)
    {
        const DuEventPtr &event = DuEvent::fromDuMusicBinary(du_sample[i]);
        if (event == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "DuLoop::fromDuMusicBinary():\n"
                        << "failed to generate DuLoop\n"
                        << "a DuEvent was not properly generated";

            return DuLoopPtr();
        }
        if (!loop->appendEvent(event))
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "DuLoop::fromDuMusicBinary():\n"
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
        qCCritical(LOG_CAT_DU_OBJECT) << "DuLoop::fromJson():\n"
                    << "failed to generate DuLoop\n"
                    << "a json key did not contain the proper type";

        return DuLoopPtr();
    }


    DuLoopPtr loop(new DuLoop);
    bool verif = true;

    verif = loop->setState(jsonState.toInt()) ? verif : false;
    verif = loop->setDurationModifier(jsonDurationMod.toInt()) ? verif : false;
    verif = loop->setMidiOutChannel(jsonOutChannel.toInt()) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuLoop::fromJson():\n"
                   << "an attribute was not properly set";

        return DuLoopPtr();
    }

    const DuInstrumentPtr &instrument =
            DuInstrument::fromJson(jsonInstrument.toObject());
    if (instrument != NULL)
        loop->setInstrument(instrument);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuLoop::fromJson():\n"
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
            qCCritical(LOG_CAT_DU_OBJECT) << "DuLoop::fromJson():\n"
                        << "failed to generate DuLoop\n"
                        << "a DuEvent was not properly generated";

            return DuLoopPtr();
        }
        if (!loop->appendEvent(event))
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "DuLoop::fromJson():\n"
                        << "failed to generate DuLoop\n"
                        << "a DuEvent was not properly appended";

            return DuLoopPtr();
        }
    }

    return loop;
}


DuLoopPtr DuLoop::fromMidi(const MidiConversionHelper &helper, int loopIndex)
{
    if (!helper.isValid())
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuLoop::fromMidi():\n"
                << "failed to generate DuLoop\n"
                << "invalid conversion helper";

        return DuLoopPtr();
    }


    const DuMidiTrackPtr &midiTrack = helper.getMidiTrack(loopIndex);
    if (midiTrack == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuLoop::fromMidi():\n"
                << "failed to generate DuLoop\n"
                << "DuMidiTrack is NULL";

        return DuLoopPtr();
    }

    const DuArrayPtr &midiEvents = midiTrack->getEvents();
    if (midiEvents == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuLoop::fromMidi():\n"
                << "failed to generate DuLoop\n"
                << "midi event list was NULL";

        return DuLoopPtr();
    }

    int count = midiEvents->count();
    if (count == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuLoop::fromMidi():\n"
                << "failed to generate DuLoop\n"
                << "invalid event list";

        return DuLoopPtr();
    }


    const DuInstrumentPtr &instrument = helper.getInstrument(loopIndex);
    if (instrument == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuLoop::fromMidi():\n"
                << "failed to generate DuLoop\n"
                << "DuInstrument is NULL";

        return DuLoopPtr();
    }

    const DuInstrumentInfoConstPtr &instrInfo = instrument->getInstrumentInfo();
    if (instrInfo == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuLoop::fromMidi():\n"
                << "failed to generate DuLoop\n"
                << "DuInstrumentInfo is NULL";

        return DuLoopPtr();
    }

    const DuPresetConstPtr &instrPreset = instrument->getPreset();
    if (instrPreset == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuLoop::fromMidi():\n"
                << "failed to generate DuLoop\n"
                << "DuPreset is NULL";

        return DuLoopPtr();
    }

    const DuExpressionConstPtr &presetExpr = instrPreset->getExpression();
    if (presetExpr == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuLoop::fromMidi():\n"
                << "failed to generate DuLoop\n"
                << "DuExpression is NULL";

        return DuLoopPtr();
    }


    int instrKeyMap = instrInfo->getKeyMap();
    if (instrKeyMap == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuLoop::fromMidi():\n"
                << "failed to generate DuLoop\n"
                << "invalid instrument key map:" << instrKeyMap;

        return DuLoopPtr();
    }

    int presetOctave = instrInfo->getOctave();
    if (presetOctave == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuLoop::fromMidi():\n"
                << "failed to generate DuLoop\n"
                << "invalid instrument preset octave:" << presetOctave;

        return DuLoopPtr();
    }

    //NOTE: tests can be more precise with du-touch parameters v2 and du-sounds
    bool isPercu =  instrKeyMap >= 1 && instrKeyMap <= 4;


    DuLoopPtr loop(new DuLoop);
    bool verif = true;

    loop->setInstrument(instrument);

    verif = loop->setState(REC_STOP) ? verif : false;
    verif = loop->setDurationModifier(1) ? verif : false;
    verif = loop->setMidiOutChannel(helper.getMidiChannel(loopIndex)) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                << "DuLoop::fromMidi():\n"
                << "an attribute was not properly set";
    }

    for (int i = 0; i < count; i++)
    {
        const DuObjectPtr &tmpObject = midiEvents->at(i);
        if (tmpObject == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT)
                    << "DuLoop::fromMidi():\n"
                    << "failed to generate DuLoop\n"
                    << "invalid du-object at" << i;

            return DuLoopPtr();
        }

        const DuMidiBasicEventPtr &midiEvent =
                tmpObject.dynamicCast<DuMidiBasicEvent>();
        if (midiEvent == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT)
                    << "DuLoop::fromMidi():\n"
                    << "failed to generate DuLoop\n"
                    << "invalid midi event at" << i;

            return DuLoopPtr();
        }

        const DuMidiChannelEventPtr &channelEvent
                = midiEvent.dynamicCast<DuMidiChannelEvent>();

        if (channelEvent != NULL)
        {
            if (channelEvent->getType() != DuMidiChannelEvent::ProgramChange)
            {
                const DuEventPtr &event =
                        DuEvent::fromMidi(channelEvent, presetOctave, instrKeyMap,
                                          isPercu, helper);
                if (event == NULL)
                {
                    qCWarning(LOG_CAT_DU_OBJECT)
                            << "DuLoop::fromMidi():\n"
                            << "DuEvent" << i << "not properly generated";
                }
                else if (!loop->appendEvent(event))
                {
                    qCCritical(LOG_CAT_DU_OBJECT)
                            << "DuLoop::fromMidi():\n"
                            << "failed to generate DuLoop\n"
                            << "DuEvent" << i << "not properly appended";

                    return DuLoopPtr();
                }
            }
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


    const DuInstrumentConstPtr &instrument = getInstrument();
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

DuMidiTrackPtr DuLoop::toDuMidiTrack(int durationRef, int channel,
                                     int transpose) const
{
    if (getState() == REC_EMPTY)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                << "DuLoop::toDuMidiTrack():\n"
                << "empty";

        return DuMidiTrackPtr();
    }

    int durationMod = getDurationModifier();
    if (durationMod == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuLoop::toDuMidiTrack():\n"
                << "incorrect duration modifier";

        return DuMidiTrackPtr();
    }


    const DuInstrumentConstPtr &instrument = getInstrument();
    if (instrument == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuLoop::toDuMidiTrack():\n"
                << "DuInstrument is NULL";

        return DuMidiTrackPtr();
    }

    const DuInstrumentInfoConstPtr &instrInfo = instrument->getInstrumentInfo();
    if (instrInfo == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuLoop::toDuMidiTrack():\n"
                << "DuInstrumentInfo is NULL";

        return DuMidiTrackPtr();
    }

    const DuPresetConstPtr &instrPreset = instrument->getPreset();
    if (instrPreset == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuLoop::toDuMidiTrack():\n"
                << "DuPreset is NULL";

        return DuMidiTrackPtr();
    }

    const DuExpressionConstPtr &presetExpr = instrPreset->getExpression();
    if (presetExpr == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuLoop::toDuMidiTrack():\n"
                << "DuExpression is NULL";

        return DuMidiTrackPtr();
    }


    int instrKeyMap = instrInfo->getKeyMap();
    if (instrKeyMap == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuLoop::toDuMidiTrack():\n"
                << "invalid instrument key map:" << instrKeyMap;

        return DuMidiTrackPtr();
    }

    int instrType = instrInfo->getType();
    if (instrType == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuLoop::toDuMidiTrack():\n"
                << "invalid instrument type:" << instrType;

        return DuMidiTrackPtr();
    }

    int presetOctave = presetExpr->getOctave();
    if (presetOctave == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuLoop::toDuMidiTrack():\n"
                << "invalid instrument preset octave:" << presetOctave;

        return DuMidiTrackPtr();
    }


    QString instrName = instrInfo->getName();

    //TODO: change Dream program change for GM program change when possible
    //current code produces incorrect PCs
    int instrPC = instrInfo->getDreamProgramChange();
    int instrC0 = instrInfo->getMidiControlChange0();

    bool isPercu = false;

    int midiChannel = channel;

    if (instrType == INSTR_PERCU)
    {
        if (instrKeyMap <= -1)
        {
            //NOTE: maybe a qCCritical and return DuMidiTrackPtr() would be better

            qCWarning(LOG_CAT_DU_OBJECT)
                    << "DuLoop::toDuMidiTrack():\n"
                    << "invalid mapping for percussive instrument";

            instrKeyMap = 1;
        }

        isPercu = true;
        midiChannel = 0x09;

        //GM for drum kit PC and C0
        instrPC = 0;
        instrC0 = 120;
    }


    DuMidiTrackPtr midiTrack(new DuMidiTrack);
    DuArrayPtr midiEvents(new DuArray);

    quint32 prevTime = 0;
    quint8 prevType = 0;

    if (!instrName.isEmpty())
    {
        DuMidiMetaEventPtr nameEvent(new DuMidiMetaEvent(prevTime));

        nameEvent->setInstrumentName(instrName);
        midiEvents->append(nameEvent);
    }

    if (instrPC != -1)
    {
        DuMidiChannelEventPtr pcEvent(new DuMidiChannelEvent(prevTime));

        pcEvent->setRunningStatus(false);
        pcEvent->setType(DuMidiChannelEvent::ProgramChange);
        pcEvent->setChannel((quint8)midiChannel);
        pcEvent->setValue((quint8)instrPC);

        midiEvents->append(pcEvent);
        prevType = DuMidiChannelEvent::ProgramChange;
    }

    if (instrC0 != -1)
    {
        DuMidiChannelEventPtr c0Event(new DuMidiChannelEvent(prevTime));

        c0Event->setRunningStatus(false);
        c0Event->setType(DuMidiChannelEvent::ControlChange);
        c0Event->setChannel((quint8)midiChannel);
        c0Event->setKey(0x00);
        c0Event->setValue((quint8)instrC0);

        midiEvents->append(c0Event);
        prevType = DuMidiChannelEvent::ControlChange;
    }


    const DuArrayConstPtr &events = getEvents();
    if (events == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuLoop::toDuMidiTrack():\n"
                << "DuEvent list is NULL";

        return DuMidiTrackPtr();
    }

    int count = events->count();

    for (int i = 0; i < count; i++)
    {
        const DuEventConstPtr &event = events->at(i).dynamicCast<const DuEvent>();
        if (event == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT)
                    << "DuLoop::toDuMidiTrack():\n"
                    << "DuEvent" << i << "is NULL";

            return DuMidiTrackPtr();
        }

        quint32 tmpTime = (quint32)event->getTime();
        DuMidiChannelEventPtr channelEvent;

        if (tmpTime < prevTime)
        {
            channelEvent = event->toDuMidiChannelEvent(0, prevType,
                                                       presetOctave, transpose,
                                                       isPercu, instrKeyMap);
            if (channelEvent != NULL)
            {
                channelEvent->setTime((quint32)durationRef * (quint8)durationMod
                                      + tmpTime - prevTime, prevTime);
                channelEvent->setChannel((quint8)midiChannel);
            }
        }

        else
        {
            channelEvent = event->toDuMidiChannelEvent(prevTime, prevType,
                                                       presetOctave, transpose,
                                                       isPercu, instrKeyMap);
            if (channelEvent != NULL)
            {
                channelEvent->setChannel((quint8)midiChannel);
            }
        }

        if (channelEvent == NULL)
        {
            qCWarning(LOG_CAT_DU_OBJECT)
                    << "DuLoop::toDuMidiTrack():\n"
                    << "DuEvent" << i << "was not"
                    << "properly converted to midi";
        }
        else
        {
            midiEvents->append(channelEvent);

            prevTime = channelEvent->getTime();
            prevType = channelEvent->getType();
        }
    }


    //Adding EndOfTrack midi event
    DuMidiMetaEventPtr eotEvent(new DuMidiMetaEvent(prevTime));

    eotEvent->setEndOfTrack();
    midiEvents->append(eotEvent);


    midiTrack->setEvents(midiEvents);
    return midiTrack;
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
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_LOOP_STATE);

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
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_LOOP_DURATIONMODIFIER);

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
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_LOOP_MIDIOUTCHANNEL);

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
    const DuArrayPtr &tmp = getChildAs<DuArray>(KEY_LOOP_EVENTS);

    if (tmp == NULL)
        return false;

    tmp->append(event);
    return true;
}
