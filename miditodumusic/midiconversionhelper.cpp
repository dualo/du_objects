#include "midiconversionhelper.h"

#include "../general/duarray.h"

#include "../midifile/dumidibasicevent.h"
#include "../midifile/dumidichannelevent.h"
#include "../midifile/dumidifile.h"
#include "../midifile/dumidimetaevent.h"
#include "../midifile/dumiditrack.h"

#include "../dusoundfile/dusound.h"

#include <QDataStream>
#include <QFile>
#include <QJsonDocument>


MidiConversionHelper::MidiConversionHelper() :
    midiValid(false),
    mapsValid(false),
    duration(0),
    tempo(100),
    timeSig(TIME_4_4),
    scale(QStringLiteral(SCALE_MAJOR)),
    tonality(0),
    midiTempo(100),
    midiTimeSig(0x0404),
    midiScale(0),
    midiTonality(0)
{
    midiScaleBoxModel.append(tr("Major"));
    midiScaleBoxModel.append(tr("Minor"));

    timeSigBoxModel.addTimeSignature(DuTimeSignature(TIME_OFF, tr("Off")));
    timeSigBoxModel.addTimeSignature(DuTimeSignature(TIME_2_4, tr("2 : 4")));
    timeSigBoxModel.addTimeSignature(DuTimeSignature(TIME_3_4, tr("3 : 4")));
    timeSigBoxModel.addTimeSignature(DuTimeSignature(TIME_4_4, tr("4 : 4")));
    timeSigBoxModel.addTimeSignature(DuTimeSignature(TIME_5_4, tr("5 : 4")));

    tonalityBoxModel.addTonality(DuTonality(0,      12,     tr("Cb")));
    tonalityBoxModel.addTonality(DuTonality(1,      1,      tr("C")));
    tonalityBoxModel.addTonality(DuTonality(2,      2,      tr("C#")));
    tonalityBoxModel.addTonality(DuTonality(3,      15,     tr("Ebb")));
    tonalityBoxModel.addTonality(DuTonality(4,      16,     tr("Eb")));
    tonalityBoxModel.addTonality(DuTonality(5,      5,      tr("E")));
    tonalityBoxModel.addTonality(DuTonality(6,      18,     tr("E#")));
    tonalityBoxModel.addTonality(DuTonality(7,      19,     tr("Gb")));
    tonalityBoxModel.addTonality(DuTonality(8,      8,      tr("G")));
    tonalityBoxModel.addTonality(DuTonality(9,      9,      tr("G#")));
    tonalityBoxModel.addTonality(DuTonality(10,     22,     tr("Bbb")));
    tonalityBoxModel.addTonality(DuTonality(11,     23,     tr("Bb")));
    tonalityBoxModel.addTonality(DuTonality(12,     0,      tr("B")));
    tonalityBoxModel.addTonality(DuTonality(13,     13,     tr("B#")));
    tonalityBoxModel.addTonality(DuTonality(14,     14,     tr("Db")));
    tonalityBoxModel.addTonality(DuTonality(15,     3,      tr("D")));
    tonalityBoxModel.addTonality(DuTonality(16,     4,      tr("D#")));
    tonalityBoxModel.addTonality(DuTonality(17,     17,     tr("Fb")));
    tonalityBoxModel.addTonality(DuTonality(18,     6,      tr("F")));
    tonalityBoxModel.addTonality(DuTonality(19,     7,      tr("F#")));
    tonalityBoxModel.addTonality(DuTonality(20,     20,     tr("F##")));
    tonalityBoxModel.addTonality(DuTonality(21,     21,     tr("Ab")));
    tonalityBoxModel.addTonality(DuTonality(22,     10,     tr("A")));
    tonalityBoxModel.addTonality(DuTonality(23,     11,     tr("A#")));
}

MidiConversionHelper::~MidiConversionHelper()
{
}


bool MidiConversionHelper::isValid() const
{
    return midiValid && mapsValid;
}


int MidiConversionHelper::getDuration() const
{
    return duration;
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
    return timeSigBoxModel.indexFromValue(timeSig);
}

void MidiConversionHelper::setTimeSig(int value)
{
    timeSig = value;
}

int MidiConversionHelper::getScale() const
{
    return scaleBoxModel.indexFromId(scale);
}

void MidiConversionHelper::setScale(const QString &value)
{
    scale = value;
}

int MidiConversionHelper::getTonality() const
{
    return tonalityBoxModel.indexFromValue(tonality);
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


int MidiConversionHelper::getMidiTempo() const
{
    return midiTempo;
}

QString MidiConversionHelper::getMidiTimeSigStr() const
{
    quint8 num = midiTimeSig & 0xFF;
    quint8 denom = (midiTimeSig >> 8) & 0xFF;

    return QString::number(num) + QStringLiteral(" : ") + QString::number(denom);
}

QString MidiConversionHelper::getMidiScale() const
{
    return midiScaleBoxModel[midiScale];
}

QString MidiConversionHelper::getMidiTonality() const
{
    return tonalityBoxModel.findName(midiTonality);
}

QString MidiConversionHelper::getMidiTitle() const
{
    return midiTitle;
}


QVector<DuMidiTrackPtr> MidiConversionHelper::getTracks()
{
    QVector<DuMidiTrackPtr> trackList;
    trackList.clear();

    const DuArrayPtr<DuMidiTrack> tracks = selectedFile->getTracks();
    if (tracks == NULL)
        return QVector<DuMidiTrackPtr>();

    DuObjectPtr tmpObject = DuObjectPtr();
    DuMidiTrackPtr tmpTrack = DuMidiTrackPtr();

    int count = tracks->count();
    if (count == -1)
        return QVector<DuMidiTrackPtr>();

    for (int i = 0; i < count; i++)
    {
        tmpObject = tracks->at(i);
        if (tmpObject == NULL)
            return QVector<DuMidiTrackPtr>();

        tmpTrack = tmpObject.dynamicCast<DuMidiTrack>();
        if (tmpTrack == NULL)
            return QVector<DuMidiTrackPtr>();

        trackList.append(tmpTrack);
    }

    return trackList;
}


int MidiConversionHelper::getMidiChannel(int index) const
{
    if (index >= selectedTracks.count())
        return -1;

    const DuMidiTrackPtr &midiTrack = selectedTracks[index];

    const DuArrayPtr<DuMidiBasicEvent> &midiEvents = midiTrack->getEvents();
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

QList<MidiTrackData> MidiConversionHelper::midiTracks() const
{
    return m_midiTracks;
}


QStringList MidiConversionHelper::mapList() const
{
    return mapper.mapList();
}

void MidiConversionHelper::chooseMap()
{
    if (scale.isEmpty() || tonality == -1)
        return;

    mapper.chooseMap(scale, static_cast<quint8>(tonality));
}


void MidiConversionHelper::addSelection(int trackNum, int loopNum, int midiTrackIndex, const DuSoundConstPtr& sound)
{
    // Midi track
    const DuMidiTrackPtr& midiTrack = selectedFile->getTrackAt(midiTrackIndex);
    if (midiTrack == NULL)
    {
        qCCritical(LOG_CAT_MIDI) << "Midi track null";
        return;
    }

    // Sound
    if (sound == NULL)
    {
        qCCritical(LOG_CAT_MIDI) << "Sound null";
        return;
    }

    selectedIndexes.append(QPair<int, int>(trackNum, loopNum));
    selectedTracks.append(midiTrack);
    selectedSounds.append(sound->cloneAs<DuSound>());
}

void MidiConversionHelper::removeSelectionAt(int index)
{
    int count = selectedIndexes.count();

    if (index >= count)
        return;

    if (count != selectedTracks.count() || count != selectedSounds.count())
        return;

    selectedIndexes.removeAt(index);
    selectedTracks.removeAt(index);
    selectedSounds.removeAt(index);
}


QPair<int, int> MidiConversionHelper::getIndexes(int index) const
{
    if (index < 0 || index >= selectedIndexes.size())
        return qMakePair<int, int>(-1, -1);

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


DuMidiTrackPtr MidiConversionHelper::getMidiTrack(int index) const
{
    if (index >= selectedTracks.count())
        return DuMidiTrackPtr();

    return selectedTracks[index];
}


DuSoundPtr MidiConversionHelper::getSound(int index) const
{
    if (index >= selectedSounds.count())
        return DuSoundPtr();

    return selectedSounds[index];
}


int MidiConversionHelper::getKeyboardFromMidi(quint8 key) const
{
    return mapper.keyboardFromMidi(key);
}

int MidiConversionHelper::percuToMidi(quint8 duKey, quint8 keyboardIndex,
                                      quint8 mapIndex)
{
    quint32 mapIndexRef = (NUM_KEY_MAP - 1) / 2 - 1;
    quint32 keyIndexRef = NUM_BUTTON_KEYBOARD_L + 35 - 1;

    if (duKey > keyIndexRef || keyboardIndex > 1 || mapIndex > mapIndexRef)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                << "MidiConversionHelper::percuKey():\n"
                << "invalid values:\n"
                << "key =" << duKey << ";"
                << "keyboardIndex =" << keyboardIndex << ";"
                << "mapIndex =" << mapIndex;

        return -1;
    }

    //TODO: get mapping directly from du-sound
//    return keyboard_note_map[mapIndex][keyboardIndex][duKey - 35].note_gmref;
    return -1;
}


int MidiConversionHelper::findTimeSig(const QString &key)
{
    return timeSigBoxModel.findValue(key);
}

DuTimeSignatureModel *MidiConversionHelper::getTimeSigBoxModel()
{
    return &timeSigBoxModel;
}

QString MidiConversionHelper::findScale(const QString &key) const
{
    return scaleBoxModel.findId(key);
}

DuScaleModel *MidiConversionHelper::getScaleBoxModel()
{
    return &scaleBoxModel;
}

int MidiConversionHelper::findTonality(const QString &key)
{
    return tonalityBoxModel.findValue(key);
}

DuTonalityModel *MidiConversionHelper::getTonalityBoxModel()
{
    return &tonalityBoxModel;
}


int MidiConversionHelper::getDutouchScale() const
{
    return mapper.dutouchScale(scale);
}


QStringList MidiConversionHelper::midiScales() const
{
    return midiScaleBoxModel;
}


bool MidiConversionHelper::importMidiFile(const DuMidiFilePtr &midiFile)
{
    if (midiFile->getFormat() != 1)
    {
        setMidiValid(false);
        return false;
    }

    selectedFile = midiFile;
    m_midiTracks.clear();

    setMidiValid(filterMetaEvents() ? (duration != 0) : false);
    return midiValid;
}


bool MidiConversionHelper::populateMapper(const QJsonObject &jsonMaps)
{
    mapper.importMaps(jsonMaps);

    QStringList mapList = mapper.mapList();
    scaleBoxModel.addScale(DuScale(QStringLiteral(SCALE_NONE), tr(SCALE_NONE)));

    int count = mapList.count();

    for (int i = 0; i < count; i++)
    {
        const QString &map = mapList[i];
        scaleBoxModel.addScale(getScaleIds(map));
    }

    setMapsValid(true);
    return mapsValid;
}

bool MidiConversionHelper::init(const QByteArray &midiRawData)
{
    selectedIndexes.clear();
    selectedTracks.clear();
    selectedSounds.clear();

    QDataStream stream(midiRawData);
    const DuMidiFilePtr &midiFile = DuMidiFile::fromMidiBinary(stream);
    if (midiFile == NULL)
    {
        qCCritical(LOG_CAT_MIDI) << "Midi file parsing failed";
        return false;
    }

    if (!importMidiFile(midiFile))
    {
        qCCritical(LOG_CAT_MIDI) << "Midi file invalid";
        return false;
    }

    QFile mapsInput(":/defaultmaps.json");
    if (!mapsInput.open(QIODevice::ReadOnly))
    {
        qCCritical(LOG_CAT_MIDI) << "Can't open default maps file";
        return false;
    }

    const QJsonObject &jsonMaps = QJsonDocument::fromJson(mapsInput.readAll()).object();

    if (!populateMapper(jsonMaps))
    {
        qCCritical(LOG_CAT_MIDI) << "Can't populate mapper";
        return false;
    }

    return true;
}


bool MidiConversionHelper::filterMetaEvents()
{
    int midiDivision = selectedFile->getDivision();
    if (midiDivision == -1)
    {
        qCCritical(LOG_CAT_MIDI) << "MidiConversionHelper::filterMetaEvents()\n"
                    << "failed to filter midi file\n"
                    << "invalid time division";

        return false;
    }

    bool tempoFound = false;
    bool timeSigFound = false;
    bool keySigFound = false;
    bool titleFound = false;

    const DuArrayPtr<DuMidiTrack> &midiTracks = selectedFile->getTracks();
    if (midiTracks == NULL)
        return false;

    if (midiTracks->count() == -1)
        return false;

    int i = 0;

    while (i < midiTracks->count())
    {
        bool instrumentFound = false;
        bool progChangeFound = false;
        QString trackName;
        int progChange = -1;

        const DuMidiTrackPtr &midiTrack =
                midiTracks->at(i).dynamicCast<DuMidiTrack>();
        if (midiTrack == NULL)
            return false;

        const DuArrayPtr<DuMidiBasicEvent> &midiEvents = midiTrack->getEvents();
        if (midiEvents == NULL)
            return false;

        if (midiEvents->count() == -1)
            return false;


        //The last event in a midi track should always be an EndOfTrack meta event

        const DuMidiBasicEventPtr &midiEvent =
                midiEvents->at(midiEvents->count() - 1).dynamicCast<DuMidiBasicEvent>();
        if (midiEvent == NULL)
            return false;

        const DuMidiMetaEventPtr &metaEvent =
                midiEvent.dynamicCast<DuMidiMetaEvent>();
        if (metaEvent == NULL)
            return false;

        if (metaEvent->getType() != DuMidiMetaEvent::EndOfTrack)
            return false;

        int trackDuration = metaEvent->getTime();

        if (trackDuration != -1)
        {
            quint64 tmp = static_cast<quint64>(trackDuration);
            tmp *= DUMUSIC_DIVISION;
            tmp /= static_cast<quint64>(midiDivision);

            //This prevents having events with a timestamp equal to the loop reference duration
            tmp += 1;

            if (static_cast<quint32>(tmp) > static_cast<quint32>(duration))
                setDuration(static_cast<int>(tmp));
        }

        //Removing EndOfTrack event
        midiEvents->removeAt(midiEvents->count() - 1);


        int j = 0;
        while (j < midiEvents->count())
        {
            const DuMidiBasicEventPtr &midiEvent =
                    midiEvents->at(j).dynamicCast<DuMidiBasicEvent>();
            if (midiEvent == NULL)
                return false;

            //Adjusting timestamp for .dumusic time division

            int time = midiEvent->getTime();

            if (time != -1)
            {
                quint64 tmp = static_cast<quint64>(time);
                tmp *= DUMUSIC_DIVISION;
                tmp /= static_cast<quint64>(midiDivision);

                midiEvent->setTime(static_cast<quint32>(tmp), 0);
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

                            trackName.append(QStringLiteral(" (PC#")
                                             + QString::number(tmpNum)
                                             + QStringLiteral(")"));
                            progChange = tmpNum;
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
                        setMidiTempo(metaEvent->getTempo());
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

                        quint32 tmpNum = static_cast<quint32>(tmpData[1]);
                        quint8 tmpDecal = 8 + static_cast<quint8>(tmpData[1]) - 1;

                        setMidiTimeSig(static_cast<int>(((tmpNum << tmpDecal) & 0xFF00) + static_cast<uint>(tmpData[0])));
                    }
                }

                else if (metaEvent->getType() == DuMidiMetaEvent::KeySignature)
                {
                    if (!keySigFound)
                    {
                        keySigFound = true;

                        int tmpScale = metaEvent->getScale();
                        int tmpTonality = metaEvent->getTonality();
                        if (tmpScale == -1 || tmpTonality == -1)
                            return false;

                        setMidiScale(tmpScale);
                        setMidiTonality(tmpTonality);
                    }
                }

                else if (metaEvent->getType() == DuMidiMetaEvent::Title)
                {
                    if (!titleFound)
                    {
                        titleFound = true;
                        setMidiTitle(QString::fromLatin1(metaEvent->getData()));
                        if (title.isNull())
                            return false;
                    }
                }

                else if (metaEvent->getType() == DuMidiMetaEvent::Instrument)
                {
                    if (!instrumentFound)
                    {
                        instrumentFound = true;

                        QString tmpStr = QString::fromLatin1(metaEvent->getData());
                        if (tmpStr.isNull())
                            return false;

                        trackName.prepend(tmpStr);
                    }
                }

                midiEvents->removeAt(j);
            }
        }

        //When all events are removed, j is still 0
        if (j == 0)
            midiTracks->removeAt(i);

        else
        {
            if (!instrumentFound)
                trackName.prepend(QStringLiteral("Track ") + QString::number(i));
            m_midiTracks << MidiTrackData({trackName, progChange});
            i++;
        }
    }

    if (!titleFound)
        setMidiTitle(tr("No Title"));

    return true;
}


DuScale MidiConversionHelper::getScaleIds(const QString &scale) const
{
    const QPair<QString, QString> &scaleIds = mapper.scaleIds(scale);

    return DuScale(scaleIds.first, scaleIds.second);
}


void MidiConversionHelper::setMidiValid(bool value)
{
    midiValid = value;
}

void MidiConversionHelper::setMapsValid(bool value)
{
    mapsValid = value;
}


void MidiConversionHelper::setDuration(int value)
{
    duration = value;
}


// The following methods are used only in filterMetaEvents() to initialize
// the midi meta data.
// The corresponding parameters to be used in export to du-music format are
// also initialized.

void MidiConversionHelper::setMidiTempo(int value)
{
    midiTempo = value;
    setTempo(value);
}

void MidiConversionHelper::setMidiTimeSig(int value)
{
    midiTimeSig = value;

    quint8 num = midiTimeSig & 0xFF;
    quint8 denom = (midiTimeSig >> 8) & 0xFF;

    if (denom == 4 && num < 6 && num != 1)
        setTimeSig(num - 1);
    else
        setTimeSig(TIME_OFF);
}

void MidiConversionHelper::setMidiScale(int value)
{
    midiScale = value;

    if (midiScale == 0)
        setScale(QStringLiteral(SCALE_MAJOR));
    else if (midiScale == 1)
        setScale(QStringLiteral(SCALE_MINOR));
    else
        setScale(QString());
}

void MidiConversionHelper::setMidiTonality(int value)
{
    midiTonality = value;
    setTonality(tonalityBoxModel.findValue(value));
}

void MidiConversionHelper::setMidiTitle(const QString &value)
{
    midiTitle = value;
    setTitle(value);
}
