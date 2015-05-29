#ifndef DUMIDIFILE_H
#define DUMIDIFILE_H

#include "dumiditrack.h"
#include <QIODevice>


#define MIDI_HEADER_ID_VALUE            "MThd"
#define MIDI_HEADER_ID_SIZE             4
#define MIDI_HEADER_CHUNK_SIZE          0x00000006


class DuMidiFile
{
public:
    explicit DuMidiFile();
    explicit DuMidiFile(QIODevice *input);
    ~DuMidiFile();

    quint16 getFormat();
    quint16 getDivision();
    QList<DuMidiTrack *>& getTracks();

    void setFormat(quint16 value);
    void setDivision(quint16 value);
    void appendTrack(DuMidiTrack *track);

    QByteArray toByteArray();

    QList<DuMidiMetaEvent *> findMetaEvents(quint16 trackIndex, quint8 type);
/*
    void sortEvents(QMap<quint16, QMap<quint8, QList<DuMidiChannelEvent *>>> *channelMap,
                    QMap<quint16, QMap<quint8, QList<DuMidiMetaEvent *>>> *metaMap);
*/
private:
    void parseFile(QIODevice *input);
    DuMidiTrack* parseTrack(QDataStream &stream);
    DuMidiBasicEvent* parseEvent(QDataStream &stream, quint8 *runningStatus,
                                 bool *trackEnded);
    DuMidiChannelEvent* parseChannelEvent(QDataStream &stream, quint8 *runningStatus,
                                        quint8 *byte);
    DuMidiChannelEvent* parseChannelEvent(QDataStream &stream, quint8 *runningStatus);
    DuMidiMetaEvent* parseMetaEvent(QDataStream &stream, bool *trackEnded);
    DuMidiSysExEvent* parseSysExEvent(QDataStream &stream, quint8 status);

    quint16 format;
    quint16 count;
    quint16 division;
    QList<DuMidiTrack*> tracks;
};

#endif // DUMIDIFILE_H
