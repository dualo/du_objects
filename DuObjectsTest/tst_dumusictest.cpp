#include "tst_dumusictest.h"

#include <QtTest>

#include <dumusicfile/duevent.h>
#include <dumusicfile/duloop.h>
#include <dumusicfile/dumusic.h>
#include <dumusicfile/dutrack.h>

#include <general/duarray.h>

DuMusicTest::DuMusicTest() :
    QObject()
{
}

void DuMusicTest::testEventInOut()
{
    DuEventPtr obj(new DuEvent);
    QByteArray data = obj->toDuMusicBinary();

    music_sample dataStruct;
    std::memcpy((char*)&dataStruct, data.constData(), (size_t) data.size());
    DuEventPtr fromBinaryObj = DuEvent::fromDuMusicBinary(dataStruct);
    QByteArray toBinaryData = fromBinaryObj->toDuMusicBinary();

    QCOMPARE(data.size(), toBinaryData.size());
    QCOMPARE(data, toBinaryData);
}

void DuMusicTest::testEventJsonInOut()
{
    DuEventPtr obj(new DuEvent);
    QJsonValue data = obj->toJson();
    QVERIFY(!data.isNull());
    QVERIFY(!data.isUndefined());
    QVERIFY(data.isObject());

    DuEventPtr fromJsonObj(new DuEvent);
    QVERIFY(fromJsonObj->parseJson(data.toObject()));
    QJsonValue toJsonData = fromJsonObj->toJson();
    QVERIFY(!toJsonData.isNull());
    QVERIFY(!toJsonData.isUndefined());
    QVERIFY(toJsonData.isObject());

    QCOMPARE(data, toJsonData);
}

void DuMusicTest::testMusicInOut()
{
    DuMusicPtr obj(new DuMusic);

    DuLoopPtr loop = obj->getTracks()->at(0)->getLoops()->at(0);
    DuEventPtr event(new DuEvent);
    loop->appendEvent(event);
    loop->setState(REC_STOP);

    QByteArray data = obj->toDuMusicBinary();
    DuMusicPtr fromBinaryObj = DuMusic::fromBinary(data);
    QByteArray toBinaryData = fromBinaryObj->toDuMusicBinary();

    QCOMPARE(data.size(), toBinaryData.size());
    QCOMPARE(data, toBinaryData);
}

void DuMusicTest::testMusicJsonInOut()
{
    DuMusicPtr obj(new DuMusic);

    DuLoopPtr loop = obj->getTracks()->at(0)->getLoops()->at(0);
    DuEventPtr event(new DuEvent);
    loop->appendEvent(event);
    loop->setState(REC_STOP);

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
