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

private:
    static QString convertToMono(SndfileHandle& oldSoundFile);
    static QString convertTo44100samplerate(SndfileHandle &oldSoundFile, int *outWaveSize);
    static double interpolate(double x0, double x1, double x2, double x3, double t);
    static QString convertTo16bits(SndfileHandle& oldSoundFile);
    static int normalizeWaveType(int format);

public:
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

private:
    static const quint8 finetune_convert[101];
};

#endif // DUSAMPLE_H
