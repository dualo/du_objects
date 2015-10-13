#include "dusound.h"

#include "dulayer.h"
#include "dunote.h"
#include "dusample.h"
#include "dusoundinfo.h"

#include "../dumusicfile/instrument/duinstrumentinfo.h"

#include "../general/duarray.h"
#include "../general/dubinarydata.h"
#include "../general/dunumeric.h"

#include <cstring>
#include <QFile>


DU_OBJECT_IMPL(DuSound)

DuSound::DuSound() :
    DuContainer(),
    m_databaseId(-1)
{
    addChild(KeyInfo,       new DuSoundInfo);

    addChild(KeyLayerArray, new DuArray);

    addChild(KeyMapping,    new DuArray);

    addChild(KeyMetadata,   new DuArray);
}

DuObjectPtr DuSound::clone() const
{
    return DuSoundPtr(new DuSound(*this));
}

int DuSound::size() const
{
    int nbLayer = 0;
    int totalNbSamples = 0;
    int sampleSize = 0;

    const DuArrayConstPtr& layers = getLayerArray();
    if (layers == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Layer array null";
        return -1;
    }

    nbLayer = getLayerArray()->count();
    for (int i = 0; i < nbLayer; ++i)
    {
        const DuLayerConstPtr& layer = layers->atAs<DuLayer>(i);
        if (layer == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Layer" << i << "null";
            return -1;
        }

        const DuArrayConstPtr& samples = layer->getSampleArray();
        if (samples == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Sample array null";
            return -1;
        }

        int nbSamples = samples->count();
        for (int j = 0; j < nbSamples; ++j)
        {
            const DuSampleConstPtr& sample = samples->atAs<DuSample>(j);
            if (sample == NULL)
            {
                qCCritical(LOG_CAT_DU_OBJECT) << "Sample" << j << "null";
                return -1;
            }

            sampleSize += sample->getData().size();
        }

        totalNbSamples += nbSamples;
    }

    int mappingSize = 0;
    const DuArrayConstPtr& mapping = getMapping();
    if (mapping == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Mapping array null";
        return -1;
    }

    if (mapping->count() != 0)
    {
        mappingSize = MAPPING_SIZE;
    }

    int metadataSize = 0;
    const DuArrayConstPtr& metadata = getMetadata();
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
            + mappingSize
            + metadataSize;
}

DuSoundPtr DuSound::fromBinary(const QByteArray &data)
{
    s_instr_header soundHeader;
    std::memcpy((char*)&soundHeader, data.data(), INSTR_HEADER_SIZE);

    if (data.size() < (int)soundHeader.full_size)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Data size < size in header :\n"
                                      << "data.size():" << data.size()
                                      << "soundHeader.full_size:" << soundHeader.full_size;

        return DuSoundPtr();
    }

    struct_instr soundStruct;
    std::memcpy((char*)&soundStruct, &data.data()[INSTR_HEADER_SIZE], INSTRU_STRUCT_SIZE);

    DuSoundPtr sound(new DuSound);

    if (soundStruct.s_instrument.instr_midi_pc == 0xFF)
    {
        // du-sound empty
        return sound;
    }

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

    uint32_t sampleOffset = DuInstrumentInfo::sampleAddressDreamToReadable(soundStruct.s_instrument.sample_address);

    int nbLayers = soundStruct.s_instrument.nb_layer;

    QList<uint8_t> nbSamplesPerLayerArray;
    nbSamplesPerLayerArray.reserve(nbLayers);
    for (int i = 0; i < nbLayers; ++i)
    {
        uint8_t nbSamples = (uint8_t)data.data()[INSTR_NB_SAMPLES_PER_LAYER_ADDRESS + (2*i)];
        nbSamplesPerLayerArray << nbSamples;
    }

    QList<dream_ip> dreamIPArray;
    int firstIPAddress = INSTR_NB_SAMPLES_PER_LAYER_ADDRESS + (2 * nbLayers);
    int ipSize = soundStruct.s_instrument.ip_size - (2 * nbLayers);
    dreamIPArray.reserve(ipSize / INSTR_DREAM_IP_SIZE);
    for (int i = 0; i < ipSize; i += INSTR_DREAM_IP_SIZE)
    {
        dream_ip dreamIP;
        std::memcpy((char*)&dreamIP, &data.data()[firstIPAddress + i], INSTR_DREAM_IP_SIZE);

        dreamIPArray << dreamIP;
    }

    QVector<dream_sp> dreamSPArray;
    int firstSPAddress = firstIPAddress + ipSize;
    int spSize = soundStruct.s_instrument.sp_size;
    dreamSPArray.reserve(spSize / INSTR_DREAM_SP_SIZE);
    for (int i = 0; i < spSize; i += INSTR_DREAM_SP_SIZE)
    {
        dream_sp dreamSP;
        std::memcpy((char*)&dreamSP, &data.data()[firstSPAddress + i], INSTR_DREAM_SP_SIZE);

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
    for (int i = 0; i < dreamSPArray.count() - 1; ++i)
    {
        const dream_sp& currentSampleParam  = dreamSPArray[i];
        const dream_sp& nextSampleParam     = dreamSPArray[i+1];

        uint32_t currentWavAddress  = DuSample::wavAddressDreamToReadable(currentSampleParam.wav_address,   sampleOffset);
        uint32_t nextWavAddress     = DuSample::wavAddressDreamToReadable(nextSampleParam.wav_address,      sampleOffset);

        int sampleSize = nextWavAddress - currentWavAddress;
        if (sampleSize <= 0)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                          << "sample" << i << "size is not positive:" << sampleSize << "\n"
                                          << "nextWavAddress:" << nextWavAddress << "\n"
                                          << "currentWavAddress:" << currentWavAddress;

            return DuSoundPtr();
        }

        QByteArray sample(data.mid(sampleAddress, sampleSize));

        sampleArray << sample;

        sampleAddress += sampleSize;
    }

    if (dreamSPArray.count() != 0)
    {
        const dream_sp& currentSampleParam  = dreamSPArray[dreamSPArray.count() - 1];

        uint32_t currentWavAddress = DuSample::wavAddressDreamToReadable(currentSampleParam.wav_address, sampleOffset);

        int sampleSize = soundStruct.s_instrument.sample_size - currentWavAddress;
        if (sampleSize <= 0)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                          << "last sample (" << dreamSPArray.count() - 1 << ") size is not positive:" << sampleSize << "\n"
                                          << "m3Infos->getSampleSize():" << soundStruct.s_instrument.sample_size << "\n"
                                          << "currentWavAddress:" << currentWavAddress;

            return DuSoundPtr();
        }

        QByteArray sample(data.mid(sampleAddress, sampleSize));

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


    DuArrayPtr layerArray(new DuArray);
    int sampleCpt = 0;
    for (int i = 0; i < nbLayers; ++i)
    {
        int nbSamples = nbSamplesPerLayerArray[i];

        DuLayerPtr layer = DuLayer::fromBinary(dreamIPArray.mid(sampleCpt, nbSamples),
                                               dreamSPArray.mid(sampleCpt, nbSamples),
                                               sampleArray.mid(sampleCpt, nbSamples));
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

        sampleCpt += nbSamples;
    }
    sound->setLayerArray(layerArray);


    if (soundHeader.mapping_addr != 0)
    {
        DuArrayPtr mapping(new DuArray);
        for (int i = 0; i < MAPPING_SIZE; i += S_NOTE_SIZE)
        {
            s_note note;
            std::memcpy((char*)&note, &data.data()[soundHeader.mapping_addr + i], S_NOTE_SIZE);

            DuNotePtr noteObject = DuNote::fromBinary(note);
            if (noteObject != NULL)
            {
                mapping->append(noteObject);
            }
            else
            {
                qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                              << "Note" << i / S_NOTE_SIZE << "was not properly generated";

                return DuSoundPtr();
            }
        }
        sound->setMapping(mapping);
    }


    if (soundHeader.meta_addr != 0)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "There are metadata in this du-sound, but we don't handle them yet.\n"
                                     << "They will be lost.";
    }


    qCDebug(LOG_CAT_DU_OBJECT) << "du-sound" << m3Infos->getName() << "has been successfully parsed";

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

    uint32_t sampleOffset = 0;

    const DuArrayConstPtr& layerArray = getLayerArray();
    if (layerArray == NULL)
        return QByteArray();

    int nbLayer = layerArray->count();
    int totalNbSamples = 0;
    for (int i = 0; i < nbLayer; ++i)
    {
        const DuLayerConstPtr& layer = layerArray->atAs<DuLayer>(i);
        if (layer == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Layer" << i << "null";
            return QByteArray();
        }

        const DuArrayConstPtr& samples = layer->getSampleArray();
        if (samples == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Sample array null";
            return QByteArray();
        }

        totalNbSamples += samples->count();
    }

    QByteArray ipHeader(2 * nbLayer, 0);
    QByteArray dreamIPData;
    QByteArray dreamSPData;
    QByteArray dreamSamplesData;
    int sampleAddress = 0;
    int totalSampleSize = 0;
    for (int i = 0; i < nbLayer; ++i)
    {
        DuLayerConstPtr layer = layerArray->atAs<DuLayer>(i);
        if (layer == NULL)
            return QByteArray();

        DuArrayConstPtr samples = layer->getSampleArray();
        if (samples == NULL)
            return QByteArray();

        int nbSamples = samples->count();

        ipHeader[i * 2] = nbSamples;
        if (i == 0)
            ipHeader[(i * 2) + 1] = nbLayer;
        else
            ipHeader[(i * 2) + 1] = 0;

        for (int j = 0; j < nbSamples; ++j)
        {
            DuSampleConstPtr sample = samples->atAs<DuSample>(j);
            if (sample == NULL)
                return QByteArray();

            dreamIPData += sample->ipBinary(layer->getMinVelocity() - 1, layer->getMaxVelocity());
            dreamSPData += sample->spBinary(sampleAddress, sampleOffset);
            dreamSamplesData += sample->getData();

            sampleAddress += sample->getData().size();
            totalSampleSize += sample->getData().size();
        }
    }

    // HEADER
    s_instr_header soundHeader;

    soundHeader.KW_INST = 0x54534E49;
    soundHeader.KW_MAPP = 0x5050414D;
    soundHeader.KW_META = 0x4154454D;

    soundHeader.full_size = size();

    int mappingAddr = 0;
    const DuArrayConstPtr& mapping = getMapping();
    if (mapping == NULL)
        return QByteArray();

    if (mapping->count() != 0)
    {
        mappingAddr = INSTR_NB_SAMPLES_PER_LAYER_ADDRESS + 2 * nbLayer + (INSTR_DREAM_IP_SIZE + INSTR_DREAM_SP_SIZE) * totalNbSamples + totalSampleSize;
    }

    soundHeader.mapping_addr = mappingAddr;

    int metadataAddr = 0;
    const DuArrayConstPtr& metadata = getMetadata();
    if (metadata == NULL)
        return QByteArray();

    if (metadata->count() != 0)
    {
        if (mappingAddr != 0)
        {
            metadataAddr = mappingAddr + MAPPING_SIZE;
        }
        else
        {
            metadataAddr = INSTR_NB_SAMPLES_PER_LAYER_ADDRESS + 2 * nbLayer + (INSTR_DREAM_IP_SIZE + INSTR_DREAM_SP_SIZE) * totalNbSamples + totalSampleSize;
        }
    }

    soundHeader.meta_addr = metadataAddr;

    data.append((char*)&soundHeader, INSTR_HEADER_SIZE);

    // SOUND STRUCT
    const DuSoundInfoConstPtr &info = getInfo();
    if (info == NULL)
        return QByteArray();

    data += info->toBinary(sampleOffset, nbLayer, totalNbSamples, totalSampleSize);
    data += QByteArray(INTR_STRUCT_ALIGN, 0);

    data += ipHeader;
    data += dreamIPData;
    data += dreamSPData;
    data += dreamSamplesData;

    data += getMapping()->toDuMusicBinary();

    data += getMetadata()->toDuMusicBinary();

    return data;
}

DuObjectPtr DuSound::getChild(const QString &key)
{
    if (    key == KeyName               ||
            key == KeyDreamProgramChange ||
            key == KeyMidiControlChange0 ||
            key == KeyKeyMapping         ||
            key == KeyOctave             ||
            key == KeyUserID             ||
            key == KeyID                 ||
            key == KeyActiveNoteOff      ||
            key == KeyCategory           ||
            key == KeyRelativeVolume     ||
            key == KeyInstrType          ||
            key == KeyInstrVersion)
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
    if (    key == KeyName               ||
            key == KeyDreamProgramChange ||
            key == KeyMidiControlChange0 ||
            key == KeyKeyMapping         ||
            key == KeyOctave             ||
            key == KeyUserID             ||
            key == KeyID                 ||
            key == KeyActiveNoteOff      ||
            key == KeyCategory           ||
            key == KeyRelativeVolume     ||
            key == KeyInstrType          ||
            key == KeyInstrVersion)
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

DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, Name,               DuSoundInfo, Info, QString, QString())
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, DreamProgramChange, DuSoundInfo, Info, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, MidiControlChange0, DuSoundInfo, Info, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, KeyMapping,         DuSoundInfo, Info, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, Octave,             DuSoundInfo, Info, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, UserID,             DuSoundInfo, Info, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, ID,                 DuSoundInfo, Info, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, ActiveNoteOff,      DuSoundInfo, Info, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, Category,           DuSoundInfo, Info, QString, QString())
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, RelativeVolume,     DuSoundInfo, Info, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, InstrType,          DuSoundInfo, Info, INSTRUMENT_TYPE, NUM_INSTR_TYPE)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, InstrVersion,       DuSoundInfo, Info, int, -1)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, Info,         DuSoundInfo)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, LayerArray,   DuArray)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, Mapping,      DuArray)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, Metadata,     DuArray)
