#ifndef DUMIDITRACK_H
#define DUMIDITRACK_H

#include "dumidichannelevent.h"
#include "dumidimetaevent.h"
#include "dumidisysexevent.h"
#include <QList>
#include <QMap>


#define MIDI_TRACK_ID_VALUE             "MTrk"
#define MIDI_TRACK_ID_SIZE              4
#define MIDI_TRACK_SIZE_SIZE            4


class DuMidiTrack
{
public:
    explicit DuMidiTrack();
    ~DuMidiTrack();

    void appendEvent(DuAbstractMidiEvent* event);

    QByteArray toByteArray();

    QList<DuAbstractMidiEvent *>& getEvents();
/*
    void sortEvents(QMap<quint8, QList<DuMidiChannelEvent *>> *channelMap,
                    QMap<quint8, QList<DuMidiMetaEvent *>> *metaMap);
*/
private:
    QList<DuAbstractMidiEvent *> events;
};

#endif // DUMIDITRACK_H
