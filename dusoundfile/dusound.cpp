#include "dusound.h"

#include "dulayer.h"
#include "dunote.h"
#include "dusample.h"
#include "dusoundheader.h"
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
    addChild(KeyHeader,     new DuSoundHeader);

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
    return getHeader()->getSize();
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

    DuSoundHeaderPtr header = DuSoundHeader::fromBinary(soundHeader);
    if (header != NULL)
    {
        sound->setHeader(header);
    }
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                      << "Header was not properly generated";

        return DuSoundPtr();
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

    uint32_t sampleOffset = m3Infos->getSampleAddress();

    int nbLayers = m3Infos->getNbLayer();

    QList<uint8_t> nbSamplesPerLayerArray;
    nbSamplesPerLayerArray.reserve(nbLayers);
    for (int i = 0; i < nbLayers; ++i)
    {
        uint8_t nbSamples = (uint8_t)data.data()[INSTR_NB_SAMPLES_PER_LAYER_ADDRESS + (2*i)];
        nbSamplesPerLayerArray << nbSamples;
    }

    QList<dream_ip> dreamIPArray;
    int firstIPAddress = INSTR_NB_SAMPLES_PER_LAYER_ADDRESS + (2 * nbLayers);
    int ipSize = m3Infos->getIPSize() - (2 * nbLayers);
    dreamIPArray.reserve(ipSize / INSTR_DREAM_IP_SIZE);
    for (int i = 0; i < ipSize; i += INSTR_DREAM_IP_SIZE)
    {
        dream_ip dreamIP;
        std::memcpy((char*)&dreamIP, &data.data()[firstIPAddress + i], INSTR_DREAM_IP_SIZE);

        dreamIPArray << dreamIP;
    }

    QList<dream_sp> dreamSPArray;
    int firstSPAddress = firstIPAddress + ipSize;
    int spSize = m3Infos->getSPSize();
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
    int sampleAddress = firstSPAddress + m3Infos->getSPSize();
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

    const dream_sp& currentSampleParam  = dreamSPArray[dreamSPArray.count() - 1];

    uint32_t currentWavAddress  = DuSample::wavAddressDreamToReadable(currentSampleParam.wav_address,   sampleOffset);

    int sampleSize = m3Infos->getSampleSize() - currentWavAddress;
    if (sampleSize <= 0)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                      << "last sample (" << dreamSPArray.count() - 1 << ") size is not positive:" << sampleSize << "\n"
                                      << "m3Infos->getSampleSize():" << m3Infos->getSampleSize() << "\n"
                                      << "currentWavAddress:" << currentWavAddress;

        return DuSoundPtr();
    }

    QByteArray sample(data.mid(sampleAddress, sampleSize));

    sampleArray << sample;


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
                                               sampleArray.mid(sampleCpt, nbSamples),
                                               sampleOffset);
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

    // HEADER
    const DuSoundHeaderConstPtr &header = getHeader();
    if (header == NULL)
        return QByteArray();
    data += header->toDuMusicBinary();

    // SOUND STRUCT
    const DuSoundInfoConstPtr &info = getInfo();
    if (info == NULL)
        return QByteArray();

    data += info->toBinary();
    data += QByteArray(INTR_STRUCT_ALIGN, 0);

    uint32_t sampleOffset = info->getSampleAddress();

    const DuArrayConstPtr& layerArray = getLayerArray();
    if (layerArray == NULL)
        return QByteArray();
    int nbLayer = layerArray->count();
    QByteArray ipHeader(2 * nbLayer, 0);
    QByteArray dreamIPData;
    QByteArray dreamSPData;
    QByteArray dreamSamplesData;
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
            dreamSPData += sample->spBinary(sampleOffset);
            dreamSamplesData += sample->getData();
        }
    }
    data += ipHeader;
    data += dreamIPData;
    data += dreamSPData;
    data += dreamSamplesData;

    data += getMapping()->toDuMusicBinary();

    data += getMetadata()->toDuMusicBinary();

    return data;
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
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, Octave,             DuSoundInfo, Info, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, ActiveNoteOff,      DuSoundInfo, Info, int, -1)
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, Category,           DuSoundInfo, Info, QString, QString())
DU_KEY_ACCESSORS_IN_CHILD_IMPL(DuSound, RelativeVolume,     DuSoundInfo, Info, int, -1)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, Header,       DuSoundHeader)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, Info,         DuSoundInfo)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, LayerArray,   DuArray)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, Mapping,      DuArray)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, Metadata,     DuArray)
