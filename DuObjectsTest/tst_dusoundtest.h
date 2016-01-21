#ifndef TST_DUSOUNDTEST_H
#define TST_DUSOUNDTEST_H

#include <QObject>


class DuSoundTest : public QObject
{
    Q_OBJECT

public:
    DuSoundTest();

private slots:
    void testInOut();
};

#endif // TST_DUSOUNDTEST_H
