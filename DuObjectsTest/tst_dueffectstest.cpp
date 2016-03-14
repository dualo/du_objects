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

void DuEffectsTest::testMixerJsonInOut()
{
    DuMixerPtr obj(new DuMixer);
    QJsonValue data = obj->toJson();
    QVERIFY(!data.isNull());
    QVERIFY(!data.isUndefined());
    QVERIFY(data.isObject());

    DuMixerPtr fromJsonObj = DuMixer::fromJson(data.toObject());
    QJsonValue toJsonData = fromJsonObj->toJson();
    QVERIFY(!toJsonData.isNull());
    QVERIFY(!toJsonData.isUndefined());
    QVERIFY(toJsonData.isObject());

    QCOMPARE(data, toJsonData);
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

void DuEffectsTest::testDistortionJsonInOut()
{
    DuDistortionPtr obj(new DuDistortion);
    QJsonValue data = obj->toJson();
    QVERIFY(!data.isNull());
    QVERIFY(!data.isUndefined());
    QVERIFY(data.isObject());

    DuDistortionPtr fromJsonObj = DuDistortion::fromJson(data.toObject());
    QJsonValue toJsonData = fromJsonObj->toJson();
    QVERIFY(!toJsonData.isNull());
    QVERIFY(!toJsonData.isUndefined());
    QVERIFY(toJsonData.isObject());

    QCOMPARE(data, toJsonData);
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

void DuEffectsTest::testCompressorJsonInOut()
{
    DuCompressorPtr obj(new DuCompressor);
    QJsonValue data = obj->toJson();
    QVERIFY(!data.isNull());
    QVERIFY(!data.isUndefined());
    QVERIFY(data.isObject());

    DuCompressorPtr fromJsonObj = DuCompressor::fromJson(data.toObject());
    QJsonValue toJsonData = fromJsonObj->toJson();
    QVERIFY(!toJsonData.isNull());
    QVERIFY(!toJsonData.isUndefined());
    QVERIFY(toJsonData.isObject());

    QCOMPARE(data, toJsonData);
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

void DuEffectsTest::testEqualizerJsonInOut()
{
    DuEqualizerPtr obj(new DuEqualizer);
    QJsonValue data = obj->toJson();
    QVERIFY(!data.isNull());
    QVERIFY(!data.isUndefined());
    QVERIFY(data.isObject());

    DuEqualizerPtr fromJsonObj = DuEqualizer::fromJson(data.toObject());
    QJsonValue toJsonData = fromJsonObj->toJson();
    QVERIFY(!toJsonData.isNull());
    QVERIFY(!toJsonData.isUndefined());
    QVERIFY(toJsonData.isObject());

    QCOMPARE(data, toJsonData);
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

void DuEffectsTest::testDelayJsonInOut()
{
    DuDelayPtr obj(new DuDelay);
    QJsonValue data = obj->toJson();
    QVERIFY(!data.isNull());
    QVERIFY(!data.isUndefined());
    QVERIFY(data.isObject());

    DuDelayPtr fromJsonObj = DuDelay::fromJson(data.toObject());
    QJsonValue toJsonData = fromJsonObj->toJson();
    QVERIFY(!toJsonData.isNull());
    QVERIFY(!toJsonData.isUndefined());
    QVERIFY(toJsonData.isObject());

    QCOMPARE(data, toJsonData);
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

void DuEffectsTest::testChorusJsonInOut()
{
    DuChorusPtr obj(new DuChorus);
    QJsonValue data = obj->toJson();
    QVERIFY(!data.isNull());
    QVERIFY(!data.isUndefined());
    QVERIFY(data.isObject());

    DuChorusPtr fromJsonObj = DuChorus::fromJson(data.toObject());
    QJsonValue toJsonData = fromJsonObj->toJson();
    QVERIFY(!toJsonData.isNull());
    QVERIFY(!toJsonData.isUndefined());
    QVERIFY(toJsonData.isObject());

    QCOMPARE(data, toJsonData);
}
