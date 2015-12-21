#include "dusample.h"

#include <du_objects/general/dubinarydata.h>
#include <du_objects/general/dunumeric.h>

#include <QFile>
#include <QStandardPaths>
#include <QtEndian>

#include <cstring>

DU_OBJECT_IMPL(DuSample)

DuSample::DuSample() :
    DuContainer()
{
    // Intrument Parameters
    addChild(KeyStartNote,       new DuNumeric(0x01, NUMERIC_DEFAULT_SIZE, 0x7F, 0x01));
    addChild(KeyEndNote,         new DuNumeric(0x7F, NUMERIC_DEFAULT_SIZE, 0x7F, 0x01));

    // Sample Parameters
    addChild(KeyAddress1,        new DuNumeric(0x0000, 2, 0xFFFF, 0x0000));
    addChild(KeyLoopType,        new DuNumeric(SND3000_Forward, 2, 0xFFFF, 0x0000));
    addChild(KeyAddress2,        new DuNumeric(0x0000, 2, 0xFFFF, 0x0000));
    addChild(KeyAddress3,        new DuNumeric(0x0000, 2, 0xFFFF, 0x0000));

    addChild(KeyVolumeAmplifier, new DuNumeric(0xFF, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));

    addChild(KeyFineTune,        new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 100, 0x00));
    addChild(KeyUnityNote,       new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0x7F, 0x00));

    addChild(KeyLoopStart,       new DuNumeric(0));

    addChild(KeyVolumeMixer1,    new DuNumeric(0xEE, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));

    addChild(KeyLoopEnd,         new DuNumeric(0));

    addChild(KeyAmplitudeOscAmp, new DuNumeric(0x46, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));
    addChild(KeyVolumeMixer2,    new DuNumeric(0xEFEF, 2, 0xFFFF, 0x0000));

    addChild(KeyInit,            new DuNumeric(0x7F00, 2, 0xFFFF, 0x0000));
    addChild(KeyAttack,          new DuNumeric(0x1FE3, 2, 0xFFFF, 0x0000));
    addChild(KeyDecay,           new DuNumeric(0x5FE3, 2, 0xFFFF, 0x0000));
    addChild(KeyRelease,         new DuNumeric(0x605A, 2, 0xFFFF, 0x0000));

    // Data
    addChild(KeyData,            new DuBinaryData);
}

DuObjectPtr DuSample::clone() const
{
    return DuSamplePtr(new DuSample(*this));
}

DuSamplePtr DuSample::fromBinary(const dream_ip& dreamIP,
                                 const dream_sp& dreamSP,
                                 const QByteArray& data,
                                 uint32_t sampleOffset)
{
    DuSamplePtr sample(new DuSample);
    bool verif = true;

    // Intrument Parameters
    verif = sample->setStartNote(dreamIP.start_note + 1) ? verif : false;
    verif = sample->setEndNote(dreamIP.end_note) ? verif : false;

    // Sample Parameters
    verif = sample->setAddress1(dreamSP.address1) ? verif : false;
    verif = sample->setLoopType((SampleType)dreamSP.loopType) ? verif : false;
    verif = sample->setAddress2(dreamSP.address2) ? verif : false;
    verif = sample->setAddress3(dreamSP.address3) ? verif : false;

    verif = sample->setVolumeAmplifier(dreamSP.volume_amplifier) ? verif : false;

    int convertedFineTune = -1;
    for (int i = 0; i < 100; ++i)
    {
        if (dreamSP.fine_tune == finetune_convert[i])
        {
            convertedFineTune = i;
            break;
        }
    }
    if (convertedFineTune == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate sample:\n"
                                      << "Fine tune not found in array:" << dreamSP.fine_tune;
        return DuSamplePtr();
    }
    verif = sample->setFineTune(convertedFineTune) ? verif : false;

    int unityNote = 0;
    if (dreamSP.coarse_tune >= 0 && dreamSP.coarse_tune <= 64)
    {
        unityNote = 64 - dreamSP.coarse_tune;
    }
    else if (dreamSP.coarse_tune >= 193 && dreamSP.coarse_tune <= 255)
    {
        unityNote = 320 - dreamSP.coarse_tune;
    }
    else
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate sample:\n"
                                      << "Coarse tune out of bounds:" << dreamSP.coarse_tune;
        return DuSamplePtr();
    }
    verif = sample->setUnityNote(unityNote) ? verif : false;

    uint32_t sampleStartAddress = wavAddressDreamToReadable(dreamSP.wav_address, sampleOffset) + sampleOffset;

    verif = sample->setLoopStart((int) loopStartDreamToReadable(dreamSP.loop_start_MSB, dreamSP.loop_start_LSB, sampleStartAddress)) ? verif : false;

    verif = sample->setVolumeMixer1(volumeDreamToReadable(dreamSP.volume_mixer1)) ? verif : false;

    verif = sample->setLoopEnd((int) loopEndDreamToReadable(dreamSP.loop_end_MSB, dreamSP.loop_end_LSB, sampleStartAddress)) ? verif : false;

    verif = sample->setAmplitudeOscAmp(dreamSP.amplitude_osc_amp) ? verif : false;
    verif = sample->setVolumeMixer2(dreamSP.volume_mixer2) ? verif : false;

    verif = sample->setInit(dreamSP.init)       ? verif : false;
    verif = sample->setAttack(dreamSP.attack)   ? verif : false;
    verif = sample->setDecay(dreamSP.decay)     ? verif : false;
    verif = sample->setRelease(dreamSP.release) ? verif : false;

    // Data
    verif = sample->setData(data) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "an attribute was not properly set";
    }

    return sample;
}

DuSamplePtr DuSample::fromWav(QFile *input)
{
    // Copy file to temp folder
    QString wavFileName = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/tmp.wav";
    if (wavFileName.isEmpty())
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to generate DuSample\n"
                << "can't find temp folder";

        return DuSamplePtr();
    }

    if (QFile::exists(wavFileName))
    {
        if (!QFile::remove(wavFileName))
        {
            qCCritical(LOG_CAT_DU_OBJECT)
                    << "Failed to generate DuSample\n"
                    << "couldn't remove previous temp file :" << wavFileName;

            return DuSamplePtr();
        }
    }

    if (!input->copy(wavFileName))
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to generate DuSample\n"
                << "selected file could not be copied to temp folder";

        return DuSamplePtr();
    }

    // Open file
    SndfileHandle soundFile(wavFileName.toStdString(), SFM_READ, 0);
    if (soundFile.rawHandle() == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to open the file" << wavFileName << ":\n"
                                      << soundFile.strError();
        return DuSamplePtr();
    }

    qint64 wavSize = soundFile.frames() * soundFile.channels();

    // Convert stereo to mono
    if (soundFile.channels() == 2)
    {
        qCDebug(LOG_CAT_DU_OBJECT) << "Stereo sample, take 1 sample on 2";

        wavFileName = convertToMono(soundFile);
        if (wavFileName.isEmpty())
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to convert to mono";
            return DuSamplePtr();
        }

        soundFile = SndfileHandle(wavFileName.toStdString(), SFM_READ, 0);
        if (soundFile.rawHandle() == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to open the file" << wavFileName << ":\n"
                                          << soundFile.strError();
            return DuSamplePtr();
        }

        wavSize /= 2;
    }

    // Convert to 16 bits
    if ((soundFile.format() & SF_FORMAT_SUBMASK) > 2)
    {
        qCDebug(LOG_CAT_DU_OBJECT) << "Wrong sample resolution => " << (soundFile.format() & SF_FORMAT_SUBMASK) * 8 << " bits";

        wavFileName = convertTo16bits(soundFile);
        if (wavFileName.isEmpty())
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to convert to 16 bits";
            return DuSamplePtr();
        }

        soundFile = SndfileHandle(wavFileName.toStdString(), SFM_READ, 0);
        if (soundFile.rawHandle() == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to open the file" << wavFileName << ":\n"
                                          << soundFile.strError();
            return DuSamplePtr();
        }

        wavSize *= 2; // 16 bits (2*8)
    }
    else
    {
        wavSize *= (soundFile.format() & SF_FORMAT_SUBMASK);
    }

    // Arrondi PAIR
    wavSize /= 2;
    wavSize *= 2;


    qCDebug(LOG_CAT_DU_OBJECT) << "size in wav =" << QString::number(wavSize, 16) << "o";


    QFile wavFile(wavFileName);
    if (!wavFile.open(QIODevice::ReadOnly))
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to open the file" << wavFileName << ":\n"
                                      << wavFile.errorString();
        return DuSamplePtr();
    }

    /***** Checking Error *****/
    qint64 diff = wavSize - wavFile.size();
    qCDebug(LOG_CAT_DU_OBJECT) << "Size wav = 0x" << QString::number(wavSize, 16) << "o / File Size = 0x" << QString::number(wavFile.size(), 16) << "o";

    if (diff < -(wavFile.size() / 2)) //(-(CNT_MAX_FOR_WAV_SIZE*10))
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "Size error : too far from the end of the file";
    }

    if (diff > 0)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "Size error : sample bigger than the file size";
    }

    QByteArray wavFileData = wavFile.readAll();
    int dataKeywordIndex = wavFileData.indexOf(QByteArrayLiteral("data"));
    if (dataKeywordIndex == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Can't find keyword 'data' in wav file";
        return DuSamplePtr();
    }

    quint32 sizeInFile = qFromLittleEndian<quint32>((uchar*)wavFileData.mid(dataKeywordIndex + 4, 4).data());
    if (sizeInFile != wavSize)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "Size in file is different from calculated size:\n"
                                     << "size in file:" << sizeInFile << "o\n"
                                     << "calculated:" << wavSize << "o";
    }

    QByteArray rawData = wavFileData.mid(dataKeywordIndex + 4 + 4, (int) wavSize); // +4 for "data", +4 for size

    DuSamplePtr sample(new DuSample);

    sample->setData(rawData);

    return sample;
}

QString DuSample::convertToMono(SndfileHandle &oldSoundFile)
{
    QString monoWavFileName = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/tmp_mono.wav";
    if (monoWavFileName.isEmpty())
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to convert to mono\n"
                << "can't find temp folder";
        return QString();
    }

    if (QFile::exists(monoWavFileName))
    {
        if (!QFile::remove(monoWavFileName))
        {
            qCCritical(LOG_CAT_DU_OBJECT)
                    << "Failed to convert to mono\n"
                    << "couldn't remove previous temp file :" << monoWavFileName;
            return QString();
        }
    }

    SndfileHandle newSoundFile(monoWavFileName.toStdString(), SFM_WRITE, oldSoundFile.format(), 1, oldSoundFile.samplerate());
    if (newSoundFile.rawHandle() == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to open the file" << monoWavFileName << ":\n"
                                      << newSoundFile.strError();
        return QString();
    }

    SF_INSTRUMENT inst;
    if (oldSoundFile.command(SFC_GET_INSTRUMENT, &inst, sizeof(inst)) != SF_TRUE)
    {
        qCDebug(LOG_CAT_DU_OBJECT) << "No instrument info found";
    }

    if (newSoundFile.command(SFC_SET_INSTRUMENT, &inst, sizeof(inst)) != SF_TRUE)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to convert to mono\n"
                << "can't set instrument :" << newSoundFile.strError();
        return QString();
    }

    const qint64 oldNbFrames = oldSoundFile.channels() * oldSoundFile.frames();
    QScopedArrayPointer<double> oldFrames(new double[oldNbFrames]);
    sf_count_t nbFramesRead = oldSoundFile.readf(oldFrames.data(), oldSoundFile.frames());
    if (nbFramesRead != oldSoundFile.frames())
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to convert to mono\n"
                << "error reading frames from stereo sound"
                << "info.frames =" << oldSoundFile.frames() << "\n"
                << "frames read =" << nbFramesRead << "\n"
                << oldSoundFile.strError();
        return QString();
    }

    const qint64 newNbFrames = newSoundFile.channels() * oldSoundFile.frames();
    QScopedArrayPointer<double> newFrames(new double[newNbFrames]);
    for (int i = 0; i < oldSoundFile.frames(); ++i)
    {
        newFrames[i] = 0;
        for (int j = 0; j < oldSoundFile.channels(); ++j)
        {
            newFrames[i] += oldFrames[i * oldSoundFile.channels() + j];
        }

        newFrames[i] /= oldSoundFile.channels();
    }

    sf_count_t writtenFrames = newSoundFile.writef(newFrames.data(), oldSoundFile.frames());
    if (writtenFrames != oldSoundFile.frames())
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to convert to mono\n"
                << "error writing frames to mono sound"
                << "info.frames =" << oldSoundFile.frames() << "\n"
                << "frames written =" << writtenFrames << "\n"
                << newSoundFile.strError();
        return QString();
    }

    newSoundFile.writeSync();

    return monoWavFileName;
}

QString DuSample::convertTo16bits(SndfileHandle &oldSoundFile)
{
    QString wav16bitsFileName = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/tmp_16bits.wav";
    if (wav16bitsFileName.isEmpty())
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to convert to 16 bits\n"
                << "can't find temp folder";
        return QString();
    }

    if (QFile::exists(wav16bitsFileName))
    {
        if (!QFile::remove(wav16bitsFileName))
        {
            qCCritical(LOG_CAT_DU_OBJECT)
                    << "Failed to convert to 16 bits\n"
                    << "couldn't remove previous temp file :" << wav16bitsFileName;
            return QString();
        }
    }

    SndfileHandle newSoundFile(wav16bitsFileName.toStdString(), SFM_WRITE, normalizeWaveType(oldSoundFile.format()), oldSoundFile.channels(), oldSoundFile.samplerate());
    if (newSoundFile.rawHandle() == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to open the file" << wav16bitsFileName << ":\n"
                                      << newSoundFile.strError();
        return QString();
    }

    SF_INSTRUMENT inst;
    if (oldSoundFile.command(SFC_GET_INSTRUMENT, &inst, sizeof(inst)) != SF_TRUE)
    {
        qCDebug(LOG_CAT_DU_OBJECT) << "No instrument info found";
    }

    if (newSoundFile.command(SFC_SET_INSTRUMENT, &inst, sizeof(inst)) != SF_TRUE)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to convert to 16 bits\n"
                << "can't set instrument :" << newSoundFile.strError();
        return QString();
    }

    const qint64 oldNbFrames = oldSoundFile.channels() * oldSoundFile.frames();
    QScopedArrayPointer<double> frames(new double[oldNbFrames]);
    sf_count_t nbFramesRead = oldSoundFile.readf(frames.data(), oldSoundFile.frames());
    if (nbFramesRead != oldSoundFile.frames())
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to convert to 16 bits\n"
                << "error reading frames from old sound"
                << "info.frames =" << oldSoundFile.frames() << "\n"
                << "frames read =" << nbFramesRead << "\n"
                << oldSoundFile.strError();
        return QString();
    }

    sf_count_t writtenFrames = newSoundFile.writef(frames.data(), oldSoundFile.frames());
    if (writtenFrames != oldSoundFile.frames())
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to convert to mono\n"
                << "error writing frames to mono sound"
                << "info.frames =" << oldSoundFile.frames() << "\n"
                << "frames written =" << writtenFrames << "\n"
                << newSoundFile.strError();
        return QString();
    }

    newSoundFile.writeSync();

    return wav16bitsFileName;
}

int DuSample::normalizeWaveType(int format)
{
    quint16 subtype = format & SF_FORMAT_SUBMASK;
    quint32 majorFormat = format & SF_FORMAT_TYPEMASK;
    quint32 newFormat = 0x00;

    qCDebug(LOG_CAT_DU_OBJECT) << "Resolution =" << subtype * 8 << "bits";
    qCDebug(LOG_CAT_DU_OBJECT) << "MajorFormat =" << majorFormat;

    subtype = 0x02;

    newFormat = majorFormat | subtype;

    qCDebug(LOG_CAT_DU_OBJECT) << "New format =" << QString::number(newFormat, 16);

    return (int) newFormat;
}

QByteArray DuSample::ipBinary(uint8_t min_vel, uint8_t max_vel) const
{
    dream_ip dataStruct;

    std::memcpy((char*)&dataStruct, QByteArray(INSTR_DREAM_IP_SIZE, 0x00), INSTR_DREAM_IP_SIZE);

    int tmpNum = 0;

    dataStruct.min_vel = min_vel;

    tmpNum = getStartNote() - 1;
    if (tmpNum == -1)
        return QByteArray();
    dataStruct.start_note = (uint8_t) tmpNum;

    dataStruct.max_vel = max_vel;

    tmpNum = getEndNote();
    if (tmpNum == -1)
        return QByteArray();
    dataStruct.end_note = (uint8_t) tmpNum;

    return QByteArray((char*)&dataStruct, INSTR_DREAM_IP_SIZE);
}

QByteArray DuSample::spBinary(uint32_t sampleAddress, uint32_t sampleOffset) const
{
    dream_sp data;

    std::memcpy((char*)&data, QByteArray(INSTR_DREAM_SP_SIZE, 0x00), INSTR_DREAM_SP_SIZE);

    int tmpNum = 0;

    tmpNum = getAddress1();
    if (tmpNum == -1)
        return QByteArray();
    data.address1 = (uint16_t) tmpNum;

    SampleType tmpSampleType = getLoopType();
    if (tmpSampleType == Unknown)
        return QByteArray();
    data.loopType = (uint16_t) tmpSampleType;

    tmpNum = getAddress2();
    if (tmpNum == -1)
        return QByteArray();
    data.address2 = (uint16_t) tmpNum;

    tmpNum = getAddress3();
    if (tmpNum == -1)
        return QByteArray();
    data.address3 = (uint16_t) tmpNum;

    data.unknown1 = 0xB5F0;
    data.unknown2 = 0x0814;
    data.unknown3 = 0xFF00;
    data.unknown4 = 0x1032;
    data.unknown5 = 0x04;

    tmpNum = getVolumeAmplifier();
    if (tmpNum == -1)
        return QByteArray();
    data.volume_amplifier = (uint8_t) tmpNum;

    data.unknown6 = 0x0081;

    tmpNum = getFineTune();
    if (tmpNum == -1 || tmpNum >= 100)
        return QByteArray();
    data.fine_tune = finetune_convert[tmpNum];

    tmpNum = getUnityNote();
    if (tmpNum == -1)
        return QByteArray();
    if (tmpNum <= 64)
        data.coarse_tune = (uint8_t) (64 - tmpNum);
    else // tmpNum > 64
        data.coarse_tune = (uint8_t) (320 - tmpNum);

    tmpNum = getLoopStart();
    if (tmpNum == -1)
        return QByteArray();
    loopStartReadableToDream((uint32_t) tmpNum, sampleAddress + sampleOffset, data.loop_start_MSB, data.loop_start_LSB);

    data.wav_address = wavAddressReadableToDream(sampleAddress, sampleOffset);

    tmpNum = volumeReadableToDream(getVolumeMixer1());
    if (tmpNum == -1)
        return QByteArray();
    data.volume_mixer1 = (uint16_t) tmpNum;

    tmpNum = getLoopEnd();
    if (tmpNum == -1)
        return QByteArray();
    loopEndReadableToDream((uint32_t) tmpNum, sampleAddress + sampleOffset, data.loop_end_MSB, data.loop_end_LSB);

    data.unknown7 = 0x7F0E;
    data.unknown8 = 0x06;

    tmpNum = getAmplitudeOscAmp();
    if (tmpNum == -1)
        return QByteArray();
    data.amplitude_osc_amp = (uint8_t) tmpNum;

    tmpNum = getVolumeMixer2();
    if (tmpNum == -1)
        return QByteArray();
    data.volume_mixer2 = (uint16_t) tmpNum;

    tmpNum = getInit();
    if (tmpNum == -1)
        return QByteArray();
    data.init = (uint16_t) tmpNum;

    tmpNum = getAttack();
    if (tmpNum == -1)
        return QByteArray();
    data.attack = (uint16_t) tmpNum;

    tmpNum = getDecay();
    if (tmpNum == -1)
        return QByteArray();
    data.decay = (uint16_t) tmpNum;

    tmpNum = getRelease();
    if (tmpNum == -1)
        return QByteArray();
    data.release = (uint16_t) tmpNum;

    data.unknown13 = 0x0401;
    data.unknown14 = 0x0F00;
    data.unknown15 = 0x0015;
    data.unknown16 = 0x0403;
    data.unknown17 = 0x0010;
    data.unknown18 = 0x0201;

    data.unknown19 = 0x00000000;
    data.unknown20 = 0x00000000;
    data.unknown21 = 0x0000;

    data.unknown22 = 0x0002;
    data.unknown23 = 0x00000000;

    return QByteArray((char*)&data, INSTR_DREAM_SP_SIZE);
}

uint32_t DuSample::wavAddressDreamToReadable(uint32_t dreamValue, uint32_t sampleOffset)
{
    uint32_t reorderedWavAddress = 0;
    reorderedWavAddress |= (0x000000FF & dreamValue) << 0;
    reorderedWavAddress |= (0x0000FF00 & dreamValue) << 16;
    reorderedWavAddress |= (0x00FF0000 & dreamValue) >> 8;
    reorderedWavAddress |= (0xFF000000 & dreamValue) >> 8;
    return (reorderedWavAddress - SOUNDBANK_STARTADRESS) * 2 - sampleOffset;
}

uint32_t DuSample::wavAddressReadableToDream(uint32_t readableValue, uint32_t sampleOffset)
{
    uint32_t tmpNum = ((readableValue + sampleOffset) / 2) + SOUNDBANK_STARTADRESS;
    uint32_t reorderedWavAddress = 0;
    reorderedWavAddress |= (0x000000FF & tmpNum) << 0;
    reorderedWavAddress |= (0x0000FF00 & tmpNum) << 8;
    reorderedWavAddress |= (0x00FF0000 & tmpNum) << 8;
    reorderedWavAddress |= (0xFF000000 & tmpNum) >> 16;
    return reorderedWavAddress;
}

uint32_t DuSample::loopStartDreamToReadable(uint16_t loopStartMSB, uint16_t loopStartLSB, uint32_t sampleStartAddress)
{
    uint32_t loopStart = (uint32_t) (loopStartMSB << 16) | (uint32_t) loopStartLSB;
    uint32_t reorderedLoopStart = 0;
    reorderedLoopStart |= ((0x000000FF & loopStart) - 0x00000001) << 24;
    reorderedLoopStart |=  (0x0000FF00 & loopStart) >> 8;
    reorderedLoopStart |=  (0x00FF0000 & loopStart) >> 8;
    reorderedLoopStart |=  (0xFF000000 & loopStart) >> 8;

    return (reorderedLoopStart - SOUNDBANK_STARTADRESS) * 2 - sampleStartAddress;
}

void DuSample::loopStartReadableToDream(uint32_t readableValue, uint32_t sampleStartAddress, uint16_t& outLoopStartMSB, uint16_t& outLoopStartLSB)
{
    uint32_t readableValueOffseted = ((readableValue + sampleStartAddress) / 2) + SOUNDBANK_STARTADRESS;

    uint32_t reorderedLoopStart = 0;
    reorderedLoopStart |=  (0x000000FF & readableValueOffseted) << 0;
    reorderedLoopStart |=  (0x0000FF00 & readableValueOffseted) << 16;
    reorderedLoopStart |=  (0x00FF0000 & readableValueOffseted) << 0;
    reorderedLoopStart |= ((0xFF000000 & readableValueOffseted) + 0x01000000) >> 16;

    uint16_t reorderedLoopStart_MSB = (0xFFFF0000 & reorderedLoopStart) >> 16;
    uint16_t reorderedLoopStart_LSB = (0x0000FFFF & reorderedLoopStart);

    outLoopStartMSB = (uint16_t) ((0x00FF & reorderedLoopStart_MSB) << 8) | (uint16_t) ((0xFF00 & reorderedLoopStart_MSB) >> 8);
    outLoopStartLSB = (uint16_t) ((0x00FF & reorderedLoopStart_LSB) << 8) | (uint16_t) ((0xFF00 & reorderedLoopStart_LSB) >> 8);
}

uint32_t DuSample::loopEndDreamToReadable(uint16_t loopEndMSB, uint16_t loopEndLSB, uint32_t sampleStartAddress)
{
    uint32_t loopEnd = (uint32_t) (loopEndMSB << 16) | (uint32_t) loopEndLSB;
    uint32_t reorderedLoopEnd = 0;
    reorderedLoopEnd |= (0x000000FF & loopEnd) << 8;
    reorderedLoopEnd |= (0x0000FF00 & loopEnd) << 8;
    reorderedLoopEnd |= (0x00FF0000 & loopEnd) >> 16;
    reorderedLoopEnd |= (0xFF000000 & loopEnd) << 0;

    return (reorderedLoopEnd - SOUNDBANK_STARTADRESS) * 2 - sampleStartAddress;
}

void DuSample::loopEndReadableToDream(uint32_t readableValue, uint32_t sampleStartAddress, uint16_t& outLoopEndMSB, uint16_t& outLoopEndLSB)
{
    uint32_t readableValueOffseted = ((readableValue + sampleStartAddress) / 2) + SOUNDBANK_STARTADRESS;

    uint32_t reorderedLoopEnd = 0;
    reorderedLoopEnd |= (0x000000FF & readableValueOffseted) << 24;
    reorderedLoopEnd |= (0x0000FF00 & readableValueOffseted) << 0;
    reorderedLoopEnd |= (0x00FF0000 & readableValueOffseted) >> 16;
    reorderedLoopEnd |= (0xFF000000 & readableValueOffseted) >> 8;

    uint16_t reorderedLoopEnd_MSB = (0xFFFF0000 & reorderedLoopEnd) >> 16;
    uint16_t reorderedLoopEnd_LSB = (0x0000FFFF & reorderedLoopEnd);

    outLoopEndMSB = (uint16_t) ((0x00FF & reorderedLoopEnd_MSB) << 8) | (uint16_t) ((0xFF00 & reorderedLoopEnd_MSB) >> 8);
    outLoopEndLSB = (uint16_t) ((0x00FF & reorderedLoopEnd_LSB) << 8) | (uint16_t) ((0xFF00 & reorderedLoopEnd_LSB) >> 8);
}

int DuSample::volumeDreamToReadable(uint16_t dreamValue)
{
    uint8_t volumeRight = (dreamValue >> 8) & 0xFF;
//    uint8_t volumeLeft  = dreamValue & 0xFF;

    return volumeRight;
}

uint16_t DuSample::volumeReadableToDream(int readableValue)
{
    uint8_t volumeRight = (uint8_t) readableValue;
    uint8_t volumeLeft  = (uint8_t) readableValue + 1; // +1 for panning

    uint16_t dreamValue = (((uint16_t)volumeRight << 8) & 0xFF00) | ((uint16_t)volumeLeft & 0x00FF);

    return dreamValue;
}

// Intrument Parameters
DU_KEY_ACCESSORS_IMPL(DuSample, StartNote,       Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSample, EndNote,         Numeric, int, -1)

// Sample Parameters
DU_KEY_ACCESSORS_IMPL(DuSample, Address1,        Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSample, LoopType,        Numeric, DuSample::SampleType, Unknown)
DU_KEY_ACCESSORS_IMPL(DuSample, Address2,        Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSample, Address3,        Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuSample, VolumeAmplifier, Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuSample, FineTune,        Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSample, UnityNote,       Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuSample, LoopStart,       Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuSample, VolumeMixer1,    Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuSample, LoopEnd,         Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuSample, AmplitudeOscAmp, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSample, VolumeMixer2,    Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuSample, Init,            Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSample, Attack,          Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSample, Decay,           Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSample, Release,         Numeric, int, -1)

// Data
DU_KEY_ACCESSORS_IMPL(DuSample, Data,           BinaryData, QByteArray, QByteArray())
