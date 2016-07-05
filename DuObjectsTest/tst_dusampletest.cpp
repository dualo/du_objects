#include "tst_dusampletest.h"

#include <QString>
#include <QtTest>

#include <dusoundfile/dusample.h>

DuSampleTest::DuSampleTest() :
    QObject()
{
}

void DuSampleTest::testConvertInit_data()
{
    QTest::addColumn<uint16_t>("dream");
    QTest::addColumn<int>("level");

    QTest::newRow("default") << (uint16_t) 0x7F00 << 127;
}

void DuSampleTest::testConvertInit()
{
    QFETCH(uint16_t, dream);
    QFETCH(int, level);

    QCOMPARE(DuSample::initLevelDreamToReadable(dream), level);
    QCOMPARE(DuSample::initReadableToDream(level), dream);
}

void DuSampleTest::testInOutInit()
{
    for (uint32_t dreamValue = 0; dreamValue <= 0xFFFF; ++dreamValue)
    {
        int level = DuSample::initLevelDreamToReadable((uint16_t) dreamValue);
        uint16_t newValue = DuSample::initReadableToDream(level);

        QCOMPARE(newValue, (uint16_t) dreamValue);
    }

    for (int level = 0; level <= 127; ++level)
    {
        uint16_t dreamValue = DuSample::initReadableToDream(level);
        int newValue = DuSample::initLevelDreamToReadable(dreamValue);

        QCOMPARE(newValue, level);
    }
}

void DuSampleTest::testConvertAttack_data()
{
    QTest::addColumn<uint16_t>("dream");
    QTest::addColumn<int>("level");
    QTest::addColumn<int>("rate");

    QTest::newRow("default") << (uint16_t) 0x1FE3 << 63 << 99;
}

void DuSampleTest::testConvertAttack()
{
    QFETCH(uint16_t, dream);
    QFETCH(int, level);
    QFETCH(int, rate);

    QCOMPARE(DuSample::attackLevelDreamToReadable(dream), level);
    QCOMPARE(DuSample::attackRateDreamToReadable(dream), rate);
    QCOMPARE(DuSample::attackReadableToDream(rate, level), dream);
}

void DuSampleTest::testInOutAttack()
{
    for (int level = 0; level <= 63; ++level)
    {
        for (int rate = 0; rate <= 127; ++rate)
        {
            uint16_t dreamValue = DuSample::attackReadableToDream(rate, level);
            int newLevel = DuSample::attackLevelDreamToReadable(dreamValue);
            int newRate = DuSample::attackRateDreamToReadable(dreamValue);

            QVERIFY2(newLevel == level && newRate == rate, QString("level=%1, newLevel=%2, rate=%3, newRate=%4").arg(level).arg(newLevel).arg(rate).arg(newRate).toLatin1().constData());
        }
    }

    for (uint32_t dreamValue = 0; dreamValue <= 0x1FFF; ++dreamValue)
    {
        int level = DuSample::attackLevelDreamToReadable((uint16_t) dreamValue);
        int rate = DuSample::attackRateDreamToReadable((uint16_t) dreamValue);
        uint16_t newValue = DuSample::attackReadableToDream(rate, level);

        QCOMPARE(newValue, (uint16_t) dreamValue);
    }
}

void DuSampleTest::testConvertDecay_data()
{
    QTest::addColumn<uint16_t>("dream");
    QTest::addColumn<int>("level");
    QTest::addColumn<int>("rate");

    QTest::newRow("default") << (uint16_t) 0x5FE3 << 63 << 99;
}

void DuSampleTest::testConvertDecay()
{
    QFETCH(uint16_t, dream);
    QFETCH(int, level);
    QFETCH(int, rate);

    QCOMPARE(DuSample::decayLevelDreamToReadable(dream), level);
    QCOMPARE(DuSample::decayRateDreamToReadable(dream), rate);
    QCOMPARE(DuSample::decayReadableToDream(rate, level), dream);
}

void DuSampleTest::testInOutDecay()
{
    for (int level = 0; level <= 63; ++level)
    {
        for (int rate = 0; rate <= 99; ++rate)
        {
            uint16_t dreamValue = DuSample::decayReadableToDream(rate, level);
            int newLevel = DuSample::decayLevelDreamToReadable(dreamValue);
            int newRate = DuSample::decayRateDreamToReadable(dreamValue);

            QVERIFY2(newLevel == level && newRate == rate, QString("level=%1, newLevel=%2, rate=%3, newRate=%4").arg(level).arg(newLevel).arg(rate).arg(newRate).toLatin1().constData());
        }
    }

    for (uint32_t dreamValue = 0x4080; dreamValue <= 0x5FE3; ++dreamValue)
    {
        int level = DuSample::decayLevelDreamToReadable((uint16_t) dreamValue);
        int rate = DuSample::decayRateDreamToReadable((uint16_t) dreamValue);
        uint16_t newValue = DuSample::decayReadableToDream(rate, level);

        QCOMPARE(newValue, (uint16_t) dreamValue);
    }

    for (uint32_t dreamValue = 0x8000; dreamValue <= 0x8063; ++dreamValue)
    {
        int level = DuSample::decayLevelDreamToReadable((uint16_t) dreamValue);
        int rate = DuSample::decayRateDreamToReadable((uint16_t) dreamValue);
        uint16_t newValue = DuSample::decayReadableToDream(rate, level);

        QCOMPARE(newValue, (uint16_t) dreamValue);
    }
}

void DuSampleTest::testConvertRelease_data()
{
    QTest::addColumn<uint16_t>("dream");
    QTest::addColumn<int>("level");
    QTest::addColumn<int>("rate");

    QTest::newRow("default") << (uint16_t) 0x605A << 0 << 90;
}

void DuSampleTest::testConvertRelease()
{
    QFETCH(uint16_t, dream);
    QFETCH(int, level);
    QFETCH(int, rate);

    QCOMPARE(DuSample::releaseLevelDreamToReadable(dream), level);
    QCOMPARE(DuSample::releaseRateDreamToReadable(dream), rate);
    QCOMPARE(DuSample::releaseReadableToDream(rate, level), dream);
}

void DuSampleTest::testInOutRelease()
{
    for (int level = 0; level <= 63; ++level)
    {
        for (int rate = 0; rate <= 99; ++rate)
        {
            uint16_t dreamValue = DuSample::releaseReadableToDream(rate, level);
            int newLevel = DuSample::releaseLevelDreamToReadable(dreamValue);
            int newRate = DuSample::releaseRateDreamToReadable(dreamValue);

            QVERIFY2(newLevel == level && newRate == rate, QString("level=%1, newLevel=%2, rate=%3, newRate=%4").arg(level).arg(newLevel).arg(rate).arg(newRate).toLatin1().constData());
        }
    }

    for (uint32_t dreamValue = 0x6000; dreamValue <= 0x7FE3; ++dreamValue)
    {
        int level = DuSample::releaseLevelDreamToReadable((uint16_t) dreamValue);
        int rate = DuSample::releaseRateDreamToReadable((uint16_t) dreamValue);
        uint16_t newValue = DuSample::releaseReadableToDream(rate, level);

        QCOMPARE(newValue, (uint16_t) dreamValue);
    }
}
