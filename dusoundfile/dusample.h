#ifndef DUSAMPLE_H
#define DUSAMPLE_H

#include "../general/ducontainer.h"

#include <QFile>

#include <sndfile.hh>

#define MAX_REF_NOTE_END_NOTE_INTERVAL 43

// TODO: Get this from external
#ifndef SOUNDBANK_STARTADRESS
#define SOUNDBANK_STARTADRESS 0x10000
#endif

DU_OBJECT(DuBinaryData);

DU_OBJECT(DuSample);

class DuSample : public DuContainer
{
    Q_GADGET

public:
    enum SampleType
    {
        Unknown = 0x000,
        SND5000_OneShot = 0x836C,
        SND5000_Forward = 0x846C, // Incorrect value
        SND3000_Forward = 0xC56C
    };
    Q_ENUM(SampleType)

    enum WavConvertionResult
    {
        StereoToMonoConvertion  = 0x0001,
        SampleRateConvertion    = 0x0002,
        SampleSizeConvertion    = 0x0004,
        TruncateConvertion      = 0x0008
    };
    Q_ENUM(WavConvertionResult)
    Q_DECLARE_FLAGS(WavConvertionResults, WavConvertionResult)
    Q_FLAG(WavConvertionResults)

    DuSample();
    virtual ~DuSample() = default;

    virtual DuObjectPtr clone() const override;

    static DuSamplePtr fromBinary(const dream_ip &dreamIP,
                                  const dream_sp &dreamSP,
                                  const QByteArray& data);
    static DuSamplePtr fromWav(QFile *input, WavConvertionResults& outResults);

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

    static int sizeWavDreamToReadable(quint32 dreamValue);
    static quint32 sizeWavReadableToDream(int readableValue);

    QString toWav(const QString& outputFilePath) const;

    // Intrument Parameters
    DU_KEY_ACCESSORS(StartNote,       int)
    DU_KEY_ACCESSORS(EndNote,         int)

    // Sample Parameters
    DU_KEY_ACCESSORS(LoopType,        SampleType)

    DU_KEY_ACCESSORS(VolumeAmplifier, int)

    DU_KEY_ACCESSORS(FineTune,        int)
    DU_KEY_ACCESSORS(UnityNote,       int)

    DU_KEY_ACCESSORS(LoopStart,       int)
    DU_KEY_ACCESSORS(LoopEnd,         int)

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

Q_DECLARE_OPERATORS_FOR_FLAGS(DuSample::WavConvertionResults)

#endif // DUSAMPLE_H
