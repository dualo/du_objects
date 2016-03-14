#ifndef DUEFFECTSTEST_H
#define DUEFFECTSTEST_H

#include <QObject>


class DuEffectsTest : public QObject
{
    Q_OBJECT

public:
    DuEffectsTest();

private slots:
    void testMixerInOut();
    void testDistortionInOut();
    void testCompressorInOut();
    void testEqualizerInOut();
    void testDelayInOut();
    void testChorusInOut();
};

#endif // DUEFFECTSTEST_H
