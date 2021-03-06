#include "dusample.h"

#include "../general/dubinarydata.h"
#include "../general/duboolean.h"
#include "../general/dunumeric.h"

#include <QFile>
#include <QStandardPaths>
#include <QtEndian>
#include <QtMath>

#include <cstring>

DU_OBJECT_IMPL(DuSample)

DuSample::DuSample() :
    DuContainer()
{
    // Intrument Parameters
    addChild(KeyStartNote,       new DuNumeric(0x01, NUMERIC_DEFAULT_SIZE, 0x7F, 0x01));
    addChild(KeyEndNote,         new DuNumeric(0x7F, NUMERIC_DEFAULT_SIZE, 0x7F, 0x01));

    // Sample Parameters
    addChild(KeyLoopType,        new DuNumeric(SND3000_Forward, 2, 0xFFFF, 0x0000));

    addChild(KeyVolumeAmplifier, new DuNumeric(0xFF, NUMERIC_DEFAULT_SIZE, 0xFF, 0x00));

    addChild(KeyFineTune,        new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 100, 0x00));
    addChild(KeyUnityNote,       new DuNumeric(0x01, NUMERIC_DEFAULT_SIZE, 0x7F, 0x01));

    addChild(KeyLoopStart,       new DuNumeric(0));
    addChild(KeyLoopEnd,         new DuNumeric(0));

    addChild(KeyInitLevel,       new DuNumeric(127, NUMERIC_DEFAULT_SIZE, 127, 0));
    addChild(KeyAttackRate,      new DuNumeric(99,  NUMERIC_DEFAULT_SIZE, 127, 0));
    addChild(KeyAttackLevel,     new DuNumeric(63,  NUMERIC_DEFAULT_SIZE, 63,  0));
    addChild(KeyDecayRate,       new DuNumeric(99,  NUMERIC_DEFAULT_SIZE, 99,  0));
    addChild(KeyDecayLevel,      new DuNumeric(63,  NUMERIC_DEFAULT_SIZE, 63,  0));
    addChild(KeyReleaseRate,     new DuNumeric(90,  NUMERIC_DEFAULT_SIZE, 99,  0));
    addChild(KeyReleaseLevel,    new DuNumeric(0,   NUMERIC_DEFAULT_SIZE, 63,  0));

    // Data
    addChild(KeyData,            new DuBinaryData);
}

DuObjectPtr DuSample::clone() const
{
    return DuSamplePtr(new DuSample(*this));
}

DuSamplePtr DuSample::fromBinary(const dream_ip& dreamIP,
                                 const dream_sp& dreamSP,
                                 const QByteArray& data)
{
    DuSamplePtr sample(new DuSample);
    bool verif = true;

    // Sample Parameters
    verif = sample->setLoopType(static_cast<SampleType>(dreamSP.loopType)) ? verif : false;

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
    if (/*dreamSP.coarse_tune >= 0 &&*/ dreamSP.coarse_tune <= 64)
    {
        unityNote = 64 - dreamSP.coarse_tune;
    }
    else if (dreamSP.coarse_tune >= 193 /*&& dreamSP.coarse_tune <= 255*/)
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

    quint32 sampleStartAddress = wavAddressDreamToReadable(dreamSP.wav_address);

    verif = sample->setLoopStart(static_cast<int>(loopStartDreamToReadable(dreamSP.loop_start_MSB, dreamSP.loop_start_LSB, sampleStartAddress))) ? verif : false;
    verif = sample->setLoopEnd(static_cast<int>(loopEndDreamToReadable(dreamSP.loop_end_MSB, dreamSP.loop_end_LSB, sampleStartAddress))) ? verif : false;

    verif = sample->setInitLevel(initLevelDreamToReadable(dreamSP.init))            ? verif : false;
    verif = sample->setAttackRate(attackRateDreamToReadable(dreamSP.attack))        ? verif : false;
    verif = sample->setAttackLevel(attackLevelDreamToReadable(dreamSP.attack))      ? verif : false;
    verif = sample->setDecayRate(decayRateDreamToReadable(dreamSP.decay))           ? verif : false;
    verif = sample->setDecayLevel(decayLevelDreamToReadable(dreamSP.decay))         ? verif : false;
    verif = sample->setReleaseRate(releaseRateDreamToReadable(dreamSP.release))     ? verif : false;
    verif = sample->setReleaseLevel(releaseLevelDreamToReadable(dreamSP.release))   ? verif : false;

    // Intrument Parameters
    verif = sample->setStartNote(dreamIP.start_note + 1) ? verif : false;

    int endNote = dreamIP.end_note;
    if (endNote - unityNote > MAX_REF_NOTE_END_NOTE_INTERVAL)
        endNote = unityNote + MAX_REF_NOTE_END_NOTE_INTERVAL;
    verif = sample->setEndNote(endNote) ? verif : false;

    // Data
    verif = sample->setData(data) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "an attribute was not properly set";
    }

    return sample;
}

DuSamplePtr DuSample::fromWav(QFile *input, WavConvertionResults& outResults)
{
    outResults = WavConvertionResults();

    // Copy file to temp folder
    QString wavFileName = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/tmp.wav";
    if (wavFileName.isEmpty())
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to generate DuSample\n"
                << "can't find temp folder";

        return DuSamplePtr();
    }

    int tmpCpt = 0;
    while (QFile::exists(wavFileName) && tmpCpt < 10)
    {
        if (!QFile::remove(wavFileName))
        {
            wavFileName = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/tmp" + QString::number(tmpCpt++) + ".wav";
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
    if (soundFile.rawHandle() == Q_NULLPTR)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to open the file" << wavFileName << ":\n"
                                      << soundFile.strError();
        return DuSamplePtr();
    }

    sf_count_t nbFrames = soundFile.frames() * soundFile.channels();

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
        if (soundFile.rawHandle() == Q_NULLPTR)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to open the file" << wavFileName << ":\n"
                                          << soundFile.strError();
            return DuSamplePtr();
        }

        outResults |= StereoToMonoConvertion;

        nbFrames /= 2;
    }

    // Convert to 44100 samplerate
    if (soundFile.samplerate() != 44100 && soundFile.channels() == 1)
    {
        qCDebug(LOG_CAT_DU_OBJECT) << "Wrong sample rate => " << soundFile.samplerate();

        sf_count_t newNbFrames = 0;
        wavFileName = convertTo44100samplerate(soundFile, &newNbFrames);
        if (wavFileName.isEmpty())
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to convert to 44100 samplerate";
            return DuSamplePtr();
        }

        soundFile = SndfileHandle(wavFileName.toStdString(), SFM_READ, 0);
        if (soundFile.rawHandle() == Q_NULLPTR)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to open the file" << wavFileName << ":\n"
                                          << soundFile.strError();
            return DuSamplePtr();
        }

        outResults |= SampleRateConvertion;

        nbFrames = newNbFrames;
    }

    qint64 wavSize = 0;

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
        if (soundFile.rawHandle() == Q_NULLPTR)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to open the file" << wavFileName << ":\n"
                                          << soundFile.strError();
            return DuSamplePtr();
        }

        outResults |= SampleSizeConvertion;

        wavSize = 2 * nbFrames; // 16 bits
    }
    else
    {
        wavSize = (soundFile.format() & SF_FORMAT_SUBMASK) * nbFrames;
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

    quint32 sizeInFile = qFromLittleEndian<quint32>(reinterpret_cast<const uchar*>(wavFileData.mid(dataKeywordIndex + 4, 4).constData()));
    if (sizeInFile != wavSize)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "Size in file is different from calculated size:\n"
                                     << "size in file:" << sizeInFile << "o\n"
                                     << "calculated:" << wavSize << "o";
    }

    QByteArray rawData = wavFileData.mid(dataKeywordIndex + 4 + 4, static_cast<int>(wavSize)); // +4 for "data", +4 for size
    if (rawData.size() > MAX_SAMPLE_SIZE - 64)
    {
        outResults |= TruncateConvertion;

        rawData.truncate(MAX_SAMPLE_SIZE - 64); // Truncate sample to the maximum length accepted by DREAM
    }
    rawData.append(QByteArray(64, 0x00)); // Append 32 half words to loop over at the end of the sample

    DuSamplePtr sample(new DuSample);

    sample->setData(rawData);
    sample->setLoopStart(rawData.size() - 64);
    sample->setLoopEnd(rawData.size() - 2);

    return sample;
}

QString DuSample::toWav(const QString& outputFilePath) const
{
    // Fill default parameters
    SF_INFO sfinfo ;
    sfinfo.channels = 1;
    sfinfo.samplerate = 44100;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    // Open file
    SndfileHandle outputSoundFile(outputFilePath.toStdString(), SFM_WRITE, sfinfo.format, sfinfo.channels, sfinfo.samplerate);
    if (outputSoundFile.rawHandle() == Q_NULLPTR)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to open the file" << outputFilePath << ":\n"
                                      << outputSoundFile.strError();
        return {};
    }

    // Fill parameters from dusound infos
    // loop start/end is a quint32 in the WAV format
    SF_INSTRUMENT instrument;
    instrument.basenote = static_cast<char>(getUnityNote());
    instrument.detune = static_cast<char>(getFineTune());
    instrument.key_lo = static_cast<char>(getStartNote());
    instrument.key_hi = static_cast<char>(getEndNote());
    if (getLoopEnd() != 0) {
        instrument.loops[0].mode = SF_LOOP_FORWARD; // FIX if necessary !
        instrument.loop_count = 1;
        instrument.loops[0].start = static_cast<unsigned int>(getLoopStart() / 2);
        qCDebug(LOG_CAT_DU_OBJECT) << "Loopstart : " << instrument.loops[0].start;
        instrument.loops[0].end = static_cast<unsigned int>(getLoopEnd() / 2);
        qCDebug(LOG_CAT_DU_OBJECT) << "Loopend : " << instrument.loops[0].end;
        instrument.loops[0].count = 0; //Specify the number of times the loop is played. 0 means infinite.
    }
    qCDebug(LOG_CAT_DU_OBJECT) << outputSoundFile.command(SFC_SET_INSTRUMENT, &instrument, sizeof (instrument)) ;

    sf_count_t nbBytes = getData().size() ;
    qCDebug(LOG_CAT_DU_OBJECT) << "nbBytes " << nbBytes;
    outputSoundFile.writeRaw(reinterpret_cast<const short*>(getData().constData()), nbBytes) ;

    return outputFilePath;
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

    int tmpCpt = 0;
    while (QFile::exists(monoWavFileName) && tmpCpt < 10)
    {
        if (!QFile::remove(monoWavFileName))
        {
            monoWavFileName = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/tmp_mono" + QString::number(tmpCpt++) + ".wav";
        }
    }

    SndfileHandle newSoundFile(monoWavFileName.toStdString(), SFM_WRITE, oldSoundFile.format(), 1, oldSoundFile.samplerate());
    if (newSoundFile.rawHandle() == Q_NULLPTR)
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
    else if (newSoundFile.command(SFC_SET_INSTRUMENT, &inst, sizeof(inst)) != SF_TRUE)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to convert to mono\n"
                << "can't set instrument :" << newSoundFile.strError();
        return QString();
    }

    const sf_count_t oldNbFrames = oldSoundFile.channels() * oldSoundFile.frames();
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

    const sf_count_t newNbFrames = newSoundFile.channels() * oldSoundFile.frames();
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

QString DuSample::convertTo44100samplerate(SndfileHandle &oldSoundFile, sf_count_t* outNbFrames)
{
    if (oldSoundFile.channels() != 1)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to convert to 44100 samplerate\n"
                << "Wav is not mono:" << oldSoundFile.channels() << "channels";
        return QString();
    }

    QString wav44100samplerateFileName = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/tmp_44100samplerate.wav";
    if (wav44100samplerateFileName.isEmpty())
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to convert to 44100 samplerate\n"
                << "can't find temp folder";
        return QString();
    }

    int tmpCpt = 0;
    while (QFile::exists(wav44100samplerateFileName) && tmpCpt < 10)
    {
        if (!QFile::remove(wav44100samplerateFileName))
        {
            wav44100samplerateFileName = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/tmp_44100samplerate" + QString::number(tmpCpt++) + ".wav";
        }
    }

    SndfileHandle newSoundFile(wav44100samplerateFileName.toStdString(), SFM_WRITE, oldSoundFile.format(), 1, 44100);
    if (newSoundFile.rawHandle() == Q_NULLPTR)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Failed to open the file" << wav44100samplerateFileName << ":\n"
                                      << newSoundFile.strError();
        return QString();
    }

    //TODO: if we want to use the loop start/end written in sample data,
    //      we have to convert the sample rate for these infos too.
    //      But because of interpolation, it could have a bad quality...
    SF_INSTRUMENT inst;
    if (oldSoundFile.command(SFC_GET_INSTRUMENT, &inst, sizeof(inst)) != SF_TRUE)
    {
        qCDebug(LOG_CAT_DU_OBJECT) << "No instrument info found";
    }
    else if (newSoundFile.command(SFC_SET_INSTRUMENT, &inst, sizeof(inst)) != SF_TRUE)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to convert to 44100 samplerate\n"
                << "can't set instrument :" << newSoundFile.strError();
        return QString();
    }

    const sf_count_t oldNbFrames = oldSoundFile.frames();
    QScopedArrayPointer<double> oldFrames(new double[oldNbFrames]);
    sf_count_t nbFramesRead = oldSoundFile.readf(oldFrames.data(), oldNbFrames);
    if (nbFramesRead != oldNbFrames)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to convert to 44100 samplerate\n"
                << "error reading frames from old sound"
                << "info.frames =" << oldNbFrames << "\n"
                << "frames read =" << nbFramesRead << "\n"
                << oldSoundFile.strError();
        return QString();
    }

    double oldSampleRate = static_cast<double>(oldSoundFile.samplerate());

    // result wav length in s
    double duration = static_cast<double>(oldNbFrames) / oldSampleRate;

    // delta time in s
    double dt = 1.0 / 44100.0;

    sf_count_t newNbFrames = static_cast<sf_count_t>(duration * 44100.0);
    QScopedArrayPointer<double> newFrames(new double[newNbFrames]);

    int i = 0;
    for (double t = 0.0; t < duration && i < newNbFrames; t += dt)
    {
        int x1 = qFloor(t * oldSampleRate);
        int x2 = x1 + 1; //qCeil(t * oldSampleRate);
        int x0 = x1 - 1;
        int x3 = x2 + 1;

        double oldT = x1 * (1.0 / oldSampleRate);

        if (x0 < 0)
            x0 = 0;
        if (x0 >= oldNbFrames)
            x0 = static_cast<int>(oldNbFrames) - 1;

        if (x1 < 0)
            x1 = 0;
        if (x1 >= oldNbFrames)
            x1 = static_cast<int>(oldNbFrames) - 1;

        if (x2 < 0)
            x2 = 0;
        if (x2 >= oldNbFrames)
            x2 = static_cast<int>(oldNbFrames) - 1;

        if (x3 < 0)
            x3 = 0;
        if (x3 >= oldNbFrames)
            x3 = static_cast<int>(oldNbFrames) - 1;

        newFrames[i] = interpolate(oldFrames[x0], oldFrames[x1], oldFrames[x2], oldFrames[x3], t - oldT);

        ++i;
    }

    sf_count_t writtenFrames = newSoundFile.writef(newFrames.data(), newNbFrames);
    if (writtenFrames != newNbFrames)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to convert to 44100 samplerate\n"
                << "error writing frames to 44100 samplerate sound"
                << "info.frames =" << newNbFrames << "\n"
                << "frames written =" << writtenFrames << "\n"
                << newSoundFile.strError();
        return QString();
    }

    newSoundFile.writeSync();

    if (outNbFrames != Q_NULLPTR)
        *outNbFrames = newNbFrames;

    return wav44100samplerateFileName;
}

double DuSample::interpolate(double x0, double x1, double x2, double x3, double t)
{
    // Hermite4pt3oX

    double c0 = x1;
    double c1 = 0.5 * (x2 - x0);
    double c2 = x0 - (2.5 * x1) + (2.0 * x2) - (0.5 * x3);
    double c3 = (0.5 * (x3 - x0)) + (1.5 * (x1 - x2));
    return (((((c3 * t) + c2) * t) + c1) * t) + c0;
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

    int tmpCpt = 0;
    while (QFile::exists(wav16bitsFileName) && tmpCpt < 10)
    {
        if (!QFile::remove(wav16bitsFileName))
        {
            wav16bitsFileName = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/tmp_16bits" + QString::number(tmpCpt++) + ".wav";
        }
    }

    SndfileHandle newSoundFile(wav16bitsFileName.toStdString(), SFM_WRITE, normalizeWaveType(oldSoundFile.format()), oldSoundFile.channels(), oldSoundFile.samplerate());
    if (newSoundFile.rawHandle() == Q_NULLPTR)
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
    else if (newSoundFile.command(SFC_SET_INSTRUMENT, &inst, sizeof(inst)) != SF_TRUE)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "Failed to convert to 16 bits\n"
                << "can't set instrument :" << newSoundFile.strError();
        return QString();
    }

    const sf_count_t oldNbFrames = oldSoundFile.channels() * oldSoundFile.frames();
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
                << "Failed to convert to 16 bits\n"
                << "error writing frames to 16 bits sound"
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

    return static_cast<int>(newFormat);
}

QByteArray DuSample::ipBinary(quint8 min_vel, quint8 max_vel) const
{
    dream_ip dataStruct;

    std::memset(&dataStruct, 0, INSTR_DREAM_IP_SIZE);

    int tmpNum = 0;

    dataStruct.min_vel = min_vel;

    tmpNum = getStartNote() - 1;
    if (tmpNum == -1)
        return QByteArray();
    dataStruct.start_note = static_cast<quint8>(tmpNum);

    dataStruct.max_vel = max_vel;

    tmpNum = getEndNote();
    if (tmpNum == -1)
        return QByteArray();
    dataStruct.end_note = static_cast<quint8>(tmpNum);

    return QByteArray(reinterpret_cast<char*>(&dataStruct), INSTR_DREAM_IP_SIZE);
}

QByteArray DuSample::spBinary(quint32 sampleAddress, bool forDuTouchSOrL) const
{
    dream_sp data;

    std::memset(&data, 0, INSTR_DREAM_SP_SIZE);

    int tmpNum = 0;

    data.address1 = 0x0000;

    data.loopType = static_cast<quint16>(forDuTouchSOrL ? SND5000_OneShot : SND3000_Forward);

    data.address2 = 0x0000;
    data.address3 = 0x0000;

    data.unknown1 = 0xB5F0;
    data.unknown2 = 0x0814;
    data.unknown3 = 0xFF00;
    data.unknown4 = 0x1032;
    data.unknown5 = 0x04;

    tmpNum = getVolumeAmplifier();
    if (tmpNum == -1)
        return QByteArray();
    data.volume_amplifier = static_cast<quint8>(tmpNum);

    data.unknown6 = 0x0081;

    tmpNum = getFineTune();
    if (tmpNum == -1 || tmpNum >= 100)
        return QByteArray();
    data.fine_tune = finetune_convert[tmpNum];

    tmpNum = getUnityNote();
    if (tmpNum == -1)
        return QByteArray();
    if (tmpNum <= 64)
        data.coarse_tune = static_cast<quint8>(64 - tmpNum);
    else // tmpNum > 64
        data.coarse_tune = static_cast<quint8>(320 - tmpNum);

    tmpNum = getLoopStart();
    if (tmpNum == -1)
        return QByteArray();
    loopStartReadableToDream(static_cast<quint32>(tmpNum), sampleAddress, data.loop_start_MSB, data.loop_start_LSB);

    data.wav_address = wavAddressReadableToDream(sampleAddress);

    data.volume_mixer1 = 0xFEFF;

    tmpNum = getLoopEnd();
    if (tmpNum == -1)
        return QByteArray();
    loopEndReadableToDream(static_cast<quint32>(tmpNum), sampleAddress, data.loop_end_MSB, data.loop_end_LSB);

    data.unknown7 = 0x7F0E;
    data.unknown8 = 0x06;

    data.amplitude_osc_amp = forDuTouchSOrL ? 0x64 : 0x46;

    data.volume_mixer2 = forDuTouchSOrL ? 0xFFFF : 0xEFEF;

    tmpNum = initReadableToDream(getInitLevel());
    if (tmpNum == -1)
        return QByteArray();
    data.init = static_cast<quint16>(tmpNum);

    tmpNum = attackReadableToDream(getAttackRate(), getAttackLevel());
    if (tmpNum == -1)
        return QByteArray();
    data.attack = static_cast<quint16>(tmpNum);

    tmpNum = decayReadableToDream(getDecayRate(), getDecayLevel());
    if (tmpNum == -1)
        return QByteArray();
    data.decay = static_cast<quint16>(tmpNum);

    tmpNum = releaseReadableToDream(getReleaseRate(), getReleaseLevel());
    if (tmpNum == -1)
        return QByteArray();
    data.release = static_cast<quint16>(tmpNum);

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

    data.size_wav = sizeWavReadableToDream(getData().size());

    return QByteArray(reinterpret_cast<char*>(&data), INSTR_DREAM_SP_SIZE);
}

quint32 DuSample::wavAddressDreamToReadable(quint32 dreamValue, quint32 soundbankStartAddr)
{
    quint32 reorderedWavAddress = 0;
    reorderedWavAddress |= (0x000000FF & dreamValue) << 0;
    reorderedWavAddress |= (0x0000FF00 & dreamValue) << 16;
    reorderedWavAddress |= (0x00FF0000 & dreamValue) >> 8;
    reorderedWavAddress |= (0xFF000000 & dreamValue) >> 8;
    return (reorderedWavAddress - soundbankStartAddr) * 2;
}

quint32 DuSample::wavAddressReadableToDream(quint32 readableValue, quint32 soundbankStartAddr)
{
    quint32 tmpNum = (readableValue / 2) + soundbankStartAddr;
    quint32 reorderedWavAddress = 0;
    reorderedWavAddress |= (0x000000FF & tmpNum) << 0;
    reorderedWavAddress |= (0x0000FF00 & tmpNum) << 8;
    reorderedWavAddress |= (0x00FF0000 & tmpNum) << 8;
    reorderedWavAddress |= (0xFF000000 & tmpNum) >> 16;
    return reorderedWavAddress;
}

quint32 DuSample::loopStartDreamToReadable(quint16 loopStartMSB, quint16 loopStartLSB, quint32 sampleStartAddress, quint32 soundbankStartAddr)
{
    quint32 loopStart = static_cast<quint32>(loopStartMSB << 16) | static_cast<quint32>(loopStartLSB);
    quint32 reorderedLoopStart = 0;
    reorderedLoopStart |= ((0x000000FF & loopStart) - 0x00000001) << 24;
    reorderedLoopStart |=  (0x0000FF00 & loopStart) >> 8;
    reorderedLoopStart |=  (0x00FF0000 & loopStart) >> 8;
    reorderedLoopStart |=  (0xFF000000 & loopStart) >> 8;

    return (reorderedLoopStart - soundbankStartAddr) * 2 - sampleStartAddress;
}

void DuSample::loopStartReadableToDream(quint32 readableValue, quint32 sampleStartAddress, quint16& outLoopStartMSB, quint16& outLoopStartLSB, quint32 soundbankStartAddr)
{
    quint32 readableValueOffseted = ((readableValue + sampleStartAddress) / 2) + soundbankStartAddr;

    quint32 reorderedLoopStart = 0;
    reorderedLoopStart |=  (0x000000FF & readableValueOffseted) << 0;
    reorderedLoopStart |=  (0x0000FF00 & readableValueOffseted) << 16;
    reorderedLoopStart |=  (0x00FF0000 & readableValueOffseted) << 0;
    reorderedLoopStart |= ((0xFF000000 & readableValueOffseted) + 0x01000000) >> 16;

    quint16 reorderedLoopStart_MSB = (0xFFFF0000 & reorderedLoopStart) >> 16;
    quint16 reorderedLoopStart_LSB = (0x0000FFFF & reorderedLoopStart);

    outLoopStartMSB = static_cast<quint16>((0x00FF & reorderedLoopStart_MSB) << 8) | static_cast<quint16>((0xFF00 & reorderedLoopStart_MSB) >> 8);
    outLoopStartLSB = static_cast<quint16>((0x00FF & reorderedLoopStart_LSB) << 8) | static_cast<quint16>((0xFF00 & reorderedLoopStart_LSB) >> 8);
}

quint32 DuSample::loopEndDreamToReadable(quint16 loopEndMSB, quint16 loopEndLSB, quint32 sampleStartAddress, quint32 soundbankStartAddr)
{
    quint32 loopEnd = static_cast<quint32>(loopEndMSB << 16) | static_cast<quint32>(loopEndLSB);
    quint32 reorderedLoopEnd = 0;
    reorderedLoopEnd |= (0x000000FF & loopEnd) << 8;
    reorderedLoopEnd |= (0x0000FF00 & loopEnd) << 8;
    reorderedLoopEnd |= (0x00FF0000 & loopEnd) >> 16;
    reorderedLoopEnd |= (0xFF000000 & loopEnd) << 0;

    return (reorderedLoopEnd - soundbankStartAddr) * 2 - sampleStartAddress;
}

void DuSample::loopEndReadableToDream(quint32 readableValue, quint32 sampleStartAddress, quint16& outLoopEndMSB, quint16& outLoopEndLSB, quint32 soundbankStartAddr)
{
    quint32 readableValueOffseted = ((readableValue + sampleStartAddress) / 2) + soundbankStartAddr;

    quint32 reorderedLoopEnd = 0;
    reorderedLoopEnd |= (0x000000FF & readableValueOffseted) << 24;
    reorderedLoopEnd |= (0x0000FF00 & readableValueOffseted) << 0;
    reorderedLoopEnd |= (0x00FF0000 & readableValueOffseted) >> 16;
    reorderedLoopEnd |= (0xFF000000 & readableValueOffseted) >> 8;

    quint16 reorderedLoopEnd_MSB = (0xFFFF0000 & reorderedLoopEnd) >> 16;
    quint16 reorderedLoopEnd_LSB = (0x0000FFFF & reorderedLoopEnd);

    outLoopEndMSB = static_cast<quint16>((0x00FF & reorderedLoopEnd_MSB) << 8) | static_cast<quint16>((0xFF00 & reorderedLoopEnd_MSB) >> 8);
    outLoopEndLSB = static_cast<quint16>((0x00FF & reorderedLoopEnd_LSB) << 8) | static_cast<quint16>((0xFF00 & reorderedLoopEnd_LSB) >> 8);
}

int DuSample::initLevelDreamToReadable(quint16 dreamValue)
{
    return qFromBigEndian(dreamValue);
}

quint16 DuSample::initReadableToDream(int level)
{
    return qToBigEndian(static_cast<quint16>(level));
}

int DuSample::attackRateDreamToReadable(quint16 dreamValue)
{
    return (qFromBigEndian(dreamValue) >> 8) & 0x7F;
}

int DuSample::attackLevelDreamToReadable(quint16 dreamValue)
{
    quint8 LSB = ((qFromBigEndian(dreamValue) >> 8) >> 7) & 0x01;
    quint8 MSB = qFromBigEndian(dreamValue) & 0x1F;

    return (LSB | (MSB << 1)) & 0xFF;
}

quint16 DuSample::attackReadableToDream(int rate, int level)
{
    quint8 LSB = ((level >> 1) & 0x1F);
    quint8 MSB = ((level << 7) & 0x80) | (rate & 0x7F);

    return qToBigEndian(static_cast<quint16>((static_cast<quint16>(MSB) << 8) | LSB));
}

int DuSample::decayRateDreamToReadable(quint16 dreamValue)
{
    return (qFromBigEndian(dreamValue) >> 8) & 0x7F;
}

int DuSample::decayLevelDreamToReadable(quint16 dreamValue)
{
    quint8 LSB = ((qFromBigEndian(dreamValue) >> 8) >> 7) & 0x01;
    quint8 MSB = qFromBigEndian(dreamValue) & 0x1F;

    return (LSB | (MSB << 1)) & 0xFF;
}

quint16 DuSample::decayReadableToDream(int rate, int level)
{
    quint8 code = level == 0 ? 4 : 2;

    quint8 LSB = ((code << 5) & 0xE0) | ((level >> 1) & 0x1F);
    quint8 MSB = ((level << 7) & 0x80) | (rate & 0x7F);

    return qToBigEndian(static_cast<quint16>((static_cast<quint16>(MSB) << 8) | LSB));
}

int DuSample::releaseRateDreamToReadable(quint16 dreamValue)
{
    return (qFromBigEndian(dreamValue) >> 8) & 0x7F;
}

int DuSample::releaseLevelDreamToReadable(quint16 dreamValue)
{
    quint8 LSB = ((qFromBigEndian(dreamValue) >> 8) >> 7) & 0x01;
    quint8 MSB = qFromBigEndian(dreamValue) & 0x1F;

    return (LSB | (MSB << 1)) & 0xFF;
}

quint16 DuSample::releaseReadableToDream(int rate, int level)
{
    quint8 code = 3;

    quint8 LSB = ((code << 5) & 0x70) | ((level >> 1) & 0x1F);
    quint8 MSB = ((level << 7) & 0x80) | (rate & 0x7F);

    return qToBigEndian(static_cast<quint16>((static_cast<quint16>(MSB) << 8) | LSB));
}

int DuSample::sizeWavDreamToReadable(quint32 dreamValue)
{
    return static_cast<int>(dreamValue) * 2;
}

quint32 DuSample::sizeWavReadableToDream(int readableValue)
{
    return static_cast<quint32>(readableValue) / 2;
}

// Intrument Parameters
DU_KEY_ACCESSORS_IMPL(DuSample, StartNote,       Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSample, EndNote,         Numeric, int, -1)

// Sample Parameters
DU_KEY_ACCESSORS_IMPL(DuSample, LoopType,        Numeric, DuSample::SampleType, Unknown)

DU_KEY_ACCESSORS_IMPL(DuSample, VolumeAmplifier, Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuSample, FineTune,        Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSample, UnityNote,       Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuSample, LoopStart,       Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSample, LoopEnd,         Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuSample, InitLevel,       Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSample, AttackRate,      Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSample, AttackLevel,     Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSample, DecayRate,       Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSample, DecayLevel,      Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSample, ReleaseRate,     Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSample, ReleaseLevel,    Numeric, int, -1)

// Data
DU_KEY_ACCESSORS_IMPL(DuSample, Data,           BinaryData, QByteArray, QByteArray())

const quint8 DuSample::finetune_convert[101] = {
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
