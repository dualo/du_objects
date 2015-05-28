#include "dumiditrack.h"

DuMidiTrack::DuMidiTrack() :
    events(QList<DuMidiAbstractEvent*>())
{
    events.clear();
}

DuMidiTrack::~DuMidiTrack()
{
    qDeleteAll(events);
}


void DuMidiTrack::appendEvent(DuMidiAbstractEvent *event)
{
    events.append(event);
}


QByteArray DuMidiTrack::toByteArray()
{
    QByteArray array;
    array.clear();

    bool runningStatusActive = false;
    quint8 tmpStatus = 0x00;

    for (int i = 0; i < events.count(); i++)
    {
        runningStatusActive = ((tmpStatus == events[i]->getStatus())
                               && (tmpStatus < 0xF0));
        //TODO: refactoring
        array += events[i]->toByteArray(runningStatusActive);

        tmpStatus = events[i]->getStatus();
    }

    quint32 size = array.size();

    array.prepend((char)(size & 0xFF));
    array.prepend((char)((size >> 8) & 0xFF));
    array.prepend((char)((size >> 16) & 0xFF));
    array.prepend((char)((size >> 24) & 0xFF));
    array.prepend(MIDI_TRACK_ID_VALUE);

    return array;
}


QList<DuMidiAbstractEvent *> &DuMidiTrack::getEvents()
{
    return events;
}

/*
void DuMidiTrack::sortEvents(QMap<quint8, QList<DuMidiChannelEvent *>> *channelMap,
                           QMap<quint8, QList<DuMidiMetaEvent *>> *metaMap)
{
    for(int i = 0; i < events.count(); i++)
    {
        DuMidiChannelEvent *channelEvent = dynamic_cast<DuMidiChannelEvent *>(events[i]);

        if (channelEvent != NULL)
        {
            QList<DuMidiChannelEvent *> tmpList =
                    channelMap->value(channelEvent->getChannel());
            tmpList.append(channelEvent);
        }
        else
        {
            DuMidiMetaEvent *metaEvent = dynamic_cast<DuMidiMetaEvent *>(events[i]);

            if (metaEvent != NULL)
            {
                QList<DuMidiMetaEvent *> tmpList = metaMap->value(metaEvent->getType());
                tmpList.append(metaEvent);
            }
        }
    }
}
*/
