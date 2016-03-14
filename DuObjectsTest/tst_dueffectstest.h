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
    void testMixerJsonInOut();

    void testDistortionInOut();
    void testDistortionJsonInOut();

    void testCompressorInOut();
    void testCompressorJsonInOut();

    void testEqualizerInOut();
    void testEqualizerJsonInOut();

    void testDelayInOut();
    void testDelayJsonInOut();

    void testChorusInOut();
    void testChorusJsonInOut();
};

#endif // DUEFFECTSTEST_H
