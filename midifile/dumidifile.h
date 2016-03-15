#ifndef DUMIDIFILE_H
#define DUMIDIFILE_H

#include <QIODevice>

#include "../general/ducontainer.h"


#define MIDI_HEADER_ID_VALUE    "MThd"
#define MIDI_HEADER_ID_SIZE     4
#define MIDI_HEADER_CHUNK_SIZE  0x06

#define DUMUSIC_DIVISION        0x0040

#define KEY_MIDIFILE_FORMAT     QStringLiteral("Format")
#define KEY_MIDIFILE_DIVISION   QStringLiteral("Division")
#define KEY_MIDIFILE_TRACKS     QStringLiteral("Tracks")


#ifdef Q_OS_WIN
#include "../general/duarray.h"
#else
DU_OBJECT_TEMPLATE(DuArray)
#endif
DU_OBJECT(DuMidiTrack)

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

    DuArrayPtr<DuMidiTrack> getTracks();
    DuArrayConstPtr<DuMidiTrack> getTracks() const;
    DuMidiTrackPtr getTrackAt(int index);

    void setTracks(const DuArrayPtr<DuMidiTrack> &array);
    bool appendTrack(const DuMidiTrackPtr &track);
    bool appendTracks(const QVector<DuMidiTrackPtr> &tracks);
};

#endif // DUMIDIFILE_H
