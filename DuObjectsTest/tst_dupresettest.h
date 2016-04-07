#ifndef DUPRESETTEST_H
#define DUPRESETTEST_H

#include <QObject>


class DuPresetTest : public QObject
{
    Q_OBJECT

public:
    DuPresetTest();

private slots:
    void testPresetInOut();
    void testPresetJsonInOut();
};

#endif // DUPRESETTEST_H
