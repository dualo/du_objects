#ifndef DUSAMPLE_H
#define DUSAMPLE_H

#include "../general/ducontainer.h"

#include <QFile>

#include <sndfile.hh>

// TODO: Get this from external
#ifndef SOUNDBANK_STARTADRESS
#define SOUNDBANK_STARTADRESS 0x10000
#endif

DU_OBJECT(DuBinaryData);

DU_OBJECT(DuSample);

class DuSample : public DuContainer
{
public:
    enum SampleType
    {
        Unknown = 0x000,
        SND5000_OneShot = 0x836C,
        SND5000_Forward = 0x846C, // Incorrect value
        SND3000_Forward = 0xC56C
    };

    DuSample();

    virtual DuObjectPtr clone() const override;

    static DuSamplePtr fromBinary(const dream_ip &dreamIP,
                                  const dream_sp &dreamSP,
                                  const QByteArray& data);
    static DuSamplePtr fromWav(QFile *input);

    static QString convertToMono(SndfileHandle& oldSoundFile);
    static QString convertTo16bits(SndfileHandle& oldSoundFile);
    static int normalizeWaveType(int format);

    QByteArray ipBinary(uint8_t min_vel, uint8_t max_vel) const;
    QByteArray spBinary(uint32_t sampleAddress) const;

    static uint32_t wavAddressDreamToReadable(uint32_t dreamValue);
    static uint32_t wavAddressReadableToDream(uint32_t readableValue);

    static uint32_t loopStartDreamToReadable(uint16_t loopStartMSB, uint16_t loopStartLSB, uint32_t sampleStartAddress);
    static void loopStartReadableToDream(uint32_t readableValue, uint32_t sampleStartAddress, uint16_t& outLoopStartMSB, uint16_t& outLoopStartLSB);

    static uint32_t loopEndDreamToReadable(uint16_t loopEndMSB, uint16_t loopEndLSB, uint32_t sampleStartAddress);
    static void loopEndReadableToDream(uint32_t readableValue, uint32_t sampleStartAddress, uint16_t& outLoopEndMSB, uint16_t& outLoopEndLSB);

    static int initLevelDreamToReadable(uint16_t dreamValue);
    static uint16_t initReadableToDream(int level);

    static int attackRateDreamToReadable(uint16_t dreamValue);
    static int attackLevelDreamToReadable(uint16_t dreamValue);
    static uint16_t attackReadableToDream(int rate, int level);

    static int decayRateDreamToReadable(uint16_t dreamValue);
    static int decayLevelDreamToReadable(uint16_t dreamValue);
    static uint16_t decayReadableToDream(int rate, int level);

    static int releaseRateDreamToReadable(uint16_t dreamValue);
    static int releaseLevelDreamToReadable(uint16_t dreamValue);
    static uint16_t releaseReadableToDream(int rate, int level);

    static int volumeDreamToReadable(uint16_t dreamValue);
    static bool isOneShotDreamToReadable(uint16_t dreamValue);
    static uint16_t volumeReadableToDream(int volume, bool isOneShot);

    static int sizeWavDreamToReadable(uint32_t dreamValue);
    static uint32_t sizeWavReadableToDream(int readableValue);

    // Intrument Parameters
    DU_KEY_ACCESSORS(StartNote,       int)
    DU_KEY_ACCESSORS(EndNote,         int)

    // Sample Parameters
    DU_KEY_ACCESSORS(Address1,        int)
    DU_KEY_ACCESSORS(LoopType,        SampleType)
    DU_KEY_ACCESSORS(Address2,        int)
    DU_KEY_ACCESSORS(Address3,        int)

    DU_KEY_ACCESSORS(VolumeAmplifier, int)

    DU_KEY_ACCESSORS(PitchShifted,    bool)

    DU_KEY_ACCESSORS(FineTune,        int)
    DU_KEY_ACCESSORS(UnityNote,       int)

    DU_KEY_ACCESSORS(LoopStart,       int)

    DU_KEY_ACCESSORS(VolumeMixer1,    int)
    DU_KEY_ACCESSORS(IsOneShot,       bool)

    DU_KEY_ACCESSORS(LoopEnd,         int)

    DU_KEY_ACCESSORS(AmplitudeOscAmp, int)
    DU_KEY_ACCESSORS(VolumeMixer2,    int)

    DU_KEY_ACCESSORS(InitLevel,       int)
    DU_KEY_ACCESSORS(AttackRate,      int)
    DU_KEY_ACCESSORS(AttackLevel,     int)
    DU_KEY_ACCESSORS(DecayRate,       int)
    DU_KEY_ACCESSORS(DecayLevel,      int)
    DU_KEY_ACCESSORS(ReleaseRate,     int)
    DU_KEY_ACCESSORS(ReleaseLevel,    int)

    // Data
    DU_KEY_ACCESSORS(Data,            QByteArray)
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

#endif // DUSAMPLE_H
