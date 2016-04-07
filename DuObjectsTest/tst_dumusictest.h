#ifndef DUMUSICTEST_H
#define DUMUSICTEST_H

#include <QObject>

class DuMusicTest : public QObject
{
    Q_OBJECT
public:
    DuMusicTest();

private slots:
    void testEventInOut();
    void testEventJsonInOut();

    void testMusicInOut();
    void testMusicJsonInOut();
};

#endif // DUMUSICTEST_H
