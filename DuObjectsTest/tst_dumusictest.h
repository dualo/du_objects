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
    void testJsonInOut();
};

#endif // DUMUSICTEST_H
