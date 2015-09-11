#include "duevent.h"

#include <QJsonObject>


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

DuEvent::~DuEvent()
{
}

DuObjectPtr DuEvent::clone() const
{
    return DuEventPtr(new DuEvent(*this));
}


DuEventPtr DuEvent::fromDuMusicBinary(const music_sample &du_sample)
{
    DuEventPtr event(new DuEvent);
    bool verif = true;

    verif = event->setTime(du_sample.time) ? verif : false;
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


DuEventPtr DuEvent::fromJson(const QJsonObject &jsonEvent)
{
    QJsonValue jsonTime     = jsonEvent[KeyTime];
    QJsonValue jsonCtrl     = jsonEvent[KeyControl];
    QJsonValue jsonCanal    = jsonEvent[KeyCanal];
    QJsonValue jsonKbrd     = jsonEvent[KeyKeyboard];
    QJsonValue jsonNote     = jsonEvent[KeyNote];
    QJsonValue jsonVal      = jsonEvent[KeyValue];

    if (        !jsonTime.isDouble()    ||  !jsonCtrl.isDouble()
            ||  !jsonCanal.isDouble()   ||  !jsonKbrd.isDouble()
            ||  !jsonNote.isDouble()    ||  !jsonVal.isDouble())
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuEvent::fromJson():\n"
                << "failed to generate DuEvent\n"
                << "a json key did not contain the proper type";

        return DuEventPtr();
    }


    DuEventPtr event(new DuEvent);
    bool verif = true;

    verif = event->setTime(jsonTime.toInt()) ? verif : false;
    verif = event->setControl(jsonCtrl.toInt()) ? verif : false;
    verif = event->setCanal(jsonCanal.toInt()) ? verif : false;
    verif = event->setKeyboard(jsonKbrd.toInt()) ? verif : false;
    verif = event->setNote(jsonNote.toInt()) ? verif : false;
    verif = event->setValue(jsonVal.toInt()) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                << "DuEvent::fromJson():\n"
                << "an attribute was not properly set";
    }

    return event;
}

DuEventPtr DuEvent::fromMidi(const DuMidiChannelEventPtr &channelEvent,
                             int presetOctave, int instrKeyMap, bool isPercu,
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

    int tmpKey = channelEvent->getKey();
    if (tmpKey == -1)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                << "DuEvent::fromMidi():\n"
                << "invalid midi key:" << tmpKey;

        return DuEventPtr();
    }

    //Converting event in case of note off written as note on with velocity 0
    if (midiType == DuMidiChannelEvent::NoteOn
            && channelEvent->getValue() == 0)
    {
        channelEvent->setType(DuMidiChannelEvent::NoteOff);
        channelEvent->setValue(0x40);
    }

    int keyboard = 0;
    int key = tmpKey;

    bool verif = true;

    if (isPercu)
    {
        //Percussion map index starts from 0 in instr_mapping.c arrays
        //In du-musics, it starts from 1 (0 being for harmonic instruments)
        quint8 percuIndex = instrKeyMap - 1;

        if (midiType <= DuMidiChannelEvent::KeyAftertouch)
        {
            key = MidiConversionHelper::percuFromMidi(tmpKey, percuIndex);
            if (key == -1)
            {
                qCWarning(LOG_CAT_DU_OBJECT)
                        << "DuEvent::fromMidi():\n"
                        << "key not found in this mapping";

                return DuEventPtr();
            }
        }

        verif = event->setCanal((percuIndex << 4) & 0xF0) ? verif : false;
    }
    else if (midiType <= DuMidiChannelEvent::KeyAftertouch)
    {
        //KEY_MUSIC_TRANSPOSE set to default when importing from Midi
        key = tmpKey - 12 * presetOctave;
        if (key < 0)
        {
            qCWarning(LOG_CAT_DU_OBJECT)
                    << "DuEvent::fromMidi():\n"
                    << "invalid midi key:" << tmpKey;

            return DuEventPtr();
        }

        keyboard = helper.getKeyboardFromMidi(key);
    }

    verif = event->setKeyboard(keyboard) ? verif : false;
    verif = event->setNote(key) ? verif : false;

    verif = event->setTime(channelEvent->getTime()) ? verif : false;
    verif = event->setControl(midiType - 0x08) ? verif : false;
    verif = event->setValue(channelEvent->getValue()) ? verif : false;

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

    int tmpNum = 0;
    int tmpKbrd = 0;


    tmpNum = getTime();
    if (tmpNum == -1)
        return QByteArray();
    du_sample.time = (quint32)tmpNum;

    tmpNum = getControl();
    if (tmpNum == -1)
        return QByteArray();
    du_sample.control = (quint8)tmpNum;

    tmpNum = getCanal();
    if (tmpNum == -1)
        return QByteArray();
    du_sample.canal = (quint8)tmpNum;

    tmpKbrd = getKeyboard();
    if (tmpKbrd == -1)
        return QByteArray();
    tmpNum = getNote();
    if (tmpNum == -1)
        return QByteArray();
    du_sample.note = (quint8)tmpKbrd + (quint8)tmpNum;

    tmpNum = getValue();
    if (tmpNum == -1)
        return QByteArray();
    du_sample.value = (quint8)tmpNum;


    return QByteArray((char *)&(du_sample), MUSIC_SAMPLE_SIZE);
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

    channelEvent->setTime((quint32)tmp - prevTime, prevTime);


    tmp = getControl();

    if (tmp == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuEvent::toDuMidiChannelEvent():\n"
                << "invalid control:" << tmp;

        return DuMidiChannelEventPtr();
    }

    quint8 midiType = (quint8)tmp + 0x08;

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
            midiKey = MidiConversionHelper::percuToMidi((quint8)tmp,keyboardIndex,
                                                        (quint8)instrKeyMap - 1);

            if (midiKey == -1 || (quint8)midiKey > 0x7F)
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

    channelEvent->setKey((quint8)midiKey);


    tmp = getValue();

    if (tmp == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuEvent::toDuMidiChannelEvent():\n"
                << "invalid value:" << tmp;

        return DuMidiChannelEventPtr();
    }

    channelEvent->setValue((quint8)tmp);


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
