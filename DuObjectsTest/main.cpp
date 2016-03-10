#include <QTest>
#include "tst_dumusictest.h"

int main(int argc, char *argv[])
{
    DuMusicTest t1;

    int result = 1;
    result = QTest::qExec(&t1, argc, argv) == 0 ? 0 : result;

    return result;
}
