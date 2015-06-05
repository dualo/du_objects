#ifndef DUMIDIMETAEVENT_H
#define DUMIDIMETAEVENT_H

#include "dumidibasicevent.h"
#include "../general/dubinarydata.h"


#define KEY_MIDIMETAEVENT_TYPE     "Type"
#define KEY_MIDIMETAEVENT_LENGTH   "Length"
#define KEY_MIDIMETAEVENT_DATA     "Data"


DU_OBJECT(DuMidiMetaEvent)

class DuMidiMetaEvent : public DuMidiBasicEvent
{
public:
    explicit DuMidiMetaEvent(quint32 time = 0);
    virtual ~DuMidiMetaEvent();

    enum MetaTypes
    {
        // A meta type is always followed by the number of bytes left to read
        // before the end of the event.

        // The meta types 0x01 to 0x0F represent text events with different purposes.
        // Not all of them are used.

        Sequence = 0x00,            // FF 00 02 seq num
        Text = 0x01,                // FF 01 length text
        Copyright = 0x02,           // FF 02 length text
        Title = 0x03,               // FF 03 length text
        Instrument = 0x04,          // FF 04 length text
        Lyric = 0x05,               // FF 05 length text
        Marker = 0x06,              // FF 06 length text
        CuePoint = 0x07,            // FF 07 length text
        ProgramName = 0x08,         // FF 08 length text
        DeviceName = 0x09,          // FF 09 length text
        MidiChannel = 0x20,         // FF 20 01 Channel
        MidiPort = 0x21,            // FF 21 01 Port
        EndOfTrack = 0x2F,          // FF 2F 00
        Tempo = 0x51,               // FF 51 03 tt tt tt
        SmpteOffset = 0x54,         // FF 54 05 hr mn se fr ff
        TimeSignature = 0x58,       // FF 58 04 num denom clocks 32nds
        KeySignature = 0x59,        // FF 59 02 sf mi
        SequencerSpecific = 0x7F    // FF 7F length data
    };

    virtual DuObjectPtr clone() const;

    static DuMidiMetaEventPtr fromMidiBinary(QDataStream &stream, bool *trackEnded);

    virtual QByteArray toMidiBinary() const;

    quint8 getType() const;
    void setType(quint8 value);

    quint32 getLength() const;
    void setLength(quint32 value);
    void setLength(QDataStream &stream);

    const QByteArray getData() const;
    void setData(const QByteArray &value);
    void setData(QDataStream &stream);
};

#endif // DUMIDIMETAEVENT_H
