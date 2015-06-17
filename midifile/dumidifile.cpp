#include "dumidifile.h"



DU_OBJECT_IMPL(DuMidiFile)

DuMidiFile::DuMidiFile() :
    DuContainer()
{
    addChild(KEY_MIDIFILE_FORMAT,   new DuNumeric(1, 2, 2, 0));

    addChild(KEY_MIDIFILE_DIVISION, new DuNumeric(DUMUSIC_DIVISION, 2, 0xFFFF, 0x0000));

    addChild(KEY_MIDIFILE_TRACKS,   new DuArray());
}

DuMidiFile::~DuMidiFile()
{
}


DuObjectPtr DuMidiFile::clone() const
{
    return DuMidiFilePtr(new DuMidiFile(*this));
}


DuMidiFilePtr DuMidiFile::fromMidiBinary(QIODevice *input)
{
    QDataStream stream(input);

    return fromMidiBinary(stream);
}

DuMidiFilePtr DuMidiFile::fromMidiBinary(QDataStream &stream)
{
    QByteArray headerId;
    quint32 headerSize;

    headerId.resize(MIDI_HEADER_ID_SIZE);
    stream.readRawData(headerId.data(), headerId.size());
    stream >> headerSize;

    if (headerId != MIDI_HEADER_ID_VALUE || headerSize != MIDI_HEADER_CHUNK_SIZE)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "The selected file is not a Standard MIDI File";

        return DuMidiFilePtr();
    }

    quint16 format;
    stream >> format;

    if (format > 0x01)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "The software cannot read MIDI format" << format;

        return DuMidiFilePtr();
    }

    DuMidiFilePtr midiFile(new DuMidiFile);

    quint16 count;
    quint16 division;

    stream >> count;
    stream >> division;

    bool verif = true;

    verif = verif && midiFile->setFormat(format);
    verif = verif && midiFile->setDivision(division);

    if (!verif)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Invalid MIDI header parameters";

        return DuMidiFilePtr();
    }

    for (int i = 0; i < count; i++)
    {
        const DuMidiTrackPtr &track = DuMidiTrack::fromMidiBinary(stream);
        midiFile->appendTrack(track);
    }

    return midiFile;
}


QByteArray DuMidiFile::toDuMusicBinary() const
{
    Q_UNIMPLEMENTED();
    return QByteArray();
}

QByteArray DuMidiFile::toMidiBinary() const
{
    QByteArray array;
    array.clear();

    int format = getFormat();
    int count = getTracks()->count();
    int division = getDivision();

    array.append(MIDI_HEADER_ID_VALUE);
    array.append((char)((MIDI_HEADER_CHUNK_SIZE >> 24) & 0xFF));
    array.append((char)((MIDI_HEADER_CHUNK_SIZE >> 16) & 0xFF));
    array.append((char)((MIDI_HEADER_CHUNK_SIZE >> 8) & 0xFF));
    array.append((char)(MIDI_HEADER_CHUNK_SIZE & 0xFF));

    array.append((char)((format >> 8) & 0xFF));
    array.append((char)(format & 0xFF));
    array.append((char)((count >> 8) & 0xFF));
    array.append((char)(count & 0xFF));
    array.append((char)((division >> 8) & 0xFF));
    array.append((char)(division & 0xFF));

    array += getTracks()->toMidiBinary();

    return array;
}


QJsonValue DuMidiFile::toJson() const
{
    Q_UNIMPLEMENTED();
    return QJsonValue();
}


int DuMidiFile::size() const
{
    return MIDI_HEADER_ID_SIZE + MIDI_HEADER_CHUNK_SIZE + getTracks()->size();
}


int DuMidiFile::getFormat() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_MIDIFILE_FORMAT);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuMidiFile::setFormat(quint16 value)
{
    const DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_MIDIFILE_FORMAT);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuMidiFile::getDivision() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_MIDIFILE_DIVISION);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuMidiFile::setDivision(quint16 value)
{
    const DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_MIDIFILE_DIVISION);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


DuArrayConstPtr DuMidiFile::getTracks() const
{
    return getChildAs<DuArray>(KEY_MIDIFILE_TRACKS);
}

void DuMidiFile::setTracks(const DuArrayPtr &array)
{
    addChild(KEY_MIDIFILE_TRACKS, array);
}

bool DuMidiFile::appendTrack(const DuMidiTrackPtr &track)
{
    DuArrayPtr tmp = getChildAs<DuArray>(KEY_MIDIFILE_TRACKS);

    if (tmp == NULL)
        return false;

    return tmp->append(track);
}

bool DuMidiFile::appendTracks(const QList<DuMidiTrackPtr> &tracks)
{
    DuArrayPtr tmp = getChildAs<DuArray>(KEY_MIDIFILE_TRACKS);

    if (tmp == NULL)
        return false;

    bool ret = true;

    //Qt doesn't convert DuMidiTrackPtr to DuObjectPtr in a QList
    foreach (const DuMidiTrackPtr &track, tracks)
    {
        ret = ret && tmp->append(track);
    }

    return ret;
}
