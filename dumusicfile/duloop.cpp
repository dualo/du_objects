#include "duloop.h"

#include "duevent.h"
#include "dumusicinstrument.h"

#include <cstring>

#include <QJsonArray>
#include <QJsonObject>

#include "../dusoundfile/dusound.h"
#include "../dusoundfile/dusoundinfo.h"

#include "../general/duarray.h"
#include "../general/dunumeric.h"

#include "../instrument/dupreset.h"
#include "../instrument/duinstrumentinfo.h"

#ifndef NO_MIDI_IMPORT
#include "../miditodumusic/midiconversionhelper.h"
#endif

#include "../midifile/dumidichannelevent.h"
#include "../midifile/dumidimetaevent.h"
#include "../midifile/dumiditrack.h"


DU_OBJECT_IMPL(DuLoop)

DuLoop::DuLoop() :
    DuContainer()
{
    addChild(KeyState,
             new DuNumeric(REC_EMPTY, NUMERIC_DEFAULT_SIZE,
                           REC_STATE_NUM - 1, REC_EMPTY));

    addChild(KeyDurationModifier,
             new DuNumeric(MUSIC_LOOPMOD_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           MUSIC_LOOPMOD_MAXVALUE, MUSIC_LOOPMOD_MINVALUE));

    addChild(KeyScoreDisplay,
             new DuNumeric(LEARN_OFF, NUMERIC_DEFAULT_SIZE,
                           NUM_LEARNMODE - 1, LEARN_OFF));

    addChild(KeyMidiOutChannel,
             new DuNumeric(0, NUMERIC_DEFAULT_SIZE,
                           15, 0));


    addChild(KeyInstrument, new DuMusicInstrument());

    addChild(KeyEvents, new DuArray<DuEvent>(RECORD_SAMPLEBUFFERSIZE));
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

    const DuMusicInstrumentPtr &instrument =
            DuMusicInstrument::fromDuMusicBinary(du_loop.l_instr);
    if (instrument != NULL)
        loop->setInstrument(instrument);
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuLoop::fromDuMusicBinary():\n"
                << "failed to generate DuLoop\n"
                << "the DuMusicInstrument was not properly generated";

        return DuLoopPtr();
    }

    verif = loop->setState(du_loop.l_state) ? verif : false;
    verif = loop->setDurationModifier(du_loop.l_loopmod) ? verif : false;
    verif = loop->setScoreDisplay(du_loop.l_learn) ? verif : false;
    verif = loop->setMidiOutChannel(du_loop.l_midioutchannel) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                << "DuLoop::fromDuMusicBinary():\n"
                << "an attribute was not properly set";
    }

    DuArrayPtr<DuEvent> eventsArray(new DuArray<DuEvent>(RECORD_SAMPLEBUFFERSIZE));
    for (int i = 0; i < du_loop.l_numsample; i++)
    {
        const DuEventPtr &event = DuEvent::fromDuMusicBinary(du_sample[i]);
        if (event == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT)
                    << "DuLoop::fromDuMusicBinary():\n"
                    << "failed to generate DuLoop\n"
                    << "a DuEvent was not properly generated";

            return DuLoopPtr();
        }
        if (!eventsArray->append(event))
        {
            qCCritical(LOG_CAT_DU_OBJECT)
                    << "DuLoop::fromDuMusicBinary():\n"
                    << "failed to generate DuLoop\n"
                    << "a DuEvent was not properly appended";

            return DuLoopPtr();
        }
    }
    loop->setEvents(eventsArray);

    return loop;
}


#ifndef NO_MIDI_IMPORT
DuLoopPtr DuLoop::fromMidi(const MidiConversionHelper &helper, int midiTrackIndex)
{
    if (!helper.isValid())
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "failed to generate DuLoop\n"
                << "invalid conversion helper";

        return DuLoopPtr();
    }


    const DuMidiTrackPtr &midiTrack = helper.getMidiTrack(midiTrackIndex);
    if (midiTrack == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "failed to generate DuLoop\n"
                << "DuMidiTrack at index" << midiTrackIndex << "is NULL";

        return DuLoopPtr();
    }

    const DuArrayPtr<DuMidiBasicEvent> &midiEvents = midiTrack->getEvents();
    if (midiEvents == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "failed to generate DuLoop\n"
                << "midi event list was NULL";

        return DuLoopPtr();
    }

    int count = midiEvents->count();
    if (count == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "failed to generate DuLoop\n"
                << "invalid event list";

        return DuLoopPtr();
    }


    const DuSoundPtr &sound = helper.getSound(midiTrackIndex);
    if (sound == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "failed to generate DuLoop\n"
                << "DuSound is NULL";

        return DuLoopPtr();
    }

    const DuSoundInfoConstPtr& soundInfo = sound->getInfo();
    if (soundInfo == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "failed to generate DuLoop\n"
                << "DuSoundInfo is NULL";

        return DuLoopPtr();
    }

    const DuInstrumentInfoConstPtr &instrInfo = soundInfo->getInstrumentInfo();
    if (instrInfo == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "failed to generate DuLoop\n"
                << "DuInstrumentInfo is NULL";

        return DuLoopPtr();
    }

    const DuPresetConstPtr &instrPreset = soundInfo->getPresetArray()->at(0);
    if (instrPreset == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "failed to generate DuLoop\n"
                << "DuPreset is NULL";

        return DuLoopPtr();
    }

    DuMusicInstrumentPtr instrument(new DuMusicInstrument);
    instrument->setInstrumentInfo(instrInfo->cloneAs<DuInstrumentInfo>());
    instrument->setPreset(instrPreset->cloneAs<DuPreset>());

    if (instrument == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "failed to generate DuLoop\n"
                << "DuMusicInstrument is NULL";

        return DuLoopPtr();
    }


    DuLoopPtr loop(new DuLoop);
    bool verif = true;

    loop->setInstrument(instrument);

    verif = loop->setState(REC_STOP) ? verif : false;
    verif = loop->setDurationModifier(1) ? verif : false;
    verif = loop->setScoreDisplay(LEARN_OFF) ? verif : false;
    verif = loop->setMidiOutChannel(helper.getMidiChannel(midiTrackIndex)) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "an attribute was not properly set";
    }

    for (int i = 0; i < count; i++)
    {
        const DuObjectPtr &tmpObject = midiEvents->at(i);
        if (tmpObject == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT)
                    << "failed to generate DuLoop\n"
                    << "invalid du-object at" << i;

            return DuLoopPtr();
        }

        const DuMidiBasicEventPtr &midiEvent =
                tmpObject.dynamicCast<DuMidiBasicEvent>();
        if (midiEvent == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT)
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
                const DuEventPtr &event = DuEvent::fromMidi(channelEvent, sound, helper);
                if (event == NULL)
                {
                    qCWarning(LOG_CAT_DU_OBJECT) << "DuEvent" << i << "not properly generated";
                }
                else if (!loop->appendEvent(event))
                {
                    qCCritical(LOG_CAT_DU_OBJECT)
                            << "failed to generate DuLoop\n"
                            << "DuEvent" << i << "not properly appended";

                    return DuLoopPtr();
                }
            }
        }
    }

    return loop;
}
#endif


QByteArray DuLoop::toDuMusicBinary() const
{
    music_loop du_loop;

    int tmpNum = 0;

    std::memset(&du_loop, 0, static_cast<size_t>(size()));


    const DuMusicInstrumentConstPtr &instrument = getInstrument();
    if (instrument == NULL)
        return QByteArray();

    const QByteArray &instrumentArray = instrument->toDuMusicBinary();
    if (instrumentArray.isNull())
        return QByteArray();

    std::memcpy(&(du_loop.l_instr), instrumentArray.data(), static_cast<size_t>(instrument->size()));


    tmpNum = getState();
    if (tmpNum == -1)
        return QByteArray();
    du_loop.l_state = static_cast<quint8>(tmpNum);

    tmpNum = getDurationModifier();
    if (tmpNum == -1)
        return QByteArray();
    du_loop.l_loopmod = static_cast<quint8>(tmpNum);

    tmpNum = getScoreDisplay();
    if (tmpNum == -1)
        return QByteArray();
    du_loop.l_learn = static_cast<quint8>(tmpNum);

    tmpNum = getMidiOutChannel();
    if (tmpNum == -1)
        return QByteArray();
    du_loop.l_midioutchannel = static_cast<quint8>(tmpNum);

    tmpNum = countEvents();
    if(tmpNum == -1)
        return QByteArray();
    du_loop.l_numsample = static_cast<quint16>(tmpNum);


    return QByteArray(reinterpret_cast<char*>(&du_loop), size());
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


    const DuMusicInstrumentConstPtr &instrument = getInstrument();
    if (instrument == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuLoop::toDuMidiTrack():\n"
                << "DuMusicInstrument is NULL";

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


    int instrKeyMap = instrInfo->getKeyMapping();
    if (instrKeyMap == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuLoop::toDuMidiTrack():\n"
                << "invalid instrument key map:" << instrKeyMap;

        return DuMidiTrackPtr();
    }

    int instrType = instrInfo->getInstrType();
    if (instrType == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuLoop::toDuMidiTrack():\n"
                << "invalid instrument type:" << instrType;

        return DuMidiTrackPtr();
    }

    int presetOctave = instrPreset->getOctave();
    if (presetOctave == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuLoop::toDuMidiTrack():\n"
                << "invalid instrument preset octave:" << presetOctave;

        return DuMidiTrackPtr();
    }


    QString instrName = instrInfo->getNameForDevice();

    //TODO: These infos must be given by the user
//    int instrPC = instrInfo->getDreamProgramChange();
//    int instrC0 = instrInfo->getMidiControlChange0();
    int instrPC = 0;
    int instrC0 = 0;

    bool isPercu = false;

    int midiChannel = channel;

    if (instrType == INSTR_PERCU)
    {
        if (instrKeyMap <= 0)
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
    DuArrayPtr<DuMidiBasicEvent> midiEvents(new DuArray<DuMidiBasicEvent>);

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
        pcEvent->setChannel(static_cast<quint8>(midiChannel));
        pcEvent->setValue(static_cast<quint8>(instrPC));

        midiEvents->append(pcEvent);
        prevType = DuMidiChannelEvent::ProgramChange;
    }

    if (instrC0 != -1)
    {
        DuMidiChannelEventPtr c0Event(new DuMidiChannelEvent(prevTime));

        c0Event->setRunningStatus(false);
        c0Event->setType(DuMidiChannelEvent::ControlChange);
        c0Event->setChannel(static_cast<quint8>(midiChannel));
        c0Event->setKey(0x00);
        c0Event->setValue(static_cast<quint8>(instrC0));

        midiEvents->append(c0Event);
        prevType = DuMidiChannelEvent::ControlChange;
    }


    const DuArrayConstPtr<DuEvent> &events = getEvents();
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

        quint32 tmpTime = static_cast<quint32>(event->getTime());
        DuMidiChannelEventPtr channelEvent;

        if (tmpTime < prevTime)
        {
            channelEvent = event->toDuMidiChannelEvent(0, prevType,
                                                       presetOctave, transpose,
                                                       isPercu, instrKeyMap);
            if (channelEvent != NULL)
            {
                channelEvent->setTime(static_cast<quint32>(durationRef) * static_cast<quint8>(durationMod)
                                      + tmpTime - prevTime, prevTime);
            }
        }

        else
        {
            channelEvent = event->toDuMidiChannelEvent(prevTime, prevType,
                                                       presetOctave, transpose,
                                                       isPercu, instrKeyMap);
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
            channelEvent->setChannel(static_cast<quint8>(midiChannel));

            midiEvents->append(channelEvent);

            prevTime = static_cast<quint32>(channelEvent->getTime());
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

DuObjectPtr DuLoop::getChild(const QString &key)
{
    if (key == KeyNameForDevice)
    {
        DuMusicInstrumentPtr instrument = getInstrument();
        if (instrument == NULL)
        {
            return DuObjectPtr();
        }

        return instrument->getChild(key);
    }
    else
    {
        return DuContainer::getChild(key);
    }
}

DuObjectConstPtr DuLoop::getChild(const QString &key) const
{
    if (key == KeyNameForDevice)
    {
        DuMusicInstrumentConstPtr instrument = getInstrument();
        if (instrument == NULL)
        {
            return DuObjectPtr();
        }

        return instrument->getChild(key);
    }
    else
    {
        return DuContainer::getChild(key);
    }
}

DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuLoop, NameForDevice, DuMusicInstrument, Instrument, QString, QString())
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuLoop, UserID,        DuMusicInstrument, Instrument, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuLoop, ID,            DuMusicInstrument, Instrument, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuLoop, InstrVersion,  DuMusicInstrument, Instrument, int, -1)

DU_KEY_ACCESSORS_IMPL(DuLoop, State,            Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuLoop, DurationModifier, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuLoop, ScoreDisplay,     Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuLoop, MidiOutChannel,   Numeric, int, -1)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuLoop, Instrument, DuMusicInstrument)

DU_KEY_ACCESSORS_OBJECT_TEMPLATE_IMPL(DuLoop, Events, DuArray, DuEvent)


int DuLoop::eventsSize() const
{
    int numEvents = countEvents();

    if (numEvents == -1)
        return -1;

    return MUSIC_SAMPLE_SIZE * numEvents;
}

int DuLoop::countEvents() const
{
    const DuArrayConstPtr<DuEvent> &tmp = getEvents();

    if (tmp == NULL)
        return -1;

    return tmp->count();
}

bool DuLoop::appendEvent(const DuEventPtr &event)
{
    const DuArrayPtr<DuEvent> &tmp = getEvents();

    if (tmp == NULL)
        return false;

    tmp->append(event);
    return true;
}
