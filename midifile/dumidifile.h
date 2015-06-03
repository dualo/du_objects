#ifndef DUMIDIFILE_H
#define DUMIDIFILE_H

#include "dumiditrack.h"
#include <QIODevice>


#define MIDI_HEADER_ID_VALUE            "MThd"
#define MIDI_HEADER_ID_SIZE             4
#define MIDI_HEADER_CHUNK_SIZE          0x00000006

#define DUMUSIC_DIVISION                0x0040
#define MICROSECS_PER_MIN               60000000

#define KEY_MIDIFILE_FORMAT             "Format"
#define KEY_MIDIFILE_DIVISION           "Division"
#define KEY_MIDIFILE_TRACKS             "Tracks"


DU_OBJECT(DuMidiFile)

class DuMidiFile : public DuContainer
{
public:
    explicit DuMidiFile();
    ~DuMidiFile();

    DuObjectPtr clone() const;

    static DuMidiFilePtr fromMidiBinary(QIODevice *input);

    QByteArray toDuMusicBinary() const;
    QByteArray toMidiBinary() const;
    QJsonValue toJson() const;

    int size() const;

    int getFormat() const;
    bool setFormat(quint16 value);

    int getDivision() const;
    bool setDivision(quint16 value);

    DuArrayConstPtr getTracks() const;
    void setTracks(const DuArrayPtr &array);
    bool appendTrack(const DuMidiTrackPtr &track);

private:
    DuMidiTrack* parseTrack(QDataStream &stream);
    DuMidiBasicEvent* parseEvent(QDataStream &stream, quint8 *runningStatus,
                                 bool *trackEnded);
    DuMidiChannelEvent* parseChannelEvent(QDataStream &stream, quint8 *runningStatus,
                                        quint8 *byte);
    DuMidiChannelEvent* parseChannelEvent(QDataStream &stream, quint8 *runningStatus);
    DuMidiMetaEvent* parseMetaEvent(QDataStream &stream, bool *trackEnded);
    DuMidiSysExEvent* parseSysExEvent(QDataStream &stream, quint8 status);
};

#endif // DUMIDIFILE_H
