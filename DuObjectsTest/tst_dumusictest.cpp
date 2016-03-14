#include "tst_dumusictest.h"

#include <QtTest>

#include <dumusicfile/dumusic.h>

DuMusicTest::DuMusicTest() :
    QObject()
{
}

void DuMusicTest::testInOut()
{
    DuMusicPtr musicObj(new DuMusic);
    QByteArray musicData = musicObj->toDuMusicBinary();
    DuMusicPtr fromBinaryMusic = DuMusic::fromBinary(musicData);
    QByteArray toBinaryMusicData = fromBinaryMusic->toDuMusicBinary();

    QCOMPARE(musicData.size(), toBinaryMusicData.size());
    QCOMPARE(musicData, toBinaryMusicData);
}

void DuMusicTest::testJsonInOut()
{
    DuMusicPtr obj(new DuMusic);
    QJsonValue data = obj->toJson();
    QVERIFY(!data.isNull());
    QVERIFY(!data.isUndefined());
    QVERIFY(data.isObject());

    DuMusicPtr fromJsonObj = DuMusic::fromJson(data.toObject());
    QJsonValue toJsonData = fromJsonObj->toJson();
    QVERIFY(!toJsonData.isNull());
    QVERIFY(!toJsonData.isUndefined());
    QVERIFY(toJsonData.isObject());

    QCOMPARE(data, toJsonData);
}
