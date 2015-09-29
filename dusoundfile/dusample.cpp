#include "dusample.h"

#include <du_objects/general/dubinarydata.h>
#include <du_objects/general/dunumeric.h>

DU_OBJECT_IMPL(DuSample)

DuSample::DuSample() :
    DuContainer()
{
    // Intrument Parameters
    addChild(KeyStartNote,       new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0x7F, 0x00));
    addChild(KeyEndNote,         new DuNumeric(0x7F, NUMERIC_DEFAULT_SIZE, 0x7F, 0x00));

    addChild(KeySampleParamAddr, new DuNumeric(0, 2, 0xFFFF, 0x0000));

    // Sample Parameters
    addChild(KeyAddress1,        new DuNumeric(0x0000, 2, 0xFFFF, 0x0000));
    addChild(KeyLoopType,        new DuNumeric(SND3000_Forward, 2, 0xFFFF, 0x0000));
    addChild(KeyAddress2,        new DuNumeric(0x0000, 2, 0xFFFF, 0x0000));
    addChild(KeyAddress3,        new DuNumeric(0x0000, 2, 0xFFFF, 0x0000));

    addChild(KeyVolumeAmplifier, new DuNumeric(0xFF, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));

    addChild(KeyFineTune,        new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));
    addChild(KeyUnityNote,       new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));
    addChild(KeyCoarseTune,      new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));

    addChild(KeyLoopStart,       new DuNumeric(0));

    addChild(KeyWavAddress,      new DuNumeric(0));

    addChild(KeyVolumeMixer1,    new DuNumeric(0xEEEF, 2, 0xFFFF, 0x0000));

    addChild(KeyLoopEnd,         new DuNumeric(0));

    addChild(KeyAmplitudeOscAmp, new DuNumeric(0x46, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));
    addChild(KeyVolumeMixer2,    new DuNumeric(0xEFEF, 2, 0xFFFF, 0x0000));

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

    verif = sample->setSampleParamAddr(dreamIP.dream_sp_addr) ? verif : false;

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
//    verif = sample->setUnityNote(dreamSP.unity_note) ? verif : false;

//    int convertedCoarseTune = 0;
//    if (dreamSP.unity_note < 65)
//    {
//        convertedCoarseTune = dreamSP.coarse_tune + dreamSP.unity_note - 64;
//    }
//    else
//    {
//        convertedCoarseTune = dreamSP.coarse_tune + dreamSP.unity_note - 320;
//    }
//    while (convertedCoarseTune < 0)
//    {
//        convertedCoarseTune += 256;
//    }
    verif = sample->setCoarseTune(dreamSP.coarse_tune) ? verif : false;

    uint32_t loop_start = (dreamSP.loop_start_MSB << 16) | dreamSP.loop_start_LSB;
    uint32_t reorderedLoopStart = 0;
    reorderedLoopStart |= ((0x000000FF & loop_start) - 0x00000001) << 24;
    reorderedLoopStart |= (0x0000FF00 & loop_start) >> 8;
    reorderedLoopStart |= (0x00FF0000 & loop_start) >> 8;
    reorderedLoopStart |= (0xFF000000 & loop_start) >> 8;
    verif = sample->setLoopStart(reorderedLoopStart) ? verif : false;

    verif = sample->setWavAddress(wavAddressDreamToReadable(dreamSP.wav_address, sampleOffset)) ? verif : false;

    verif = sample->setVolumeMixer1(volumeDreamToReadable(dreamSP.volume_mixer1)) ? verif : false;

    uint32_t loop_end = (dreamSP.loop_end_MSB << 16) | dreamSP.loop_end_LSB;
    uint32_t reorderedLoopEnd = 0;
    reorderedLoopEnd |= (0x000000FF & loop_end) << 8;
    reorderedLoopEnd |= (0x0000FF00 & loop_end) << 8;
    reorderedLoopEnd |= (0x00FF0000 & loop_end) >> 16;
    reorderedLoopEnd |= (0xFF000000 & loop_end) << 0;
    verif = sample->setLoopEnd(reorderedLoopEnd) ? verif : false;

    verif = sample->setAmplitudeOscAmp(dreamSP.amplitude_osc_amp) ? verif : false;
    verif = sample->setVolumeMixer2(dreamSP.volume_mixer2) ? verif : false;

    // Data
    verif = sample->setData(data) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "an attribute was not properly set";
    }

    return sample;
}

QByteArray DuSample::ipBinary(uint8_t min_vel, uint8_t max_vel) const
{
    dream_ip dataStruct;

    int tmpNum = 0;

    dataStruct.min_vel = min_vel;

    tmpNum = getStartNote() - 1;
    if (tmpNum == -1)
        return QByteArray();
    dataStruct.start_note = tmpNum;

    dataStruct.max_vel = max_vel;

    tmpNum = getEndNote();
    if (tmpNum == -1)
        return QByteArray();
    dataStruct.end_note = tmpNum;

    tmpNum = getSampleParamAddr();
    if (tmpNum == -1)
        return QByteArray();
    dataStruct.dream_sp_addr = tmpNum;

    return QByteArray((char*)&dataStruct, INSTR_DREAM_IP_SIZE);
}

QByteArray DuSample::spBinary(uint32_t sampleOffset) const
{
    dream_sp data;

    int tmpNum = 0;

    tmpNum = getAddress1();
    if (tmpNum == -1)
        return QByteArray();
    data.address1 = tmpNum;

    SampleType tmpSampleType = getLoopType();
    if (tmpSampleType == Unknown)
        return QByteArray();
    data.loopType = (uint16_t)tmpSampleType;

    tmpNum = getAddress2();
    if (tmpNum == -1)
        return QByteArray();
    data.address2 = tmpNum;

    tmpNum = getAddress3();
    if (tmpNum == -1)
        return QByteArray();
    data.address3 = tmpNum;

    tmpNum = getVolumeAmplifier();
    if (tmpNum == -1)
        return QByteArray();
    data.volume_amplifier = tmpNum;

    tmpNum = getFineTune();
    if (tmpNum == -1 || tmpNum >= 100)
        return QByteArray();
    data.fine_tune = finetune_convert[tmpNum];

    tmpNum = getCoarseTune();
    if (tmpNum == -1)
        return QByteArray();
    data.coarse_tune = tmpNum;

    tmpNum = getLoopStart();
    if (tmpNum == -1)
        return QByteArray();
    uint32_t reorderedLoopStart = 0;
    reorderedLoopStart |= (0x000000FF & tmpNum) << 0;
    reorderedLoopStart |= (0x0000FF00 & tmpNum) << 16;
    reorderedLoopStart |= (0x00FF0000 & tmpNum) << 0;
    reorderedLoopStart |= ((0xFF000000 & tmpNum) + 0x01000000) >> 16;
    uint16_t reorderedLoopStart_MSB = (0xFFFF0000 & reorderedLoopStart) >> 16;
    uint16_t reorderedLoopStart_LSB = (0x0000FFFF & reorderedLoopStart);
    data.loop_start_MSB = ((0x00FF & reorderedLoopStart_MSB) << 8) | ((0xFF00 & reorderedLoopStart_MSB) >> 8);
    data.loop_start_LSB = ((0x00FF & reorderedLoopStart_LSB) << 8) | ((0xFF00 & reorderedLoopStart_LSB) >> 8);

    data.wav_address = wavAddressReadableToDream(getWavAddress(), sampleOffset);

    tmpNum = volumeReadableToDream(getVolumeMixer1());
    if (tmpNum == -1)
        return QByteArray();
    data.volume_mixer1 = tmpNum;

    tmpNum = getLoopEnd();
    if (tmpNum == -1)
        return QByteArray();
    uint32_t reorderedLoopEnd = 0;
    reorderedLoopEnd |= (0x000000FF & tmpNum) << 24;
    reorderedLoopEnd |= (0x0000FF00 & tmpNum) << 0;
    reorderedLoopEnd |= (0x00FF0000 & tmpNum) >> 16;
    reorderedLoopEnd |= (0xFF000000 & tmpNum) >> 8;
    uint16_t reorderedLoopEnd_MSB = (0xFFFF0000 & reorderedLoopEnd) >> 16;
    uint16_t reorderedLoopEnd_LSB = (0x0000FFFF & reorderedLoopEnd);
    data.loop_end_MSB = ((0x00FF & reorderedLoopEnd_MSB) << 8) | ((0xFF00 & reorderedLoopEnd_MSB) >> 8);
    data.loop_end_LSB = ((0x00FF & reorderedLoopEnd_LSB) << 8) | ((0xFF00 & reorderedLoopEnd_LSB) >> 8);

    tmpNum = getAmplitudeOscAmp();
    if (tmpNum == -1)
        return QByteArray();
    data.amplitude_osc_amp = tmpNum;

    tmpNum = getVolumeMixer2();
    if (tmpNum == -1)
        return QByteArray();
    data.volume_mixer2 = tmpNum;

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

int DuSample::volumeDreamToReadable(uint16_t dreamValue)
{
    uint8_t volumeRight = (dreamValue >> 8) & 0xFF;
//    uint8_t volumeLeft  = dreamValue & 0xFF;

    return volumeRight;
}

uint16_t DuSample::volumeReadableToDream(int readableValue)
{
    uint8_t volumeRight = readableValue;
    uint8_t volumeLeft  = readableValue + 1; // +1 for panning

    uint16_t dreamValue = (((uint16_t)volumeRight << 8) & 0xFF00) | ((uint16_t)volumeLeft & 0x00FF);

    return dreamValue;
}

// Intrument Parameters
DU_KEY_ACCESSORS_IMPL(DuSample, StartNote,       Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSample, EndNote,         Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuSample, SampleParamAddr, Numeric, int, -1)

// Sample Parameters
DU_KEY_ACCESSORS_IMPL(DuSample, Address1,        Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSample, LoopType,        Numeric, DuSample::SampleType, Unknown)
DU_KEY_ACCESSORS_IMPL(DuSample, Address2,        Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSample, Address3,        Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuSample, VolumeAmplifier, Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuSample, FineTune,        Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSample, UnityNote,       Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSample, CoarseTune,      Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuSample, LoopStart,       Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuSample, WavAddress,      Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuSample, VolumeMixer1,    Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuSample, LoopEnd,         Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuSample, AmplitudeOscAmp, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSample, VolumeMixer2,    Numeric, int, -1)

// Data
DU_KEY_ACCESSORS_IMPL(DuSample, Data,           BinaryData, QByteArray, QByteArray())
