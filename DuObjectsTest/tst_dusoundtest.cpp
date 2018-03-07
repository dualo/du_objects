#include "tst_dusoundtest.h"

#include <QString>
#include <QtTest>

#include <dusoundfile/dusound.h>

DuSoundTest::DuSoundTest() :
    QObject()
{
}

void DuSoundTest::testInOut()
{
    DuSoundPtr soundObj(new DuSound);
    QByteArray soundData = soundObj->toBinary(true);
    DuSoundPtr fromBinarySound = DuSound::fromBinary(soundData);
    QByteArray toBinarySoundData = fromBinarySound->toBinary(true);

    QCOMPARE(soundData.size(), toBinarySoundData.size());
    QCOMPARE(soundData, toBinarySoundData);
}
