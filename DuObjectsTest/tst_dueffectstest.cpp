#include "tst_dueffectstest.h"

#include <QtTest>

#include <instrument/effects/duchorus.h>
#include <instrument/effects/ducompressor.h>
#include <instrument/effects/dudelay.h>
#include <instrument/effects/dudistortion.h>
#include <instrument/effects/duequalizer.h>
#include <instrument/effects/dumixer.h>

DuEffectsTest::DuEffectsTest() :
    QObject()
{

}

void DuEffectsTest::testMixerInOut()
{
    DuMixerPtr obj(new DuMixer);
    QByteArray data = obj->toDuMusicBinary();
    QVERIFY(!data.isEmpty());

    FX_mix dataStruct;
    std::memcpy((char*)&dataStruct, data.constData(), (size_t) data.size());
    DuMixerPtr fromBinaryObj = DuMixer::fromDuMusicBinary(dataStruct);
    QByteArray toBinaryData = fromBinaryObj->toDuMusicBinary();
    QVERIFY(!toBinaryData.isEmpty());

    QCOMPARE(data.size(), toBinaryData.size());
    QCOMPARE(data, toBinaryData);
}

void DuEffectsTest::testDistortionInOut()
{
    DuDistortionPtr obj(new DuDistortion);
    QByteArray data = obj->toDuMusicBinary();
    QVERIFY(!data.isEmpty());

    FX_distortion dataStruct;
    std::memcpy((char*)&dataStruct, data.constData(), (size_t) data.size());
    DuDistortionPtr fromBinaryObj = DuDistortion::fromDuMusicBinary(dataStruct);
    QByteArray toBinaryData = fromBinaryObj->toDuMusicBinary();
    QVERIFY(!toBinaryData.isEmpty());

    QCOMPARE(data.size(), toBinaryData.size());
    QCOMPARE(data, toBinaryData);
}

void DuEffectsTest::testCompressorInOut()
{
    DuCompressorPtr obj(new DuCompressor);
    QByteArray data = obj->toDuMusicBinary();
    QVERIFY(!data.isEmpty());

    FX_compressor dataStruct;
    std::memcpy((char*)&dataStruct, data.constData(), (size_t) data.size());
    DuCompressorPtr fromBinaryObj = DuCompressor::fromDuMusicBinary(dataStruct);
    QByteArray toBinaryData = fromBinaryObj->toDuMusicBinary();
    QVERIFY(!toBinaryData.isEmpty());

    QCOMPARE(data.size(), toBinaryData.size());
    QCOMPARE(data, toBinaryData);
}

void DuEffectsTest::testEqualizerInOut()
{
    DuEqualizerPtr obj(new DuEqualizer);
    QByteArray data = obj->toDuMusicBinary();
    QVERIFY(!data.isEmpty());

    FX_equalizer dataStruct;
    std::memcpy((char*)&dataStruct, data.constData(), (size_t) data.size());
    DuEqualizerPtr fromBinaryObj = DuEqualizer::fromDuMusicBinary(dataStruct);
    QByteArray toBinaryData = fromBinaryObj->toDuMusicBinary();
    QVERIFY(!toBinaryData.isEmpty());

    QCOMPARE(data.size(), toBinaryData.size());
    QCOMPARE(data, toBinaryData);
}

void DuEffectsTest::testDelayInOut()
{
    DuDelayPtr obj(new DuDelay);
    QByteArray data = obj->toDuMusicBinary();
    QVERIFY(!data.isEmpty());

    FX_delay dataStruct;
    std::memcpy((char*)&dataStruct, data.constData(), (size_t) data.size());
    DuDelayPtr fromBinaryObj = DuDelay::fromDuMusicBinary(dataStruct);
    QByteArray toBinaryData = fromBinaryObj->toDuMusicBinary();
    QVERIFY(!toBinaryData.isEmpty());

    QCOMPARE(data.size(), toBinaryData.size());
    QCOMPARE(data, toBinaryData);
}

void DuEffectsTest::testChorusInOut()
{
    DuChorusPtr obj(new DuChorus);
    QByteArray data = obj->toDuMusicBinary();
    QVERIFY(!data.isEmpty());

    FX_chorus dataStruct;
    std::memcpy((char*)&dataStruct, data.constData(), (size_t) data.size());
    DuChorusPtr fromBinaryObj = DuChorus::fromDuMusicBinary(dataStruct);
    QByteArray toBinaryData = fromBinaryObj->toDuMusicBinary();
    QVERIFY(!toBinaryData.isEmpty());

    QCOMPARE(data.size(), toBinaryData.size());
    QCOMPARE(data, toBinaryData);
}
