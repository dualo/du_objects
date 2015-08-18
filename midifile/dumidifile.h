#ifndef DUMIDIFILE_H
#define DUMIDIFILE_H

#include "dumiditrack.h"
#include <QIODevice>


#define MIDI_HEADER_ID_VALUE            "MThd"
#define MIDI_HEADER_ID_SIZE             4
#define MIDI_HEADER_CHUNK_SIZE          0x06

#define DUMUSIC_DIVISION                0x0040

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
    static DuMidiFilePtr fromMidiBinary(QDataStream &stream);

    QByteArray toDuMusicBinary() const;
    QByteArray toMidiBinary() const;
    QJsonValue toJson() const;

    int size() const;

    int getFormat() const;
    bool setFormat(quint16 value);

    int getDivision() const;
    bool setDivision(quint16 value);

    DuArrayPtr getTracks();
    DuArrayConstPtr getTracks() const;
    DuMidiTrackPtr getTrackAt(int index);

    void setTracks(const DuArrayPtr &array);
    bool appendTrack(const DuMidiTrackPtr &track);
    bool appendTracks(const QList<DuMidiTrackPtr> &tracks);
};

#endif // DUMIDIFILE_H
