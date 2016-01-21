#include <QTest>
#include "tst_dumusictest.h"
#include "tst_dusampletest.h"
#include "tst_dusoundtest.h"

int main(int argc, char *argv[])
{
    DuSampleTest t1;
    DuSoundTest t2;
    DuMusicTest t3;

    int result = 1;
    result = QTest::qExec(&t1, argc, argv) == 0 ? 0 : result;
    result = QTest::qExec(&t2, argc, argv) == 0 ? 0 : result;
    result = QTest::qExec(&t3, argc, argv) == 0 ? 0 : result;

    return result;
}
