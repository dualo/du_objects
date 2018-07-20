#include "dumidimetaevent.h"

#include "dumidistatus.h"
#include "dumidivariablelength.h"

#include "../general/dubinarydata.h"
#include "../general/dunumeric.h"



DU_OBJECT_IMPL(DuMidiMetaEvent)

DuMidiMetaEvent::DuMidiMetaEvent(quint32 time) :
    DuMidiBasicEvent(time, 0xFF)
{
    addChild(KEY_MIDIMETAEVENT_TYPE,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0x7F, 0x00));

    addChild(KEY_MIDIMETAEVENT_LENGTH,  new DuMidiVariableLength(0));

    addChild(KEY_MIDIMETAEVENT_DATA,    new DuBinaryData());
}


DuObjectPtr DuMidiMetaEvent::clone() const
{
    return DuMidiMetaEventPtr(new DuMidiMetaEvent(*this));
}

DuMidiMetaEventPtr DuMidiMetaEvent::fromMidiBinary(QDataStream &stream,
                                                   bool *trackEnded)
{
    DuMidiMetaEventPtr metaEvent(new DuMidiMetaEvent);

    quint8 type;

    stream >> type;

    metaEvent->setType(type);
    metaEvent->setLength(stream);
    metaEvent->setData(stream);

    *trackEnded = (type == EndOfTrack);

    return metaEvent;
}


QByteArray DuMidiMetaEvent::toMidiBinary() const
{
    QByteArray retArray;
    retArray.clear();

    const DuMidiVariableLengthConstPtr &time =
            getChildAs<DuMidiVariableLength>(KEY_MIDIEVENT_TIME);

    if (time == Q_NULLPTR)
        return QByteArray();

    retArray += time->toMidiBinary();


    const DuMidiStatusConstPtr &status =
            getChildAs<DuMidiStatus>(KEY_MIDIEVENT_STATUS);

    if (status == Q_NULLPTR)
        return QByteArray();

    retArray += status->toMidiBinary();


    const DuNumericConstPtr &type =
            getChildAs<DuNumeric>(KEY_MIDIMETAEVENT_TYPE);

    if (type == Q_NULLPTR)
        return QByteArray();

    retArray += type->toMidiBinary();


    const DuMidiVariableLengthConstPtr &length =
            getChildAs<DuMidiVariableLength>(KEY_MIDIMETAEVENT_LENGTH);

    if (length == Q_NULLPTR)
        return QByteArray();

    retArray += length->toMidiBinary();


    const DuBinaryDataConstPtr &data =
            getChildAs<DuBinaryData>(KEY_MIDIMETAEVENT_DATA);

    if (data == Q_NULLPTR)
        return QByteArray();

    retArray += data->toMidiBinary();


    return retArray;
}


int DuMidiMetaEvent::getType() const
{
    const DuNumericConstPtr &tmp =
            getChildAs<DuNumeric>(KEY_MIDIMETAEVENT_TYPE);

    if (tmp == Q_NULLPTR)
        return -1;

    return tmp->getNumeric();
}

void DuMidiMetaEvent::setType(quint8 value)
{
    const DuNumericPtr &tmp =
            getChildAs<DuNumeric>(KEY_MIDIMETAEVENT_TYPE);

     if (tmp == Q_NULLPTR)
         return;

     tmp->setNumeric(value);
}


int DuMidiMetaEvent::getLength() const
{
    const DuMidiVariableLengthConstPtr &tmp =
            getChildAs<DuMidiVariableLength>(KEY_MIDIMETAEVENT_LENGTH);

    if (tmp == Q_NULLPTR)
        return -1;

    return static_cast<int>(tmp->getAbsolute());
}

void DuMidiMetaEvent::setLength(quint32 value)
{
    const DuMidiVariableLengthPtr &length =
            getChildAs<DuMidiVariableLength>(KEY_MIDIMETAEVENT_LENGTH);
    if (length == Q_NULLPTR)
        return;

    const DuBinaryDataPtr &data =
            getChildAs<DuBinaryData>(KEY_MIDIMETAEVENT_DATA);
    if (data == Q_NULLPTR)
        return;

    length->setAbsolute(value);
    data->resize(static_cast<int>(value));
}

void DuMidiMetaEvent::setLength(QDataStream &stream)
{
    const DuMidiVariableLengthPtr &length =
            getChildAs<DuMidiVariableLength>(KEY_MIDIMETAEVENT_LENGTH);
    if (length == Q_NULLPTR)
        return;

    const DuBinaryDataPtr &data =
            getChildAs<DuBinaryData>(KEY_MIDIMETAEVENT_DATA);
    if (data == Q_NULLPTR)
        return;

    length->setAbsolute(stream);
    data->resize(static_cast<int>(length->getAbsolute()));
}


const QByteArray DuMidiMetaEvent::getData() const
{
    const DuBinaryDataConstPtr &tmp =
            getChildAs<DuBinaryData>(KEY_MIDIMETAEVENT_DATA);

    if (tmp == Q_NULLPTR)
        return QByteArray();

    return tmp->getBinaryData();
}

void DuMidiMetaEvent::setData(const QByteArray &value)
{
    const DuMidiVariableLengthPtr &length =
            getChildAs<DuMidiVariableLength>(KEY_MIDIMETAEVENT_LENGTH);
    if (length == Q_NULLPTR)
        return;

    const DuBinaryDataPtr &data =
            getChildAs<DuBinaryData>(KEY_MIDIMETAEVENT_DATA);
    if (data == Q_NULLPTR)
        return;

    length->setAbsolute(static_cast<quint32>(value.size()));
    data->setBinaryData(value);
}

void DuMidiMetaEvent::setData(QDataStream &stream)
{
    const DuBinaryDataPtr &tmp =
            getChildAs<DuBinaryData>(KEY_MIDIMETAEVENT_DATA);

    if (tmp == Q_NULLPTR)
        return;

    tmp->setData(stream);
}


void DuMidiMetaEvent::setTitle(const QString &title)
{
    if (title.isEmpty())
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuMidiMetaEvent::setTitle():\n"
                   << "the title given is empty";
    }

    setType(DuMidiMetaEvent::Title);

    setData(title.toLatin1());
}

void DuMidiMetaEvent::setInstrumentName(const QString &instrument)
{
    if (instrument.isEmpty())
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuMidiMetaEvent::setInstrumentName():\n"
                   << "the instrument name given is empty";
    }

    setType(DuMidiMetaEvent::Instrument);

    setData(instrument.toLatin1());
}


int DuMidiMetaEvent::getTempo() const
{
    if (getType() != Tempo)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMidiMetaEvent::getTempo():\n"
                    << "this event does not have tempo meta type";

        return -1;
    }

    const QByteArray &tempoArray = getData();
    if (tempoArray.size() != 3)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMidiMetaEvent::getTempo():\n"
                    << "incorrect data";

        return -1;
    }

    quint32 tmpByte = 0;
    quint32 tmp = 0;

    tmpByte = static_cast<quint32>(tempoArray[0]);
    tmp += (tmpByte << 16) & 0xFF0000;

    tmpByte = static_cast<quint32>(tempoArray[1]);
    tmp += (tmpByte << 8) & 0xFF00;

    tmpByte = static_cast<quint32>(tempoArray[2]);
    tmp += tmpByte & 0xFF;

    return MICROSECS_PER_MIN / tmp;
}

void DuMidiMetaEvent::setTempo(quint8 bpm)
{
    if (bpm < 4)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMidiMetaEvent::setTempo():\n"
                    << "bpm value is too small and will result"
                    << "in an incorrect midi tempo value";
    }

    setType(DuMidiMetaEvent::Tempo);

    QByteArray tempoArray;
    tempoArray.clear();

    quint32 tempoValue = MICROSECS_PER_MIN / bpm;
    tempoArray.append(static_cast<char>((tempoValue >> 16) & 0xFF));
    tempoArray.append(static_cast<char>((tempoValue >> 8) & 0xFF));
    tempoArray.append(static_cast<char>(tempoValue & 0xFF));

    setData(tempoArray);
}


void DuMidiMetaEvent::setTimeSignature(quint8 nn, quint8 dd, quint8 cc, quint8 bb)
{
    if (nn == 0 ||  dd == 0 || cc == 0  ||bb == 0)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMidiMetaEvent::setTimeSignature():\n"
                    << "a parameter is 0, this will result in an"
                    << "incorrect midi time signature value";
    }

    setType(DuMidiMetaEvent::TimeSignature);

    QByteArray timeSigArray;
    timeSigArray.clear();

    timeSigArray.append(static_cast<char>(nn));
    timeSigArray.append(static_cast<char>(dd));
    timeSigArray.append(static_cast<char>(cc));
    timeSigArray.append(static_cast<char>(bb));

    setData(timeSigArray);
}


int DuMidiMetaEvent::getTonality() const
{
    if (getType() != KeySignature)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMidiMetaEvent::getTonality():\n"
                    << "this event does not have key signature meta type";

        return -1;
    }

    const QByteArray &keySigArray = getData();
    if (keySigArray.size() != 2)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMidiMetaEvent::getTonality():\n"
                    << "incorrect data";

        return -1;
    }

    qint8 sf = keySigArray[0];
    quint8 mi = static_cast<quint8>(keySigArray[1]);

    return ((sf + 6 * (2 + sf % 2) - 3 * mi + 1) % 12);
}

int DuMidiMetaEvent::getScale() const
{
    if (getType() != KeySignature)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMidiMetaEvent::getTonality():\n"
                    << "this event does not have key signature meta type";

        return -1;
    }

    const QByteArray &keySigArray = getData();
    if (keySigArray.size() != 2)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMidiMetaEvent::getTempo():\n"
                    << "incorrect data";

        return -1;
    }

    return keySigArray[1];
}

void DuMidiMetaEvent::setKeySignature(quint8 key, bool isMinor)
{
    quint8 scale = 0;
    if (isMinor) scale++;

    qint8 tmpTonal = (static_cast<quint8>(key) + 3 * scale - 1) % 12;

    qint8 signature = (tmpTonal - 6 * (tmpTonal % 2)) % 12;
    if (signature > 7)
        signature -= 12;
    else if (signature < -7)
        signature += 12;

    setType(DuMidiMetaEvent::KeySignature);

    QByteArray keySigArray;
    keySigArray.clear();

    keySigArray.append(static_cast<char>(signature));
    keySigArray.append(static_cast<char>(scale));

    setData(keySigArray);
}


void DuMidiMetaEvent::setEndOfTrack()
{
    setType(DuMidiMetaEvent::EndOfTrack);
}
