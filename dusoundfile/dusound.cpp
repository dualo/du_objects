#include "dusound.h"

#include "dulayer.h"
#include "dunote.h"
#include "dusample.h"
#include "dusoundinfo.h"

#include "../instrument/duinstrumentinfo.h"

#include "../general/duarray.h"
#include "../general/dubinarydata.h"
#include "../general/dunumeric.h"

#include <cstring>
#include <QFile>
#include <QDateTime>

QAtomicInt DuSound::m_currentGlobalID = static_cast<int>(QDateTime::currentDateTimeUtc().toTime_t());

int DuSound::currentGlobalID()
{
    return m_currentGlobalID++;
}


DU_OBJECT_IMPL(DuSound)

DuSound::DuSound() :
    DuContainer(),
    m_databaseId(-1),
    m_indexInDevice(-1),
    m_hasSamplesDownloaded(false),
    m_sizeWithSamples(0)
{
    addChild(KeyInfo,       new DuSoundInfo);

    addChild(KeyLayerArray, new DuArray<DuLayer>);

    addChild(KeyMappingL,   new DuArray<DuNote>);

    addChild(KeyMetadata,   new DuArray<DuBinaryData>);
}

DuObjectPtr DuSound::clone() const
{
    return DuSoundPtr(new DuSound(*this));
}

int DuSound::size() const
{
    if (!m_hasSamplesDownloaded)
    {
        return m_sizeWithSamples;
    }

    int nbLayer = 0;
    int totalNbSamples = 0;
    int sampleSize = 0;

    const DuArrayConstPtr<DuLayer>& layers = getLayerArray();
    if (layers == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Layer array null";
        return -1;
    }

    nbLayer = getLayerArray()->count();
    for (int i = 0; i < nbLayer; ++i)
    {
        const DuLayerConstPtr& layer = layers->at(i);
        if (layer == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Layer" << i << "null";
            return -1;
        }

        const DuArrayConstPtr<DuSample>& samples = layer->getSampleArray();
        if (samples == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Sample array null";
            return -1;
        }

        int nbSamples = samples->count();
        for (int j = 0; j < nbSamples; ++j)
        {
            const DuSampleConstPtr& sample = samples->at(j);
            if (sample == NULL)
            {
                qCCritical(LOG_CAT_DU_OBJECT) << "Sample" << j << "null";
                return -1;
            }

            sampleSize += sample->getData().size();
        }

        totalNbSamples += nbSamples;
    }

    int mappingLSize = 0;
    const DuArrayConstPtr<DuNote>& mappingL = getMappingL();
    if (mappingL == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Mapping array null";
        return -1;
    }

    if (mappingL->count() != 0)
    {
        mappingLSize = MAPPING_L_SIZE;
    }

    int metadataSize = 0;
    const DuArrayConstPtr<DuBinaryData>& metadata = getMetadata();
    if (metadata == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Metadata array null";
        return -1;
    }

    if (metadata->count() != 0)
    {
        //TODO: calculate metadata size
    }

    return INSTR_NB_SAMPLES_PER_LAYER_ADDRESS
            + nbLayer * 2
            + totalNbSamples * (INSTR_DREAM_IP_SIZE + INSTR_DREAM_SP_SIZE)
            + sampleSize
            + mappingLSize
            + metadataSize;
}

DuSoundPtr DuSound::fromHeaderBinary(const QByteArray &data)
{
    if (data.size() != INSTR_HEADER_SIZE + FULL_PRESET_ALIGN)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Data size != header + instr struct size :\n"
                                      << "data.size():" << data.size()
                                      << "header + instr struct size:" << INSTR_HEADER_SIZE + FULL_PRESET_ALIGN;

        return DuSoundPtr();
    }

    s_instr_header soundHeader;
    std::memcpy(&soundHeader, data.data(), INSTR_HEADER_SIZE);

    sound_instr soundStruct;
    std::memcpy(&soundStruct, &data.data()[INSTR_HEADER_SIZE], INSTRU_STRUCT_SIZE);

    DuSoundPtr sound(new DuSound);

    if (soundStruct.s_instrument.instr_midi_pc == 0xFF)
    {
        // du-sound empty
        return sound;
    }

    sound->setSizeWithSamples(static_cast<int>(soundHeader.full_size));
    sound->setHasSamplesDownloaded(false);

    DuSoundInfoPtr info = DuSoundInfo::fromBinary(soundStruct);
    if (info != NULL)
    {
        sound->setInfo(info);
    }
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                      << "Info was not properly generated";

        return DuSoundPtr();
    }

    qCDebug(LOG_CAT_DU_OBJECT) << "du-sound" << info->getName() << "has been successfully parsed";

    return sound;
}

DuSoundPtr DuSound::fromBinary(const QByteArray &data)
{
    s_instr_header soundHeader;
    std::memcpy(&soundHeader, data.data(), INSTR_HEADER_SIZE);

    if (data.size() < static_cast<int>(soundHeader.full_size))
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Data size < size in header :\n"
                                      << "data.size():" << data.size()
                                      << "soundHeader.full_size:" << soundHeader.full_size;

        return DuSoundPtr();
    }

    sound_instr soundStruct;
    std::memcpy(&soundStruct, &data.data()[INSTR_HEADER_SIZE], INSTRU_STRUCT_SIZE);

    DuSoundPtr sound(new DuSound);

    if (soundStruct.s_instrument.instr_midi_pc == 0xFF)
    {
        // du-sound empty
        return sound;
    }

    sound->setHasSamplesDownloaded(true);

    DuSoundInfoPtr info = DuSoundInfo::fromBinary(soundStruct);
    if (info != NULL)
    {
        sound->setInfo(info);
    }
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                      << "Info was not properly generated";

        return DuSoundPtr();
    }

    DuInstrumentInfoConstPtr m3Infos = info->getInstrumentInfo();
    if (m3Infos == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                      << "Can't get InstrumentInfo from Info";

        return DuSoundPtr();
    }

    int nbLayers = soundStruct.s_instrument.nb_layer;

    QList<quint8> nbSamplesPerLayerArray;
    nbSamplesPerLayerArray.reserve(nbLayers);
    for (int i = 0; i < nbLayers; ++i)
    {
        quint8 nbSamples = static_cast<quint8>(data.data()[INSTR_NB_SAMPLES_PER_LAYER_ADDRESS + (2*i)]);
        nbSamplesPerLayerArray << nbSamples;
    }

    QList<dream_ip> dreamIPArray;
    int firstIPAddress = INSTR_NB_SAMPLES_PER_LAYER_ADDRESS + (2 * nbLayers);
    int ipSize = soundStruct.s_instrument.ip_size - (2 * nbLayers);
    dreamIPArray.reserve(ipSize / INSTR_DREAM_IP_SIZE);
    for (int i = 0; i < ipSize; i += INSTR_DREAM_IP_SIZE)
    {
        dream_ip dreamIP;
        std::memcpy(&dreamIP, &data.data()[firstIPAddress + i], INSTR_DREAM_IP_SIZE);

        dreamIPArray << dreamIP;
    }

    QVector<dream_sp> dreamSPArray;
    int firstSPAddress = firstIPAddress + ipSize;
    int spSize = soundStruct.s_instrument.sp_size;
    dreamSPArray.reserve(spSize / INSTR_DREAM_SP_SIZE);
    for (int i = 0; i < spSize; i += INSTR_DREAM_SP_SIZE)
    {
        dream_sp dreamSP;
        std::memcpy(&dreamSP, &data.data()[firstSPAddress + i], INSTR_DREAM_SP_SIZE);

        if (m3Infos->getDreamFormatId() == DuInstrumentInfo::SDK_3000)
        {
            if (dreamSP.loopType != DuSample::SND3000_Forward)
            {
                qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                              << "dreamSP" << i / INSTR_DREAM_SP_SIZE << "was not properly generated\n"
                                              << "dream format id is 3000 but loop type is" << dreamSP.loopType;

                return DuSoundPtr();
            }
        }
        else if (m3Infos->getDreamFormatId() == DuInstrumentInfo::SDK_5000)
        {
            if (dreamSP.loopType != DuSample::SND5000_Forward
                    && dreamSP.loopType != DuSample::SND5000_OneShot)
            {
                qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                              << "dreamSP" << i / INSTR_DREAM_SP_SIZE << "was not properly generated\n"
                                              << "dream format id is 5000 but loop type is" << dreamSP.loopType;

                return DuSoundPtr();
            }
        }

        dreamSPArray << dreamSP;
    }

    QByteArrayList sampleArray;
    int sampleAddress = firstSPAddress + spSize;
    sampleArray.reserve(dreamSPArray.count());
    for (int i = 0; i < dreamSPArray.count(); ++i)
    {
        const dream_sp& currentSampleParam = dreamSPArray[i];

        int currentWavAddress = static_cast<int>(DuSample::wavAddressDreamToReadable(currentSampleParam.wav_address));

        int sampleSize = DuSample::sizeWavDreamToReadable(currentSampleParam.size_wav);
        if (sampleSize <= 0)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                          << "sample" << i << "size is not positive:" << sampleSize << "\n"
                                          << "size_wav:" << currentSampleParam.size_wav;

            return DuSoundPtr();
        }

        QByteArray sample(data.mid(sampleAddress + currentWavAddress, sampleSize));

        sampleArray << sample;
    }


    int totalNbSamples = 0;
    for (int i = 0; i < nbLayers; ++i)
    {
        totalNbSamples += nbSamplesPerLayerArray[i];
    }

    Q_ASSERT(dreamIPArray.size() == totalNbSamples);
    Q_ASSERT(dreamSPArray.size() == totalNbSamples);
    Q_ASSERT(sampleArray.size()  == totalNbSamples);


    DuArrayPtr<DuLayer> layerArray(new DuArray<DuLayer>);
    int sampleCpt = 0;
    for (int i = 0; i < nbLayers; ++i)
    {
        int nbSamples = nbSamplesPerLayerArray[i];

        // If there is no samples in this layer, we just add an empty layer
        if (nbSamples == 0)
        {
            layerArray->append(new DuLayer);
            continue;
        }

        // If some samples in this layer have different bounds, we separate them in different layers.
        // We start by grouping the indexes of the samples that have the same bounds.
        QMap<QPair<int, int>, QList<int> > effectiveLayers;
        for (int sampleIndex = sampleCpt; sampleIndex < sampleCpt + nbSamples; ++sampleIndex)
        {
            const dream_ip& ip = dreamIPArray.at(sampleIndex);
            int minVel = ip.min_vel;
            int maxVel = ip.max_vel;

            effectiveLayers[qMakePair<int, int>(minVel, maxVel)].append(sampleIndex);
        }

        // Then, for each group, we create a new layer.
        // The resulting number of layers can be greater than the one given in the binary file.
        foreach (const QList<int>& indexList, effectiveLayers)
        {
            QList<dream_ip> effectiveDreamIPArray;
            QVector<dream_sp> effectiveDreamSPArray;
            QByteArrayList effectiveSampleArray;

            foreach (int index, indexList)
            {
                effectiveDreamIPArray << dreamIPArray.at(index);
                effectiveDreamSPArray << dreamSPArray.at(index);
                effectiveSampleArray << sampleArray.at(index);
            }

            DuLayerPtr layer = DuLayer::fromBinary(effectiveDreamIPArray,
                                                   effectiveDreamSPArray,
                                                   effectiveSampleArray);

            if (layer != NULL)
            {
                layerArray->append(layer);
            }
            else
            {
                qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                              << "Layer" << i << "was not properly generated";

                return DuSoundPtr();
            }
        }

        sampleCpt += nbSamples;
    }
    sound->setLayerArray(layerArray);


    if (soundHeader.mapping_addr != 0)
    {
        DuArrayPtr<DuNote> mappingL(new DuArray<DuNote>);
        for (uint i = 0; i < MAPPING_L_SIZE; i += S_NOTE_SIZE)
        {
            // --- WE HAVE TO DO A CORRESPONDANCE ---
            const uint halfMappingSize = MAPPING_L_SIZE / 2;
            uint corI = i < halfMappingSize ? i + halfMappingSize : i - halfMappingSize;

            s_note note;
            std::memcpy(&note, &data.data()[soundHeader.mapping_addr + corI], S_NOTE_SIZE);

            DuNotePtr noteObject = DuNote::fromBinary(note);
            if (noteObject != NULL)
            {
                mappingL->append(noteObject);
            }
            else
            {
                qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                              << "Note" << i / S_NOTE_SIZE << "was not properly generated";

                return DuSoundPtr();
            }
        }
        sound->setMappingL(mappingL);
    }


    if (soundHeader.meta_addr != 0)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "There are metadata in this du-sound, but we don't handle them yet.\n"
                                     << "They will be lost.";
    }


    qCDebug(LOG_CAT_DU_OBJECT) << "du-sound" << info->getName() << "has been successfully parsed";

    return sound;
}

DuSoundPtr DuSound::fromBinary(QIODevice *input)
{
    QByteArray array = input->readAll();

    if (array.isEmpty())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "failed to generate DuSound\n"
                                      << "selected file could not be read";

        return DuSoundPtr();
    }

    return DuSound::fromBinary(array);
}

QByteArray DuSound::toBinary() const
{
    QByteArray data;

    data += headerIpSpSamplesBinary();
    data += mappingLBinary();
    data += metadataBinary();

    return data;
}

QByteArray DuSound::headerIpSpSamplesBinary(bool forDuTouchSOrL) const
{
    QByteArray data;

    const DuArrayConstPtr<DuLayer>& layerArray = getLayerArray();
    if (layerArray == NULL)
        return QByteArray();

    int nbLayer = layerArray->count();

    int ipHeaderSize = 2 * nbLayer + (forDuTouchSOrL ? 2 : 0);
    QByteArray ipHeader(ipHeaderSize, 0);
    QByteArray dreamIPData;
    QByteArray dreamSPData;
    QByteArray dreamSamplesData;
    int sampleAddress = 0;
    int totalSampleSize = 0;
    int totalNbSamples = 0;

    if (forDuTouchSOrL)
    {
        ipHeader[0] = 0x40;
        ipHeader[1] = static_cast<char>(nbLayer);
    }

    for (int i = 0; i < nbLayer; ++i)
    {
        DuLayerConstPtr layer = layerArray->at(i);
        if (layer == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Layer" << i << "null";
            return QByteArray();
        }

        DuArrayConstPtr<DuSample> samples = layer->getSampleArray();
        if (samples == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Sample array null";
            return QByteArray();
        }

        int nbSamples = samples->count();

        if (forDuTouchSOrL)
        {
            ipHeader[(i + 1) * 2] = static_cast<char>(nbSamples);
            ipHeader[((i + 1) * 2) + 1] = 0;
        }
        else
        {
            ipHeader[i * 2] = static_cast<char>(nbSamples);
            if (i == 0)
                ipHeader[(i * 2) + 1] = static_cast<char>(nbLayer);
            else
                ipHeader[(i * 2) + 1] = 0;
        }

        // Sort samples by start note
        QMultiMap<int, DuSampleConstPtr> sortedSamples;
        for (int j = 0; j < nbSamples; ++j)
        {
            DuSampleConstPtr sample = samples->at(j);
            if (sample == NULL)
                return QByteArray();

            sortedSamples.insert(sample->getStartNote(), sample);
        }

        foreach (const DuSampleConstPtr& sample, sortedSamples)
        {
            if (sample == NULL)
                return QByteArray();

            dreamIPData += sample->ipBinary(static_cast<quint8>(layer->getMinVelocity()) - 1, static_cast<quint8>(layer->getMaxVelocity()));
            dreamSPData += sample->spBinary(static_cast<quint32>(sampleAddress));
            dreamSamplesData += sample->getData();

            sampleAddress += sample->getData().size();
            totalSampleSize += sample->getData().size();
        }

        totalNbSamples += nbSamples;
    }

    // HEADER
    s_instr_header soundHeader;
    std::memset(&soundHeader, 0, INSTR_HEADER_SIZE);

    soundHeader.KW_INST = 0x54534E49;
    soundHeader.KW_META = 0x4154454D;

    soundHeader.full_size = static_cast<quint32>(size() + (forDuTouchSOrL ? 2 : 0));

    soundHeader.HW_version = static_cast<quint16>(getHardInstrVersion());
    soundHeader.SW_version = static_cast<quint16>(getSoftInstrVersion());

    int mappingAddr = 0;
    const DuArrayConstPtr<DuNote>& mappingL = getMappingL();
    if (mappingL == NULL)
        return QByteArray();

    if (mappingL->count() != 0)
    {
        mappingAddr = INSTR_NB_SAMPLES_PER_LAYER_ADDRESS + ipHeaderSize + (INSTR_DREAM_IP_SIZE + INSTR_DREAM_SP_SIZE) * totalNbSamples + totalSampleSize;
    }

    soundHeader.mapping_addr = static_cast<quint32>(mappingAddr);

    int metadataAddr = 0;
    const DuArrayConstPtr<DuBinaryData>& metadata = getMetadata();
    if (metadata == NULL)
        return QByteArray();

    if (metadata->count() != 0)
    {
        if (mappingAddr != 0)
        {
            metadataAddr = mappingAddr + MAPPING_L_SIZE;
        }
        else
        {
            metadataAddr = INSTR_NB_SAMPLES_PER_LAYER_ADDRESS + ipHeaderSize + (INSTR_DREAM_IP_SIZE + INSTR_DREAM_SP_SIZE) * totalNbSamples + totalSampleSize;
        }
    }

    soundHeader.meta_addr = static_cast<quint32>(metadataAddr);

    data.append(reinterpret_cast<char*>(&soundHeader), INSTR_HEADER_SIZE);

    // SOUND STRUCT
    const DuSoundInfoConstPtr &info = getInfo();
    if (info == NULL)
        return QByteArray();

    data += info->toBinary(static_cast<quint8>(nbLayer), totalNbSamples, static_cast<quint32>(totalSampleSize));
    data += QByteArray(INTR_STRUCT_ALIGN, 0);

    data += ipHeader;
    data += dreamIPData;
    data += dreamSPData;
    data += dreamSamplesData;

    return data;
}

QByteArray DuSound::mappingLBinary() const
{
    const QByteArray& data = getMappingL()->toDuMusicBinary();
    if (data.isEmpty())
    {
        return QByteArray();
    }
    else if (data.size() != MAPPING_L_SIZE)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Mapping size is incorrect:" << data.size() << "!=" << MAPPING_L_SIZE;
        return QByteArray();
    }
    else
    {
        // --- WE HAVE TO DO A CORRESPONDANCE ---
        const int halfMappingLSize = MAPPING_L_SIZE / 2;
        return data.mid(halfMappingLSize, halfMappingLSize) + data.mid(0, halfMappingLSize);
    }
}

QByteArray DuSound::metadataBinary() const
{
    return getMetadata()->toDuMusicBinary();
}

DuObjectPtr DuSound::getChild(const QString &key)
{
    if (    key == KeyNameForDevice      ||
            key == KeyKeyMapping         ||
            key == KeyOctave             ||
            key == KeyUserID             ||
            key == KeyID                 ||
            key == KeyActiveNoteOff      ||
            key == KeyCategory           ||
            key == KeyRelativeVolume     ||
            key == KeyInstrType          ||
            key == KeyInstrVersion       ||
            key == KeyHardInstrVersion   ||
            key == KeySoftInstrVersion   ||
            key == KeyName)
    {
        DuSoundInfoPtr info = getInfo();
        if (info == NULL)
        {
            return DuObjectPtr();
        }

        return info->getChild(key);
    }
    else
    {
        return DuContainer::getChild(key);
    }
}

DuObjectConstPtr DuSound::getChild(const QString &key) const
{
    if (    key == KeyNameForDevice      ||
            key == KeyKeyMapping         ||
            key == KeyOctave             ||
            key == KeyUserID             ||
            key == KeyID                 ||
            key == KeyActiveNoteOff      ||
            key == KeyCategory           ||
            key == KeyRelativeVolume     ||
            key == KeyInstrType          ||
            key == KeyInstrVersion       ||
            key == KeyHardInstrVersion   ||
            key == KeySoftInstrVersion   ||
            key == KeyName)
    {
        DuSoundInfoConstPtr info = getInfo();
        if (info == NULL)
        {
            return DuObjectPtr();
        }

        return info->getChild(key);
    }
    else
    {
        return DuContainer::getChild(key);
    }
}

int DuSound::databaseId() const
{
    return m_databaseId;
}

void DuSound::setDatabaseId(int databaseId)
{
    m_databaseId = databaseId;
}

QStringList DuSound::lists() const
{
    return m_lists;
}

void DuSound::setLists(const QStringList &lists)
{
    m_lists = lists;
}

int DuSound::indexInDevice() const
{
    return m_indexInDevice;
}

void DuSound::setIndexInDevice(int indexInDevice)
{
    m_indexInDevice = indexInDevice;
}

QString DuSound::deviceSerialNumber() const
{
    return m_deviceSerialNumber;
}

void DuSound::setDeviceSerialNumber(const QString &deviceSerialNumber)
{
    m_deviceSerialNumber = deviceSerialNumber;
}

bool DuSound::getHasSamplesDownloaded() const
{
    return m_hasSamplesDownloaded;
}

void DuSound::setHasSamplesDownloaded(bool hasSamplesDownloaded)
{
    m_hasSamplesDownloaded = hasSamplesDownloaded;
}

int DuSound::getSizeWithSamples() const
{
    return m_sizeWithSamples;
}

void DuSound::setSizeWithSamples(int sizeWithSamples)
{
    m_sizeWithSamples = sizeWithSamples;
}

DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, NameForDevice,      DuSoundInfo, Info, QString, QString())
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, KeyMapping,         DuSoundInfo, Info, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, Octave,             DuSoundInfo, Info, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, UserID,             DuSoundInfo, Info, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, ID,                 DuSoundInfo, Info, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, ActiveNoteOff,      DuSoundInfo, Info, bool, false)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, Category,           DuSoundInfo, Info, QString, QString())
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, RelativeVolume,     DuSoundInfo, Info, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, InstrType,          DuSoundInfo, Info, INSTRUMENT_TYPE, NUM_INSTR_TYPE)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, InstrVersion,       DuSoundInfo, Info, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, HardInstrVersion,   DuSoundInfo, Info, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, SoftInstrVersion,   DuSoundInfo, Info, int, -1)

DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, Name, DuSoundInfo, Info, QString, QString())

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, Info,         DuSoundInfo)

DU_KEY_ACCESSORS_OBJECT_TEMPLATE_IMPL(DuSound, LayerArray, DuArray, DuLayer)

DU_KEY_ACCESSORS_OBJECT_TEMPLATE_IMPL(DuSound, MappingL,   DuArray, DuNote)

DU_KEY_ACCESSORS_OBJECT_TEMPLATE_IMPL(DuSound, Metadata,   DuArray, DuBinaryData)
