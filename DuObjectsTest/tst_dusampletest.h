#ifndef TST_DUSAMPLETEST_H
#define TST_DUSAMPLETEST_H

#include <QObject>

class DuSampleTest : public QObject
{
    Q_OBJECT

public:
    DuSampleTest();

private slots:
    void testConvertInit_data();
    void testConvertInit();
    void testInOutInit();

    void testConvertAttack_data();
    void testConvertAttack();
    void testInOutAttack();

    void testConvertDecay_data();
    void testConvertDecay();
    void testInOutDecay();

    void testConvertRelease_data();
    void testConvertRelease();
    void testInOutRelease();
};

#endif // TST_DUSAMPLETEST_H
