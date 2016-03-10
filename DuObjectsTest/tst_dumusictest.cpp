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
