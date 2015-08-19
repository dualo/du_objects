#include "dusound.h"

#include "dudreaminstrparam.h"
#include "dudreamsampleparam.h"
#include "dunote.h"
#include "dusoundheader.h"

#include <cstring>

#include "../dumusicfile/instrument/duinstrumentinfo.h"
#include "../dumusicfile/instrument/dupreset.h"

#include "../dumusicfile/instrument/effects/duadsr.h"
#include "../dumusicfile/instrument/effects/duchorus.h"
#include "../dumusicfile/instrument/effects/ducompressor.h"
#include "../dumusicfile/instrument/effects/dudelay.h"
#include "../dumusicfile/instrument/effects/dudistortion.h"
#include "../dumusicfile/instrument/effects/duequalizer.h"
#include "../dumusicfile/instrument/effects/dumixer.h"
#include "../dumusicfile/instrument/effects/duvibrato.h"
#include "../dumusicfile/instrument/effects/duwah.h"

#include "../general/duarray.h"
#include "../general/dubinarydata.h"
#include "../general/dunumeric.h"

#include <QFile>


DU_OBJECT_IMPL(DuSound)

DuSound::DuSound() :
    DuContainer(),
    m_databaseId(-1)
{
    addChild(KeyHeader,                new DuSoundHeader);

    addChild(KeyInstrumentInfo,        new DuInstrumentInfo);

    addChild(KeyPresetNum,             new DuNumeric(0, NUMERIC_DEFAULT_SIZE, FX_NUM_PRESET_INTR, 0));
    addChild(KeyDisplayLed,            new DuNumeric(0xFF, NUMERIC_DEFAULT_SIZE, 0xFF, 0));

    addChild(KeyPresetArray,           new DuArray(FX_NUM_PRESET_INTR));

    addChild(KeyMixer,                 new DuMixer);
    addChild(KeyDistortionArray,       new DuArray(FX_NUM_FX_INTR));
    addChild(KeyWahArray,              new DuArray(FX_NUM_FX_INTR));
    addChild(KeyCompressorArray,       new DuArray(FX_NUM_FX_INTR));
    addChild(KeyEqualizerArray,        new DuArray(FX_NUM_FX_INTR));
    addChild(KeyDelayArray,            new DuArray(FX_NUM_FX_INTR));
    addChild(KeyChorusArray,           new DuArray(FX_NUM_FX_INTR));
    addChild(KeyVibratoArray,          new DuArray(FX_NUM_FX_INTR));
    addChild(KeyAdsrArray,             new DuArray(FX_NUM_FX_INTR));

    addChild(KeyLedArray,              new DuArray(NUM_LED_VALUE));

    addChild(KeyDreamInstrParamArray,  new DuArray);
    addChild(KeyDreamSampleParamArray, new DuArray);

    addChild(KeySampleArray,           new DuArray);

    addChild(KeyMapping,               new DuArray);

    addChild(KeyMetadata,              new DuArray);
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

    QScopedPointer<struct_instr> soundStruct(new struct_instr);
    std::memcpy((char*)soundStruct.data(), &data.data()[INSTR_HEADER_SIZE], INSTRU_STRUCT_SIZE);

    DuSoundPtr sound(new DuSound);

    if (soundStruct->s_instrument.instr_midi_pc == 0xFF)
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

    DuInstrumentInfoPtr m3Infos = DuInstrumentInfo::fromDuMusicBinary(soundStruct->s_instrument);
    if (m3Infos != NULL)
    {
        sound->setInstrumentInfo(m3Infos);
    }
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                      << "M3 infos were not properly generated";

        return DuSoundPtr();
    }

    uint32_t sampleOffset = m3Infos->getSampleAddress();

    bool verif = true;

    verif = sound->setPresetNum(soundStruct->s_presetnum)       ? verif : false;
    verif = sound->setDisplayLed(soundStruct->s_displayled)     ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "An attribute was not properly set";
    }

    DuArrayPtr presetArray(new DuArray);
    for (int i = 0; i < FX_NUM_PRESET_INTR; ++i)
    {
        DuPresetPtr preset = DuPreset::fromDuMusicBinary(soundStruct->s_preset[i]);
        if (preset != NULL)
        {
            presetArray->append(preset);
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                          << "Preset" << i << "was not properly generated";

            return DuSoundPtr();
        }
    }
    sound->setPresetArray(presetArray);

    DuMixerPtr mixer = DuMixer::fromDuMusicBinary(soundStruct->s_mix);
    if (mixer != NULL)
    {
        sound->setMixer(mixer);
    }
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                      << "Mixer was not properly generated";

        return DuSoundPtr();
    }

    DuArrayPtr distoArray(new DuArray);
    for (int i = 0; i < FX_NUM_FX_INTR; ++i)
    {
        DuDistortionPtr disto = DuDistortion::fromDuMusicBinary(soundStruct->s_distortion[i]);
        if (disto != NULL)
        {
            distoArray->append(disto);
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                          << "Distortion" << i << "was not properly generated";

            return DuSoundPtr();
        }
    }
    sound->setDistortionArray(distoArray);

    DuArrayPtr wahArray(new DuArray);
    for (int i = 0; i < FX_NUM_FX_INTR; ++i)
    {
        DuWahPtr wah = DuWah::fromDuMusicBinary(soundStruct->s_wah[i]);
        if (wah != NULL)
        {
            wahArray->append(wah);
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                          << "Wah" << i << "was not properly generated";

            return DuSoundPtr();
        }
    }
    sound->setWahArray(wahArray);

    DuArrayPtr compressorArray(new DuArray);
    for (int i = 0; i < FX_NUM_FX_INTR; ++i)
    {
        DuCompressorPtr compressor = DuCompressor::fromDuMusicBinary(soundStruct->s_compressor[i]);
        if (compressor != NULL)
        {
            compressorArray->append(compressor);
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                          << "Compressor" << i << "was not properly generated";

            return DuSoundPtr();
        }
    }
    sound->setCompressorArray(compressorArray);

    DuArrayPtr equalizerArray(new DuArray);
    for (int i = 0; i < FX_NUM_FX_INTR; ++i)
    {
        DuEqualizerPtr equalizer = DuEqualizer::fromDuMusicBinary(soundStruct->s_equalizer[i]);
        if (equalizer != NULL)
        {
            equalizerArray->append(equalizer);
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                          << "Equalizer" << i << "was not properly generated";

            return DuSoundPtr();
        }
    }
    sound->setEqualizerArray(equalizerArray);

    DuArrayPtr delayArray(new DuArray);
    for (int i = 0; i < FX_NUM_FX_INTR; ++i)
    {
        DuDelayPtr delay = DuDelay::fromDuMusicBinary(soundStruct->s_delay[i]);
        if (delay != NULL)
        {
            delayArray->append(delay);
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                          << "Delay" << i << "was not properly generated";

            return DuSoundPtr();
        }
    }
    sound->setDelayArray(delayArray);

    DuArrayPtr chorusArray(new DuArray);
    for (int i = 0; i < FX_NUM_FX_INTR; ++i)
    {
        DuChorusPtr chorus = DuChorus::fromDuMusicBinary(soundStruct->s_chorus[i]);
        if (chorus != NULL)
        {
            chorusArray->append(chorus);
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                          << "Chorus" << i << "was not properly generated";

            return DuSoundPtr();
        }
    }
    sound->setChorusArray(chorusArray);

    DuArrayPtr vibratoArray(new DuArray);
    for (int i = 0; i < FX_NUM_FX_INTR; ++i)
    {
        DuVibratoPtr vibrato = DuVibrato::fromDuMusicBinary(soundStruct->s_vibrato[i]);
        if (vibrato != NULL)
        {
            vibratoArray->append(vibrato);
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                          << "Vibrato" << i << "was not properly generated";

            return DuSoundPtr();
        }
    }
    sound->setVibratoArray(vibratoArray);

    DuArrayPtr adsrArray(new DuArray);
    for (int i = 0; i < FX_NUM_FX_INTR; ++i)
    {
        DuAdsrPtr adsr = DuAdsr::fromDuMusicBinary(soundStruct->s_adsr[i]);
        if (adsr != NULL)
        {
            adsrArray->append(adsr);
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                          << "ADSR" << i << "was not properly generated";

            return DuSoundPtr();
        }
    }
    sound->setAdsrArray(adsrArray);

    DuArrayPtr ledsArray(new DuArray);
    for (int i = 0; i < NUM_LED_VALUE; ++i)
    {
        DuNumericPtr leds(new DuNumeric(soundStruct->s_leds[i], NUMERIC_DEFAULT_SIZE, 0xFF, 0x000, 0));
        if (leds != NULL)
        {
            ledsArray->append(leds);
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                          << "Leds" << i << "was not properly generated";

            return DuSoundPtr();
        }
    }
    sound->setLedArray(ledsArray);

    int nbLayers = m3Infos->getNbLayer();

    DuArrayPtr nbSamplesPerLayerArray(new DuArray);
    for (int i = 0; i < nbLayers; ++i)
    {
        uint8_t nbSamples = (uint8_t)data.data()[INSTR_NB_SAMPLES_PER_LAYER_ADDRESS + (2*i)];

        DuNumericPtr nbSamplesObject(new DuNumeric(nbSamples, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00, 0));
        if (nbSamplesObject != NULL)
        {
            nbSamplesPerLayerArray->append(nbSamplesObject);
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                          << "NbSamples for layer" << i << "was not properly generated";

            return DuSoundPtr();
        }
    }
    sound->setNbSamplesPerLayerArray(nbSamplesPerLayerArray);

    DuArrayPtr dreamIPArray(new DuArray);
    int firstIPAddress = INSTR_NB_SAMPLES_PER_LAYER_ADDRESS + (2 * nbLayers);
    int ipSize = m3Infos->getIPSize() - (2 * nbLayers);
    for (int i = 0; i < ipSize; i += INSTR_DREAM_IP_SIZE)
    {
        dream_ip dreamIP;
        std::memcpy((char*)&dreamIP, &data.data()[firstIPAddress + i], INSTR_DREAM_IP_SIZE);

        DuDreamInstrParamPtr dreamIPObject = DuDreamInstrParam::fromBinary(dreamIP);
        if (dreamIPObject != NULL)
        {
            dreamIPArray->append(dreamIPObject);
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                          << "dreamIP" << i / INSTR_DREAM_IP_SIZE << "was not properly generated";

            return DuSoundPtr();
        }
    }
    sound->setDreamInstrParamArray(dreamIPArray);


    DuArrayPtr dreamSPArray(new DuArray);
    int firstSPAddress = firstIPAddress + ipSize;
    int spSize = m3Infos->getSPSize();
    for (int i = 0; i < spSize; i += INSTR_DREAM_SP_SIZE)
    {
        dream_sp dreamSP;
        std::memcpy((char*)&dreamSP, &data.data()[firstSPAddress + i], INSTR_DREAM_SP_SIZE);

        if (m3Infos->getDreamFormatId() == DuInstrumentInfo::SDK_3000)
        {
            if (dreamSP.loopType != DuDreamSampleParam::SND3000_Forward)
            {
                qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                              << "dreamSP" << i / INSTR_DREAM_SP_SIZE << "was not properly generated\n"
                                              << "dream format id is 3000 but loop type is" << dreamSP.loopType;

                return DuSoundPtr();
            }
        }
        else if (m3Infos->getDreamFormatId() == DuInstrumentInfo::SDK_5000)
        {
            if (dreamSP.loopType != DuDreamSampleParam::SND5000_Forward
                    && dreamSP.loopType != DuDreamSampleParam::SND5000_OneShot)
            {
                qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                              << "dreamSP" << i / INSTR_DREAM_SP_SIZE << "was not properly generated\n"
                                              << "dream format id is 5000 but loop type is" << dreamSP.loopType;

                return DuSoundPtr();
            }
        }

        DuDreamSampleParamPtr dreamSPObject = DuDreamSampleParam::fromBinary(dreamSP, sampleOffset);
        if (dreamSPObject != NULL)
        {
            dreamSPArray->append(dreamSPObject);
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                          << "dreamSP" << i / INSTR_DREAM_SP_SIZE << "was not properly generated";

            return DuSoundPtr();
        }
    }
    sound->setDreamSampleParamArray(dreamSPArray);


    DuArrayPtr sampleArray(new DuArray);
    int sampleAddress = firstSPAddress + m3Infos->getSPSize();
    for (int i = 0; i < dreamSPArray->count() - 1; ++i)
    {
        const DuDreamSampleParamConstPtr& currentSampleParam = dreamSPArray->at(i).dynamicCast<const DuDreamSampleParam>();
        const DuDreamSampleParamConstPtr& nextSampleParam = dreamSPArray->at(i+1).dynamicCast<const DuDreamSampleParam>();
        Q_ASSERT(currentSampleParam != NULL);
        Q_ASSERT(nextSampleParam != NULL);

        int sampleSize = nextSampleParam->getWavAddress() - currentSampleParam->getWavAddress();
        if (sampleSize <= 0)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                          << "sample" << i << "size is not positive:" << sampleSize << "\n"
                                          << "nextSampleParam->getWavAddress():" << nextSampleParam->getWavAddress() << "\n"
                                          << "currentSampleParam->getWavAddress():" << currentSampleParam->getWavAddress();

            return DuSoundPtr();
        }

        DuBinaryDataPtr sampleObject(new DuBinaryData);
        sampleObject->setData(data.mid(sampleAddress, sampleSize));

        sampleArray->append(sampleObject);

        sampleAddress += sampleSize;
    }

    const DuDreamSampleParamConstPtr& currentSampleParam = dreamSPArray->at(dreamSPArray->count() - 1).dynamicCast<const DuDreamSampleParam>();
    Q_ASSERT(currentSampleParam != NULL);
    int sampleSize = m3Infos->getSampleSize() - currentSampleParam->getWavAddress();
    if (sampleSize <= 0)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate du-sound:\n"
                                      << "last sample (" << dreamSPArray->count() - 1 << ") size is not positive:" << sampleSize << "\n"
                                      << "m3Infos->getSampleSize():" << m3Infos->getSampleSize() << "\n"
                                      << "currentSampleParam->getWavAddress():" << currentSampleParam->getWavAddress();

        return DuSoundPtr();
    }

    QByteArray sample(data.mid(sampleAddress, sampleSize));

    DuBinaryDataPtr sampleObject(new DuBinaryData);
    sampleObject->setData(sample);

    sampleArray->append(sampleObject);

    sound->setSampleArray(sampleArray);


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

    QFile file("/Users/nkniebilher/Documents/du-sounds/3L_after.dusound");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    QByteArray binaryData = sound->toBinary();
    out.writeRawData(binaryData.data(), binaryData.size());

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

    int tmpInt;

    // HEADER
    const DuSoundHeaderConstPtr &header = getHeader();
    if (header == NULL)
        return QByteArray();
    data += header->toDuMusicBinary();

    // SOUND STRUCT
    QScopedPointer<struct_instr> soundStruct(new struct_instr);

    const DuInstrumentInfoConstPtr &m3infos = getInstrumentInfo();
    if (m3infos == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct->s_instrument), m3infos->toDuMusicBinary().data(), m3infos->size());

    tmpInt = getPresetNum();
    if (tmpInt == -1)
        return QByteArray();
    soundStruct->s_presetnum = tmpInt;

    tmpInt = getDisplayLed();
    if (tmpInt == -1)
        return QByteArray();
    soundStruct->s_displayled = tmpInt;

    const DuArrayConstPtr &presetArray = getPresetArray();
    if (presetArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct->s_preset), presetArray->toDuMusicBinary().data(), presetArray->size());

    const DuMixerConstPtr &mixer = getMixer();
    if (mixer == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct->s_mix), mixer->toDuMusicBinary().data(), mixer->size());

    const DuArrayConstPtr &distortionArray = getDistortionArray();
    if (distortionArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct->s_distortion), distortionArray->toDuMusicBinary().data(), distortionArray->size());

    const DuArrayConstPtr &wahArray = getWahArray();
    if (wahArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct->s_wah), wahArray->toDuMusicBinary().data(), wahArray->size());

    const DuArrayConstPtr &compressorArray = getCompressorArray();
    if (compressorArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct->s_compressor), compressorArray->toDuMusicBinary().data(), compressorArray->size());

    const DuArrayConstPtr &equalizerArray = getEqualizerArray();
    if (equalizerArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct->s_equalizer), equalizerArray->toDuMusicBinary().data(), equalizerArray->size());

    const DuArrayConstPtr &delayArray = getDelayArray();
    if (delayArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct->s_delay), delayArray->toDuMusicBinary().data(), delayArray->size());

    const DuArrayConstPtr &chorusArray = getChorusArray();
    if (chorusArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct->s_chorus), chorusArray->toDuMusicBinary().data(), chorusArray->size());

    const DuArrayConstPtr &vibratoArray = getVibratoArray();
    if (vibratoArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct->s_vibrato), vibratoArray->toDuMusicBinary().data(), vibratoArray->size());

    const DuArrayConstPtr &adsrArray = getAdsrArray();
    if (adsrArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct->s_adsr), adsrArray->toDuMusicBinary().data(), adsrArray->size());

    const DuArrayConstPtr &ledsArray = getLedArray();
    if (ledsArray == NULL)
        return QByteArray();
    std::memcpy((char*)&(soundStruct->s_leds), ledsArray->toDuMusicBinary().data(), ledsArray->size());

    data += QByteArray((char*)soundStruct.data(), INSTRU_STRUCT_SIZE);
    data += QByteArray(INTR_STRUCT_ALIGN, 0);

    const DuArrayConstPtr &nbSamplesPerLayerArray = getNbSamplesPerLayerArray();
    if (nbSamplesPerLayerArray == NULL)
        return QByteArray();
    int nbLayer = nbSamplesPerLayerArray->count();
    QByteArray ipHeader(2 * nbLayer, 0);
    for (int i = 0; i < nbLayer; ++i)
    {
        ipHeader[i * 2] = ((DuNumericConstPtr)nbSamplesPerLayerArray->at(i).dynamicCast<const DuNumeric>())->getNumeric();
        if (i == 0)
            ipHeader[(i * 2) + 1] = nbLayer;
        else
            ipHeader[(i * 2) + 1] = 0;
    }
    data += ipHeader;

    data += getDreamInstrParamArray()->toDuMusicBinary();

    data += getDreamSampleParamArray()->toDuMusicBinary();

    data += getSampleArray()->toDuMusicBinary();

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

QString DuSound::name() const
{
    const DuInstrumentInfoConstPtr& instrInfo = getInstrumentInfo();

    if (instrInfo == NULL)
    {
        return QString();
    }

    return instrInfo->getName();
}

bool DuSound::setName(const QString &name)
{
    const DuInstrumentInfoPtr &instrInfo = getChildAs<DuInstrumentInfo>(KeyInstrumentInfo);

    if (instrInfo == NULL)
    {
        return false;
    }

    return instrInfo->setName(name);
}

int DuSound::dreamProgramChange() const
{
    const DuInstrumentInfoConstPtr& instrInfo = getInstrumentInfo();

    if (instrInfo == NULL)
    {
        return -1;
    }

    return instrInfo->getDreamProgramChange();
}

bool DuSound::setDreamProgramChange(int value)
{
    const DuInstrumentInfoPtr &instrInfo = getChildAs<DuInstrumentInfo>(KeyInstrumentInfo);

    if (instrInfo == NULL)
    {
        return false;
    }

    return instrInfo->setDreamProgramChange(value);
}

int DuSound::octave() const
{
    const DuInstrumentInfoConstPtr& instrInfo = getInstrumentInfo();

    if (instrInfo == NULL)
    {
        return -1;
    }

    return instrInfo->getOctave();
}

bool DuSound::setOctave(int value)
{
    const DuInstrumentInfoPtr &instrInfo = getChildAs<DuInstrumentInfo>(KeyInstrumentInfo);

    if (instrInfo == NULL)
    {
        return false;
    }

    return instrInfo->setOctave(value);
}

int DuSound::activeNoteOff() const
{
    const DuInstrumentInfoConstPtr& instrInfo = getInstrumentInfo();

    if (instrInfo == NULL)
    {
        return -1;
    }

    return instrInfo->getActiveNoteOff();
}

bool DuSound::setActiveNoteOff(int value)
{
    const DuInstrumentInfoPtr &instrInfo = getChildAs<DuInstrumentInfo>(KeyInstrumentInfo);

    if (instrInfo == NULL)
    {
        return false;
    }

    return instrInfo->setActiveNoteOff(value);
}

QString DuSound::category() const
{
    const DuInstrumentInfoConstPtr& instrInfo = getInstrumentInfo();

    if (instrInfo == NULL)
    {
        return QString();
    }

    return instrInfo->getCategory();
}

bool DuSound::setCategory(const QString &value)
{
    const DuInstrumentInfoPtr &instrInfo = getChildAs<DuInstrumentInfo>(KeyInstrumentInfo);

    if (instrInfo == NULL)
    {
        return false;
    }

    return instrInfo->setCategory(value);
}

int DuSound::relativeVolume() const
{
    const DuInstrumentInfoConstPtr& instrInfo = getInstrumentInfo();

    if (instrInfo == NULL)
    {
        return -1;
    }

    return instrInfo->getRelativeVolume();
}

bool DuSound::setRelativeVolume(int value)
{
    const DuInstrumentInfoPtr &instrInfo = getChildAs<DuInstrumentInfo>(KeyInstrumentInfo);

    if (instrInfo == NULL)
    {
        return false;
    }

    return instrInfo->setRelativeVolume(value);
}

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, Header,                 DuSoundHeader)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, InstrumentInfo,         DuInstrumentInfo)

DU_KEY_ACCESSORS_IMPL(DuSound, PresetNum,       Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSound, DisplayLed,      Numeric, int, -1)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, PresetArray,            DuArray)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, Mixer,                  DuMixer)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, DistortionArray,        DuArray)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, WahArray,               DuArray)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, CompressorArray,        DuArray)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, EqualizerArray,         DuArray)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, DelayArray,             DuArray)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, ChorusArray,            DuArray)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, VibratoArray,           DuArray)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, AdsrArray,              DuArray)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, LedArray,               DuArray)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, NbSamplesPerLayerArray, DuArray)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, DreamInstrParamArray,   DuArray)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, DreamSampleParamArray,  DuArray)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, SampleArray,            DuArray)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, Mapping,                DuArray)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSound, Metadata,               DuArray)
