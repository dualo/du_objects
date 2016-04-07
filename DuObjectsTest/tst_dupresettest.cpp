#include "tst_dupresettest.h"

#include <QtTest>

#include <instrument/dupreset.h>

DuPresetTest::DuPresetTest() :
    QObject()
{

}

void DuPresetTest::testPresetInOut()
{
    DuPresetPtr presetObj(new DuPreset);
    QByteArray presetData = presetObj->toDuMusicBinary();
    QVERIFY(!presetData.isEmpty());

    preset_instr presetStruct;
    std::memcpy((char*)&presetStruct, presetData.constData(), (size_t) presetData.size());
    DuPresetPtr fromBinaryPreset = DuPreset::fromDuMusicBinary(presetStruct);
    QByteArray toBinaryPresetData = fromBinaryPreset->toDuMusicBinary();
    QVERIFY(!toBinaryPresetData.isEmpty());

    QCOMPARE(presetData.size(), toBinaryPresetData.size());
    QCOMPARE(presetData, toBinaryPresetData);
}

void DuPresetTest::testPresetJsonInOut()
{
    DuPresetPtr obj(new DuPreset);
    QJsonValue data = obj->toJson();
    QVERIFY(!data.isNull());
    QVERIFY(!data.isUndefined());
    QVERIFY(data.isObject());

    DuPresetPtr fromJsonObj = DuPreset::fromJson(data.toObject());
    QJsonValue toJsonData = fromJsonObj->toJson();
    QVERIFY(!toJsonData.isNull());
    QVERIFY(!toJsonData.isUndefined());
    QVERIFY(toJsonData.isObject());

    QCOMPARE(data, toJsonData);
}
