#include "dudreamsampleparam.h"

#include "../general/dunumeric.h"

/*

                                      @
                                      &
            @@&&&&@                  @&
           @&&&&&&&&&@             @&&
           &&&&&&&&&&&&          @&&&
           &&&&&&&&&& &&@      @&&&&
             &&&&&&     &@   @&&&&&
                         &&&&&&&
                       @&&&&&&&
                    @&&&&&&&
                  @&&&&&&&&&@
               @&&&&&&&&  &&@
              @&&&&&&&   @&&@
             @&&&&&&     &&&
            @&&&&&      &&&&
           @&&&&       &&&&
           &&&&      @&&&&&
          @&&&     @&&&&&&
          @&&    @&&&&&&
          @&@ @&&&&&&&&
          @&@&&&&&&&&
          @&&&&&&&&
        @&&&&&&&
      @&&&&&&&
    @&&&&&   &@     @@@@@@
   @&&&       &&@@@&&&&&&&&
 @&&&&         &&&&&&&&&&&&@
@&&              &&&&&&&&&&&
&&                  &&&&&&
@

*/
DuDreamSampleParam::DuDreamSampleParam() :
    DuContainer()
{
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
}

DuObjectPtr DuDreamSampleParam::clone() const
{
    return DuDreamSampleParamPtr(new DuDreamSampleParam(*this));
}

int DuDreamSampleParam::size() const
{
    return INSTR_DREAM_SP_SIZE;
}

DuDreamSampleParamPtr DuDreamSampleParam::fromBinary(const dream_sp &data)
{
    DuDreamSampleParamPtr dreamSP(new DuDreamSampleParam);
    bool verif = true;

    verif = dreamSP->setAddress1(data.address1) ? verif : false;
    verif = dreamSP->setLoopType((SampleType)data.loopType) ? verif : false;
    verif = dreamSP->setAddress2(data.address2) ? verif : false;
    verif = dreamSP->setAddress3(data.address3) ? verif : false;

    verif = dreamSP->setVolumeAmplifier(data.volume_amplifier) ? verif : false;

    int convertedFineTune = -1;
    for (int i = 0; i < 100; ++i)
    {
        if (data.fine_tune == finetune_convert[i])
        {
            convertedFineTune = i;
            break;
        }
    }
    if (convertedFineTune == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate dream sample param:\n"
                                      << "Fine tune not found in array:" << data.fine_tune;
        return DuDreamSampleParamPtr();
    }
    verif = dreamSP->setFineTune(convertedFineTune) ? verif : false;
//    verif = dreamSP->setUnityNote(data.unity_note) ? verif : false;

//    int convertedCoarseTune = 0;
//    if (data.unity_note < 65)
//    {
//        convertedCoarseTune = data.coarse_tune + data.unity_note - 64;
//    }
//    else
//    {
//        convertedCoarseTune = data.coarse_tune + data.unity_note - 320;
//    }
//    while (convertedCoarseTune < 0)
//    {
//        convertedCoarseTune += 256;
//    }
    verif = dreamSP->setCoarseTune(data.coarse_tune) ? verif : false;

    uint32_t loop_start = (data.loop_start_MSB << 16) | data.loop_start_LSB;
    uint32_t reorderedLoopStart = 0;
    reorderedLoopStart |= ((0x000000FF & loop_start) - 0x00000001) << 24;
    reorderedLoopStart |= (0x0000FF00 & loop_start) >> 8;
    reorderedLoopStart |= (0x00FF0000 & loop_start) >> 8;
    reorderedLoopStart |= (0xFF000000 & loop_start) >> 8;
    verif = dreamSP->setLoopStart(reorderedLoopStart) ? verif : false;

    uint32_t reorderedWavAddress = 0;
    reorderedWavAddress |= (0x000000FF & data.wav_address) << 0;
    reorderedWavAddress |= (0x0000FF00 & data.wav_address) << 16;
    reorderedWavAddress |= (0x00FF0000 & data.wav_address) >> 8;
    reorderedWavAddress |= (0xFF000000 & data.wav_address) >> 8;
    verif = dreamSP->setWavAddress(reorderedWavAddress * 2) ? verif : false;

    verif = dreamSP->setVolumeMixer1(data.volume_mixer1) ? verif : false;

    uint32_t loop_end = (data.loop_end_MSB << 16) | data.loop_end_LSB;
    uint32_t reorderedLoopEnd = 0;
    reorderedLoopEnd |= (0x000000FF & loop_end) << 8;
    reorderedLoopEnd |= (0x0000FF00 & loop_end) << 8;
    reorderedLoopEnd |= (0x00FF0000 & loop_end) >> 16;
    reorderedLoopEnd |= (0xFF000000 & loop_end) << 0;
    verif = dreamSP->setLoopEnd(reorderedLoopEnd) ? verif : false;

    verif = dreamSP->setAmplitudeOscAmp(data.amplitude_osc_amp) ? verif : false;
    verif = dreamSP->setVolumeMixer2(data.volume_mixer2) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "an attribute was not properly set";
    }

    return dreamSP;
}

QByteArray DuDreamSampleParam::toDuMusicBinary() const
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

    tmpNum = getWavAddress() / 2;
    if (tmpNum == -1)
        return QByteArray();
    uint32_t reorderedWavAddress = 0;
    reorderedWavAddress |= (0x000000FF & tmpNum) << 0;
    reorderedWavAddress |= (0x0000FF00 & tmpNum) << 8;
    reorderedWavAddress |= (0x00FF0000 & tmpNum) << 8;
    reorderedWavAddress |= (0xFF000000 & tmpNum) >> 16;
    data.wav_address = reorderedWavAddress;

    tmpNum = getVolumeMixer1();
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

    return QByteArray((char*)&data, size());
}

DU_KEY_ACCESSORS_IMPL(DuDreamSampleParam, Address1,        Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuDreamSampleParam, LoopType,        Numeric, DuDreamSampleParam::SampleType, Unknown)
DU_KEY_ACCESSORS_IMPL(DuDreamSampleParam, Address2,        Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuDreamSampleParam, Address3,        Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuDreamSampleParam, VolumeAmplifier, Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuDreamSampleParam, FineTune,        Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuDreamSampleParam, UnityNote,       Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuDreamSampleParam, CoarseTune,      Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuDreamSampleParam, LoopStart,       Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuDreamSampleParam, WavAddress,      Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuDreamSampleParam, VolumeMixer1,    Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuDreamSampleParam, LoopEnd,         Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuDreamSampleParam, AmplitudeOscAmp, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuDreamSampleParam, VolumeMixer2,    Numeric, int, -1)
