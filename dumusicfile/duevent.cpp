#include "duevent.h"

#include <cstring>

#include <QJsonObject>

#include "../dusoundfile/dunote.h"
#include "../dusoundfile/dusound.h"

#include "../general/duarray.h"
#include "../general/dunumeric.h"

#include "../miditodumusic/midiconversionhelper.h"

#include "../midifile/dumidichannelevent.h"


DU_OBJECT_IMPL(DuEvent)


DuEvent::DuEvent() :
    DuContainer()
{
    addChild(KeyTime,
             new DuNumeric(0));

    addChild(KeyControl,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x06, 0x00));

    addChild(KeyCanal,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0xFF, 0x00));

    addChild(KeyKeyboard,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x80, 0x00));

    addChild(KeyNote,
             new DuNumeric(0x40, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KeyValue,
             new DuNumeric(0x40, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));
}

DuObjectPtr DuEvent::clone() const
{
    return DuEventPtr(new DuEvent(*this));
}


DuEventPtr DuEvent::fromDuMusicBinary(const music_sample &du_sample)
{
    DuEventPtr event(new DuEvent);
    bool verif = true;

    verif = event->setTime(static_cast<const int>(du_sample.time)) ? verif : false;
    verif = event->setControl(du_sample.control) ? verif : false;
    verif = event->setCanal(du_sample.canal) ? verif : false;
    verif = event->setKeyboard(du_sample.note & 0x80) ? verif : false;
    verif = event->setNote(du_sample.note & 0x7F) ? verif : false;
    verif = event->setValue(du_sample.value & 0x7F) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                << "DuEvent::fromDuMusicBinary():\n"
                << "an attribute was not properly set";
    }

    return event;
}

DuEventPtr DuEvent::fromMidi(const DuMidiChannelEventPtr &channelEvent,
                             const DuSoundConstPtr& sound,
                             const MidiConversionHelper &helper)
{
    //This case should not occur
    //Already tested in DuMusic::fromMidi();
    if (!helper.isValid())
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuEvent::fromMidi():\n"
                << "failed to generate DuEvent\n"
                << "invalid conversion helper";

        return DuEventPtr();
    }

    int midiType = channelEvent->getType();
    if (midiType < 0x08)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuEvent::fromMidi():\n"
                << "invalid MIDI event type" << midiType;

        return DuEventPtr();
    }

    //This case should not occur since it is already tested in DuLoop::fromMidi()
    if (midiType == DuMidiChannelEvent::ProgramChange)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuEvent::fromMidi():\n"
                << "failed to generate DuEvent\n"
                << "invalid channel event type";

        return DuEventPtr();
    }

    DuEventPtr event(new DuEvent);

    int midiKey = channelEvent->getKey();
    if (midiKey == -1)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                << "DuEvent::fromMidi():\n"
                << "invalid midi key:" << midiKey;

        return DuEventPtr();
    }

    int tmpValue = channelEvent->getValue();
    if (tmpValue == -1)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                << "DuEvent::fromMidi():\n"
                << "invalid midi key value:" << tmpValue;

        return DuEventPtr();
    }

    //Converting event in case of note off written as note on with velocity 0
    if (midiType == DuMidiChannelEvent::NoteOn
            && channelEvent->getValue() == 0)
    {
        midiType = DuMidiChannelEvent::NoteOff;
        tmpValue = 0x40;
    }

    int keyboard = 0;
    int key = -1;

    bool verif = true;

    if (sound->getInstrType() == INSTR_PERCU)
    {
        if (midiType <= DuMidiChannelEvent::KeyAftertouch)
        {
            const DuArrayConstPtr<DuNote>& mapping = sound->getMapping();
            int mappingSize = mapping->count();
            for (int i = 0; i < mappingSize; ++i)
            {
                const DuNoteConstPtr& note = mapping->at(i);
                if (note != NULL && note->getNoteGM() == midiKey)
                {
                    key = note->getNote();
                }
            }

            if (key == -1)
            {
                qCWarning(LOG_CAT_DU_OBJECT)
                        << "DuEvent::fromMidi():\n"
                        << "key not found in this mapping";

                return DuEventPtr();
            }
        }
    }
    else if (midiType <= DuMidiChannelEvent::KeyAftertouch)
    {
        //KEY_MUSIC_TRANSPOSE set to default when importing from Midi
        key = midiKey - 12 * sound->getOctave();
        if (key < 0)
        {
            qCWarning(LOG_CAT_DU_OBJECT)
                    << "DuEvent::fromMidi():\n"
                    << "invalid midi key:" << midiKey;

            return DuEventPtr();
        }

        keyboard = helper.getKeyboardFromMidi(static_cast<quint8>(key));
    }

    verif = event->setKeyboard(keyboard) ? verif : false;
    verif = event->setNote(key) ? verif : false;

    verif = event->setTime(channelEvent->getTime()) ? verif : false;
    verif = event->setControl(midiType - 0x08) ? verif : false;
    verif = event->setValue(tmpValue) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                << "DuEvent::fromMidi():\n"
                << "an attribute was not properly set";
    }

    return event;
}


QByteArray DuEvent::toDuMusicBinary() const
{
    music_sample du_sample;
    std::memset(&du_sample, 0, static_cast<size_t>(size()));

    int tmpNum = 0;
    int tmpKbrd = 0;


    tmpNum = getTime();
    if (tmpNum == -1)
        return QByteArray();
    du_sample.time = static_cast<quint32>(tmpNum);

    tmpNum = getControl();
    if (tmpNum == -1)
        return QByteArray();
    du_sample.control = static_cast<quint8>(tmpNum);

    tmpNum = getCanal();
    if (tmpNum == -1)
        return QByteArray();
    du_sample.canal = static_cast<quint8>(tmpNum);

    tmpKbrd = getKeyboard();
    if (tmpKbrd == -1)
        return QByteArray();
    tmpNum = getNote();
    if (tmpNum == -1)
        return QByteArray();
    du_sample.note = static_cast<quint8>(tmpKbrd) + static_cast<quint8>(tmpNum);

    tmpNum = getValue();
    if (tmpNum == -1)
        return QByteArray();
    du_sample.value = static_cast<quint8>(tmpNum);


    return QByteArray(reinterpret_cast<char*>(&du_sample), MUSIC_SAMPLE_SIZE);
}

DuMidiChannelEventPtr DuEvent::toDuMidiChannelEvent(quint32 prevTime,
                                                    quint8 prevType,
                                                    int presetOctave,
                                                    int transpose,
                                                    bool isPercu,
                                                    int instrKeyMap) const
{
    //This case should not occur
    //Already tested in DuLoop::toDuMidiTrack()
    if (isPercu && instrKeyMap <= 0)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuEvent::toDuMidiChannelEvent():\n"
                << "invalid mapping for a percussive type instrument";

        return DuMidiChannelEventPtr();
    }

    DuMidiChannelEventPtr channelEvent(new DuMidiChannelEvent);
    int tmp = 0;


    tmp = getTime();

    if (tmp == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuEvent::toDuMidiChannelEvent():\n"
                << "invalid time:" << tmp;

        return DuMidiChannelEventPtr();
    }

    channelEvent->setTime(static_cast<quint32>(tmp) - prevTime, prevTime);


    tmp = getControl();

    if (tmp == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuEvent::toDuMidiChannelEvent():\n"
                << "invalid control:" << tmp;

        return DuMidiChannelEventPtr();
    }

    quint8 midiType = static_cast<quint8>(tmp) + 0x08;

    channelEvent->setRunningStatus(midiType == prevType);
    channelEvent->setType(midiType);


    int keyboard = getKeyboard();
    tmp = getNote();

    if (tmp == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuEvent::toDuMidiChannelEvent():\n"
                << "invalid note:" << tmp;

        return DuMidiChannelEventPtr();
    }

    int midiKey = 0;

    if (midiType < DuMidiChannelEvent::KeyAftertouch)
    {
        if (isPercu)
        {
            if (keyboard == -1)
            {
                qCCritical(LOG_CAT_DU_OBJECT)
                        << "DuEvent::toDuMidiChannelEvent():\n"
                        << "invalid keyboard:" << keyboard;

                return DuMidiChannelEventPtr();
            }

            //The keyboard value is not the same as the keyboard index to be used
            //when calling MidiConversionHelper::percuKey()

            //Percussion instrKeyMap starts from 1 in du-musics
            //Percussion map index starts from 0 in instr_mapping.c arrays

            quint8 keyboardIndex = (keyboard >> 7) & 0x01;
            midiKey = MidiConversionHelper::percuToMidi(static_cast<quint8>(tmp), keyboardIndex,
                                                        static_cast<quint8>(instrKeyMap) - 1);

            if (midiKey == -1 || static_cast<quint8>(midiKey) > 0x7F)
            {
                qCCritical(LOG_CAT_DU_OBJECT)
                        << "DuEvent::toDuMidiChannelEvent():\n"
                        << "invalid percussion key:" << midiKey;

                return DuMidiChannelEventPtr();
            }
        }
        else
        {
            midiKey = tmp + 12 * presetOctave + transpose - RECORD_TRANSPOSEMAX;
            if (midiKey > 0x7F)
            {
                qCCritical(LOG_CAT_DU_OBJECT)
                        << "DuEvent::toDuMidiChannelEvent():\n"
                        << "invalid harmonic key:" << midiKey << "\n"
                        << "(instrument preset octave =" << presetOctave << "\n"
                        << "transpose =" << transpose - RECORD_TRANSPOSEMAX << ")";

                return DuMidiChannelEventPtr();
            }
        }
    }
    else
    {
        midiKey = tmp;
    }

    channelEvent->setKey(static_cast<quint8>(midiKey));


    tmp = getValue();

    if (tmp == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuEvent::toDuMidiChannelEvent():\n"
                << "invalid value:" << tmp;

        return DuMidiChannelEventPtr();
    }

    channelEvent->setValue(static_cast<quint8>(tmp));


    return channelEvent;
}


int DuEvent::size() const
{
    return MUSIC_SAMPLE_SIZE;
}


DU_KEY_ACCESSORS_IMPL(DuEvent, Time,     Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuEvent, Control,  Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuEvent, Canal,    Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuEvent, Keyboard, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuEvent, Note,     Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuEvent, Value,    Numeric, int, -1)
