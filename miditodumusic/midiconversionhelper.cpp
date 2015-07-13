#include "midiconversionhelper.h"

#include <QFileDialog>
#include <QJsonDocument>

#include <QDebug>

//#pragma pack(push, 4)
#include "../du-touch/parameters/instr_mapping.c"
//#pragma pack(pop)


MidiConversionHelper::MidiConversionHelper() :
    valid(false),
    duration(0),
    tempo(100),
    timeSig(-1),
    scale(-1),
    tonality(-1),
    mapper(DuMidiKeyMapperPtr(new DuMidiKeyMapper))
{
    if (importMidiFile())
    {
        if (!populateMapper())
            qCritical() << "failed to import jsonMaps";

        else if (duration != 0)
            valid = true;
    }
    else
        qCritical() << "failed to import midiFile";
}

MidiConversionHelper::~MidiConversionHelper()
{
}


bool MidiConversionHelper::isValid() const
{
    return valid;
}


int MidiConversionHelper::getDuration() const
{
    return duration;
}


QList<QString> MidiConversionHelper::mapList() const
{
    return mapper->mapList();
}

void MidiConversionHelper::chooseMap()
{
    if (scale == -1 || tonality == -1)
        return;

    mapper->chooseMap(mapper->mapList()[scale], tonality);
}


int MidiConversionHelper::getTempo() const
{
    return tempo;
}

void MidiConversionHelper::setTempo(int value)
{
    tempo = value;
}

int MidiConversionHelper::getTimeSig() const
{
    return timeSig;
}

void MidiConversionHelper::setTimeSig(int value)
{
    timeSig = value;
}

int MidiConversionHelper::getScale() const
{
    return scale;
}

void MidiConversionHelper::setScale(int value)
{
    scale = value;
}

int MidiConversionHelper::getTonality() const
{
    return tonality;
}

void MidiConversionHelper::setTonality(int value)
{
    tonality = value;
}

QString MidiConversionHelper::getTitle() const
{
    return title;
}

void MidiConversionHelper::setTitle(const QString &value)
{
    title = value;
}


QString MidiConversionHelper::getTrackName(int index) const
{
    if (index >= trackNames.count())
        return QString();

    return trackNames[index];
}

const QStringList MidiConversionHelper::getTrackNames() const
{
    return trackNames;
}


QList<DuMidiTrackPtr> MidiConversionHelper::getTracks()
{
    QList<DuMidiTrackPtr> trackList;
    trackList.clear();

    const DuArrayPtr tracks = selectedFile->getTracks();
    if (tracks == NULL)
        return QList<DuMidiTrackPtr>();

    DuObjectPtr tmpObject = DuObjectPtr();
    DuMidiTrackPtr tmpTrack = DuMidiTrackPtr();

    int count = tracks->count();
    if (count == -1)
        return QList<DuMidiTrackPtr>();

    for (int i = 0; i < count; i++)
    {
        tmpObject = tracks->at(i);
        if (tmpObject == NULL)
            return QList<DuMidiTrackPtr>();

        tmpTrack = tmpObject.dynamicCast<DuMidiTrack>();
        if (tmpTrack == NULL)
            return QList<DuMidiTrackPtr>();

        trackList.append(tmpTrack);
    }

    return trackList;
}


int MidiConversionHelper::getMidiChannel(int index) const
{
    if (index >= selectedTracks.count())
        return -1;

    const DuMidiTrackPtr &midiTrack = selectedTracks[index];

    const DuArrayPtr &midiEvents = midiTrack->getEvents();
    if (midiEvents == NULL)
        return -1;

    const DuObjectPtr &tmpObject = midiEvents->at(0);
    if (tmpObject == NULL)
        return -1;

    const DuMidiBasicEventPtr &midiEvent =
            tmpObject.dynamicCast<DuMidiBasicEvent>();
    if (midiEvent == NULL)
        return -1;

    const DuMidiChannelEventPtr &channelEvent =
            midiEvent.dynamicCast<DuMidiChannelEvent>();
    if (channelEvent == NULL)
        return -1;

    return channelEvent->getChannel();
}


void MidiConversionHelper::addSelection(int trackNum, int loopNum)
{
    selectedIndexes.append(QPair<int, int>(trackNum, loopNum));
    selectedTracks.append(DuMidiTrackPtr());
    selectedInstruments.append(DuInstrumentPtr());

    percuMappings.append(QPair<bool, int>(false, -1));
}

void MidiConversionHelper::removeSelectionAt(int index)
{
    int count = selectedIndexes.count();

    if (index >= count)
        return;

    if (count != selectedTracks.count() || count != selectedInstruments.count()
            || count != percuMappings.count())
        return;

    selectedIndexes.removeAt(index);
    selectedTracks.removeAt(index);
    selectedInstruments.removeAt(index);

    percuMappings.removeAt(index);
}


QPair<int, int> MidiConversionHelper::getIndexes(int index) const
{
    return selectedIndexes[index];
}

int MidiConversionHelper::findIndexes(int trackIndex, int loopIndex) const
{
    if (loopIndex > 2 + trackIndex % 2)
        return -1;

    int count = selectedIndexes.count();

    for (int i = 0; i < count; i++)
    {
        const QPair<int, int> &indexes = selectedIndexes[i];

        if (indexes.first == trackIndex && indexes.second == loopIndex)
            return i;
    }

    return -1;
}


const DuMidiTrackPtr MidiConversionHelper::getMidiTrack(int index) const
{
    if (index >= selectedTracks.count())
        return DuMidiTrackPtr();

    return selectedTracks[index];
}

void MidiConversionHelper::setSelectedTrack(int index,
                                            const DuMidiTrackPtr &midiTrack)
{
    if (index >= selectedTracks.count())
        return;

    selectedTracks[index] = midiTrack;
}


const DuInstrumentPtr MidiConversionHelper::getInstrument(int index) const
{
    if (index >= selectedInstruments.count())
        return DuInstrumentPtr();

    return selectedInstruments[index];
}

void MidiConversionHelper::setSelectedInstr(int index,
                                            const DuInstrumentPtr &instrument)
{
    if (index >= selectedInstruments.count())
        return;

    selectedInstruments[index] = instrument;
}


QPair<bool, int> MidiConversionHelper::getPercuSetting(int index) const
{
    if (index >= percuMappings.count())
        return QPair<bool, int>(true, 0xFF);

    return percuMappings[index];
}


void MidiConversionHelper::setPercuMapping(int index, const QPair<bool, int> &mapping)
{
    if (index >= percuMappings.count())
        return;

    percuMappings[index] = mapping;
}


bool MidiConversionHelper::isPercu(int index) const
{
    if (index >= percuMappings.count())
        return false;

    return percuMappings[index].first;
}


int MidiConversionHelper::fetchKeyboard(int key, int index) const
{
    if (index >= selectedInstruments.count())
        return -1;

    const DuInstrumentPtr &instrument = selectedInstruments[index];
    if (instrument == NULL)
        return -1;

    const DuInstrumentInfoConstPtr &instrInfo =
            instrument->getInstrumentInfo();
    if (instrInfo == NULL)
        return -1;

    return mapper->fetchKeyboard(instrInfo->getOctave(), key);
}


int MidiConversionHelper::fetchPercuKey(int gmKey, int index) const
{
    if (!isPercu(index))
        return gmKey;

    int percuMap = percuMappings[index].second;

    s_note tmpNote;
    int tmpKey = 0xFF;

    for (int i = 0; i < 58; i++)
    {
        tmpNote = keyboard_note_map[percuMap][0][i];
        tmpKey = tmpNote.note_gmref;

        if (tmpKey == gmKey)
            return i;
    }

    return -1;
}


int MidiConversionHelper::percuKey(quint8 duKey, quint8 keyboard, quint8 mapIndex)
{
    if (duKey > 57 || keyboard > 1 || mapIndex > 3)
        return -1;

    return keyboard_note_map[mapIndex][keyboard][duKey].note_gmref;
}


bool MidiConversionHelper::populateMapper()
{
    QString fileName = QFileDialog::getOpenFileName(0, "Open json maps",
                                                    "", "json (*.json)");
    if(fileName.isEmpty())
        return false;

    QFile *input = new QFile(fileName);
    if (!input->open(QIODevice::ReadOnly))
        return false;

    QJsonObject &jsonMaps =
            QJsonDocument::fromJson(input->readAll()).object();

    input->close();

    mapper->importMaps(jsonMaps);

    return true;
}


bool MidiConversionHelper::importMidiFile()
{
    QString fileName = QFileDialog::getOpenFileName(0, "Open midi file",
                                                    "", "midi (*.mid *.midi)");
    if(fileName.isEmpty())
        return false;

    QFile *input = new QFile(fileName);
    if (!input->open(QIODevice::ReadOnly))
        return false;

    DuMidiFilePtr &midiFile = DuMidiFile::fromMidiBinary(input);

    input->close();

    if (midiFile->getFormat() != 1)
        return false;

    selectedFile = midiFile;
    trackNames.clear();

    return filterMetaEvents();
}


bool MidiConversionHelper::filterMetaEvents()
{
    int midiDivision = selectedFile->getDivision();
    if (midiDivision == -1)
    {
        qCritical() << "MidiConversionHelper::filterMetaEvents()\n"
                    << "failed to filter midi file\n"
                    << "invalid time division";

        return false;
    }

    bool tempoFound = false;
    bool timeSigFound = false;
    bool keySigFound = false;
    bool titleFound = false;

    const DuArrayPtr &midiTracks = selectedFile->getTracks();
    if (midiTracks == NULL)
        return false;

    if (midiTracks->count() == -1)
        return false;

    int i = 0;

    while (i < midiTracks->count())
    {
        bool instrumentFound = false;
        bool progChangeFound = false;
        QString trackName = "";

        const DuMidiTrackPtr &midiTrack =
                midiTracks->at(i).dynamicCast<DuMidiTrack>();
        if (midiTrack == NULL)
            return false;

        const DuArrayPtr &midiEvents = midiTrack->getEvents();
        if (midiEvents == NULL)
            return false;

        if (midiEvents->count() == -1)
            return false;


        //The last event in a midi track should always be an EndOfTrack meta event
        //We will need the information provided by these events later so we won't
        //remove them now.

        int j = 0;
        while (j < midiEvents->count() - 1)
        {
            const DuMidiBasicEventPtr &midiEvent =
                    midiEvents->at(j).dynamicCast<DuMidiBasicEvent>();
            if (midiEvent == NULL)
                return false;

            //Adjusting timestamp for .dumusic time division

            int time = midiEvent->getTime();
            if (time != -1)
            {
                quint64 tmp = time;
                tmp *= DUMUSIC_DIVISION;
                tmp /= midiDivision;

                midiEvent->setTime((quint32)tmp, 0);
            }

            const DuMidiMetaEventPtr &metaEvent =
                    midiEvent.dynamicCast<DuMidiMetaEvent>();

            if (metaEvent == NULL)          //Event is ChannelEvent or SysExEvent
            {
                const DuMidiChannelEventPtr &channelEvent =
                        midiEvent.dynamicCast<DuMidiChannelEvent>();

                if (channelEvent == NULL)   //Event is SysExEvent
                {
                    midiEvents->removeAt(j);
                }

                else                        //Event is ChannelEvent
                {
                    if (channelEvent->getType() == DuMidiChannelEvent::ProgramChange)
                    {
                        if (!progChangeFound)
                        {
                            progChangeFound = true;

                            int tmpNum = channelEvent->getValue();
                            if (tmpNum == -1)
                                return false;

                            trackName.append(QString(" (PC#")
                                             + QString::number(tmpNum)
                                             + QString(")"));
                        }

                        midiEvents->removeAt(j);
                    }

                    else
                    {
                        j++;
                    }
                }
            }

            else                            //Event is MetaEvent
            {
                if (metaEvent->getType() == -1)
                    return false;

                if (metaEvent->getType() == DuMidiMetaEvent::Tempo)
                {
                    if (!tempoFound)
                    {
                        tempoFound = true;
                        tempo = metaEvent->getTempo();
                        if (tempo == -1)
                            return false;
                    }
                }

                else if (metaEvent->getType() == DuMidiMetaEvent::TimeSignature)
                {
                    if (!timeSigFound)
                    {
                        timeSigFound = true;
                        QByteArray tmpData = metaEvent->getData();
                        if (tmpData.size() != 4)
                            return false;

                        quint32 tmpNum = tmpData[1];
                        quint8 tmpDecal = 8 + tmpData[1] - 1;

                        timeSig = ((tmpNum << tmpDecal) & 0xFF00) + tmpData[0];
                    }
                }

                else if (metaEvent->getType() == DuMidiMetaEvent::KeySignature)
                {
                    if (!keySigFound)
                    {
                        keySigFound = true;
                        scale = metaEvent->getScale();
                        tonality = metaEvent->getTonality();
                        if (scale == -1 || tonality == -1)
                            return false;
                    }
                }

                else if (metaEvent->getType() == DuMidiMetaEvent::Title)
                {
                    if (!titleFound)
                    {
                        titleFound = true;
                        title = QString(metaEvent->getData().data());
                        if (title.isNull())
                            return false;
                        qDebug() << title;
                    }
                }

                else if (metaEvent->getType() == DuMidiMetaEvent::Instrument)
                {
                    if (!instrumentFound)
                    {
                        instrumentFound = true;

                        QString tmpStr = QString(metaEvent->getData());
                        if (tmpStr.isNull())
                            return false;

                        trackName.prepend(tmpStr);
                    }
                }

                midiEvents->removeAt(j);
            }
        }

        if (midiEvents->count() == 1)
            midiTracks->removeAt(i);

        else
        {
            if (!instrumentFound)
                trackName.prepend(QString("Track ") + QString::number(i));
            trackNames.append(trackName);
            i++;
        }

        const DuMidiBasicEventPtr &midiEvent =
                midiEvents->at(j).dynamicCast<DuMidiBasicEvent>();
        if (midiEvent == NULL)
            return false;

        const DuMidiMetaEventPtr &metaEvent =
                midiEvent.dynamicCast<DuMidiMetaEvent>();
        if (metaEvent == NULL)
            return false;

        if (metaEvent->getType() != DuMidiMetaEvent::EndOfTrack)
            return false;
        int trackDuration = metaEvent->getTime();

        midiEvents->removeAt(j);

        if (trackDuration != -1)
        {
            quint64 tmp = trackDuration;
            tmp *= DUMUSIC_DIVISION;
            tmp /= midiDivision;

            if ((quint32)tmp > (quint32)duration)
                duration = tmp;
        }
    }

    if (!titleFound)
        title = "No Title";

    return true;
}
