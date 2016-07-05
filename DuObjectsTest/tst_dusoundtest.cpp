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
    QByteArray soundData = soundObj->toBinary();
    DuSoundPtr fromBinarySound = DuSound::fromBinary(soundData);
    QByteArray toBinarySoundData = fromBinarySound->toBinary();

    QCOMPARE(soundData.size(), toBinarySoundData.size());
    QCOMPARE(soundData, toBinarySoundData);
}
