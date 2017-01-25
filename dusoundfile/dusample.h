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
    virtual ~DuSample() = default;

    virtual DuObjectPtr clone() const override;

    static DuSamplePtr fromBinary(const dream_ip &dreamIP,
                                  const dream_sp &dreamSP,
                                  const QByteArray& data);
    static DuSamplePtr fromWav(QFile *input);

private:
    static QString convertToMono(SndfileHandle& oldSoundFile);
    static QString convertTo44100samplerate(SndfileHandle &oldSoundFile, sf_count_t *outNbFrames);
    static double interpolate(double x0, double x1, double x2, double x3, double t);
    static QString convertTo16bits(SndfileHandle& oldSoundFile);
    static int normalizeWaveType(int format);

public:
    QByteArray ipBinary(quint8 min_vel, quint8 max_vel) const;
    QByteArray spBinary(quint32 sampleAddress, bool forDuTouchSOrL) const;

    static quint32 wavAddressDreamToReadable(quint32 dreamValue, quint32 soundbankStartAddr = SOUNDBANK_STARTADRESS);
    static quint32 wavAddressReadableToDream(quint32 readableValue, quint32 soundbankStartAddr = SOUNDBANK_STARTADRESS);

    static quint32 loopStartDreamToReadable(quint16 loopStartMSB, quint16 loopStartLSB, quint32 sampleStartAddress, quint32 soundbankStartAddr = SOUNDBANK_STARTADRESS);
    static void loopStartReadableToDream(quint32 readableValue, quint32 sampleStartAddress, quint16 &outLoopStartMSB, quint16 &outLoopStartLSB, quint32 soundbankStartAddr = SOUNDBANK_STARTADRESS);

    static quint32 loopEndDreamToReadable(quint16 loopEndMSB, quint16 loopEndLSB, quint32 sampleStartAddress, quint32 soundbankStartAddr = SOUNDBANK_STARTADRESS);
    static void loopEndReadableToDream(quint32 readableValue, quint32 sampleStartAddress, quint16 &outLoopEndMSB, quint16 &outLoopEndLSB, quint32 soundbankStartAddr = SOUNDBANK_STARTADRESS);

    static int initLevelDreamToReadable(quint16 dreamValue);
    static quint16 initReadableToDream(int level);

    static int attackRateDreamToReadable(quint16 dreamValue);
    static int attackLevelDreamToReadable(quint16 dreamValue);
    static quint16 attackReadableToDream(int rate, int level);

    static int decayRateDreamToReadable(quint16 dreamValue);
    static int decayLevelDreamToReadable(quint16 dreamValue);
    static quint16 decayReadableToDream(int rate, int level);

    static int releaseRateDreamToReadable(quint16 dreamValue);
    static int releaseLevelDreamToReadable(quint16 dreamValue);
    static quint16 releaseReadableToDream(int rate, int level);

    static int volumeDreamToReadable(quint16 dreamValue);
    static bool isOneShotDreamToReadable(quint16 dreamValue);
    static quint16 volumeReadableToDream(int volume, bool isOneShot);

    static int sizeWavDreamToReadable(quint32 dreamValue);
    static quint32 sizeWavReadableToDream(int readableValue);

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
