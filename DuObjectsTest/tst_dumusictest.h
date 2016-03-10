#ifndef DUMUSICTEST_H
#define DUMUSICTEST_H

#include <QObject>

class DuMusicTest : public QObject
{
    Q_OBJECT
public:
    DuMusicTest();

private slots:
    void testInOut();
};

#endif // DUMUSICTEST_H
