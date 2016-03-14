#include <QTest>
#include "tst_dueffectstest.h"
#include "tst_dumusictest.h"
#include "tst_dupresettest.h"

int main(int argc, char *argv[])
{
    DuEffectsTest t1;
    DuPresetTest t2;
    DuMusicTest t3;

    int result = 1;
    result = QTest::qExec(&t1, argc, argv) == 0 ? 0 : result;
    result = QTest::qExec(&t2, argc, argv) == 0 ? 0 : result;
    result = QTest::qExec(&t3, argc, argv) == 0 ? 0 : result;

    return result;
}
