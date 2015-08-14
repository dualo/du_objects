#ifndef DUDREAMSAMPLEPARAM_H
#define DUDREAMSAMPLEPARAM_H

#include "../general/ducontainer.h"


#pragma pack(push, 4)
struct dream_sp
{
    uint16_t align1 = 0x0000;
    uint16_t address1;
    uint16_t loopType;
    uint16_t address2;
    uint16_t address3;

    uint16_t unknown1 = 0xB5F0;
    uint16_t unknown2 = 0x0814;
    uint16_t unknown3 = 0xFF00;
    uint16_t unknown4 = 0x1032;
    uint8_t unknown5 = 0x04;

    uint8_t volume_amplifier;

    uint16_t unknown6 = 0x0081;

    uint8_t fine_tune;
//    uint8_t unity_note;
    uint8_t coarse_tune;

    uint16_t align2 = 0x0000;

    uint16_t loop_start_MSB;
    uint16_t loop_start_LSB;

    uint16_t align3 = 0x0000;

    uint32_t wav_address;

    uint16_t volume_mixer1;

    uint16_t loop_end_MSB;
    uint16_t loop_end_LSB;

    uint16_t unknown7 = 0x7F0E;
    uint8_t unknown8 = 0x06;

    uint8_t amplitude_osc_amp;
    uint16_t volume_mixer2;

    uint16_t unknown9 = 0x7F00;
    uint16_t unknown10 = 0x1FE3;
    uint16_t unknown11 = 0x5FE3;
    uint16_t unknown12 = 0x605A;
    uint16_t unknown13 = 0x0401;
    uint16_t unknown14 = 0x0F00;
    uint16_t unknown15 = 0x0015;
    uint16_t unknown16 = 0x0403;
    uint16_t unknown17 = 0x0010;
    uint16_t unknown18 = 0x0201;

    uint32_t unknown19 = 0x00000000;
    uint32_t unknown20 = 0x00000000;
    uint16_t unknown21 = 0x0000;

    uint16_t unknown22 = 0x0002;
    uint32_t unknown23 = 0x00000000;
};

#define INSTR_DREAM_SP_SIZE 84
#pragma pack(pop)


DU_OBJECT(DuDreamSampleParam)

class DuDreamSampleParam : public DuContainer
{
public:
    enum SampleType
    {
        Unknown = 0x000,
        SND5000_OneShot = 0x836C,
        SND5000_Forward = 0x846C, // Incorrect value
        SND3000_Forward = 0xC56C
    };

    DuDreamSampleParam();

    virtual DuObjectPtr clone() const;
    virtual int size() const;
    virtual QByteArray toDuMusicBinary() const;

    static DuDreamSampleParamPtr fromBinary(const dream_sp& data);

    DU_KEY_ACCESSORS(Address1,        int)
    DU_KEY_ACCESSORS(LoopType,        SampleType)
    DU_KEY_ACCESSORS(Address2,        int)
    DU_KEY_ACCESSORS(Address3,        int)

    DU_KEY_ACCESSORS(VolumeAmplifier, int)

    DU_KEY_ACCESSORS(FineTune,        int)
    DU_KEY_ACCESSORS(UnityNote,       int)
    DU_KEY_ACCESSORS(CoarseTune,      int)

    DU_KEY_ACCESSORS(LoopStart,       int)

    DU_KEY_ACCESSORS(WavAddress,      int)

    DU_KEY_ACCESSORS(VolumeMixer1,    int)

    DU_KEY_ACCESSORS(LoopEnd,         int)

    DU_KEY_ACCESSORS(AmplitudeOscAmp, int)
    DU_KEY_ACCESSORS(VolumeMixer2,    int)
};

static quint8 finetune_convert[101] = {
    0xc2,
    0xc4,
    0xc6,
    0xc8,
    0xcc,
    0xce,
    0xd0,
    0xd2,
    0xd6,
    0xd8,
    0xda,
    0xdc,
    0xe0,
    0xe2,
    0xe4,
    0xe8,
    0xea,
    0xec,
    0xee,
    0xf2,
    0xf4,
    0xf6,
    0xf8,
    0xfc,
    0x0,
    0x2,
    0x6,
    0x8,
    0xa,
    0xc,
    0x10,
    0x12,
    0x14,
    0x16,
    0x1a,
    0x1c,
    0x1e,
    0x22,
    0x24,
    0x26,
    0x28,
    0x2c,
    0x2e,
    0x30,
    0x32,
    0x36,
    0x38,
    0x3a,
    0x3c,
    0x40,
    0x42,
    0x44,
    0x48,
    0x4a,
    0x4c,
    0x4e,
    0x52,
    0x54,
    0x56,
    0x58,
    0x5c,
    0x5e,
    0x60,
    0x64,
    0x66,
    0x68,
    0x6a,
    0x6e,
    0x70,
    0x72,
    0x74,
    0x78,
    0x7a,
    0x7c,
    0x80,
    0x82,
    0x84,
    0x86,
    0x8a,
    0x8c,
    0x8e,
    0x90,
    0x94,
    0x96,
    0x98,
    0x9a,
    0x9e,
    0xa0,
    0xa2,
    0xa6,
    0xa8,
    0xaa,
    0xac,
    0xb0,
    0xb2,
    0xb4,
    0xb6,
    0xba,
    0xbc,
    0xbe,
    0xc2
};

#endif // DUDREAMSAMPLEPARAM_H
