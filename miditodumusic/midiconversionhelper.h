#ifndef MIDICONVERSIONHELPER_H
#define MIDICONVERSIONHELPER_H

#include "../midifile/dumidifile.h"
#include "../dumusicfile/instrument/duinstrument.h"

#include "dumidikeymapper.h"

#include <QStringList>
#include <QPair>

#include "dutimesignaturemodel.h"
#include "duscalemodel.h"
#include "dutonalitymodel.h"


class MidiConversionHelper : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool valid READ isValid NOTIFY validChanged)

    Q_PROPERTY(int duration READ getDuration NOTIFY durationChanged)

    Q_PROPERTY(int tempo READ getTempo NOTIFY tempoChanged)
    Q_PROPERTY(int timeSig READ getTimeSig NOTIFY timeSigChanged)
    Q_PROPERTY(int scale READ getScale NOTIFY scaleChanged)
    Q_PROPERTY(int tonality READ getTonality NOTIFY tonalityChanged)
    Q_PROPERTY(QString title READ getTitle NOTIFY titleChanged)

    Q_PROPERTY(int midiTempo READ getMidiTempo NOTIFY validChanged)
    Q_PROPERTY(QString midiTimeSig READ getMidiTimeSigStr NOTIFY validChanged)
    Q_PROPERTY(QString midiScale READ getMidiScale NOTIFY validChanged)
    Q_PROPERTY(QString midiTonality READ getMidiTonality NOTIFY validChanged)
    Q_PROPERTY(QString midiTitle READ getMidiTitle NOTIFY validChanged)

    Q_PROPERTY(QStringList midiTracks READ getTrackNames NOTIFY validChanged)

public:
    explicit MidiConversionHelper(QObject *parent = 0);
    ~MidiConversionHelper();

    bool isValid() const;

    int getDuration() const;

    int getTempo() const;
    int getTimeSig() const;
    int getScale() const;
    int getTonality() const;
    QString getTitle() const;

    int getMidiTempo() const;
    QString getMidiTimeSigStr() const;
    QString getMidiScale() const;
    QString getMidiTonality() const;
    QString getMidiTitle() const;

    QList<DuMidiTrackPtr> getTracks();
    int getMidiChannel(int index) const;

    QString getTrackName(int index) const;
    const QStringList getTrackNames() const;

    QStringList mapList() const;
    void chooseMap();

    QPair<int, int> getIndexes(int index) const;

    const DuMidiTrackPtr getMidiTrack(int index) const;
    const DuInstrumentPtr getInstrument(int index) const;

    int getKeyboardFromMidi(int key) const;
    static int percuFromMidi(int gmKey, int mapIndex);
    static int percuToMidi(quint8 duKey, quint8 keyboardIndex, quint8 mapIndex);

    DuTimeSignatureModel *getTimeSigBoxModel();
    DuScaleModel *getScaleBoxModel();
    DuTonalityModel *getTonalityBoxModel();

    int getDutouchScale() const;

    QStringList midiScales() const;

public slots:
    void setTempo(int value);
    void setTimeSig(int value);
    void setScale(const QString &value);
    void setTonality(int value);

    void setTitle(const QString &value);

    int findTimeSig(const QString &key);
    QString findScale(const QString &key) const;
    int findTonality(const QString &key);

    void addSelection(int trackNum, int loopNum);
    void removeSelectionAt(int index);

    int findIndexes(int trackIndex, int loopIndex) const;

    void setSelectedTrack(int index, const DuMidiTrackPtr &midiTrack);
    void setSelectedTrack(int index, int midiTrackIndex);

    void setSelectedInstr(int index, const DuInstrumentPtr &instrument);
    void setSelectedInstr(int index, int instrumentIndex);

    bool importMidiFile(const DuMidiFilePtr &midiFile);
    bool populateMapper(const QJsonObject &jsonMaps);

    void importMidiFromFile();
    void importMapsFromFile();

    void newImport();

signals:
    void validChanged();

    void durationChanged();

    void tempoChanged();
    void timeSigChanged();
    void scaleChanged();
    void tonalityChanged();
    void titleChanged();

private:
    bool filterMetaEvents();

    DuScale getScaleIds(const QString &scale) const;

    void setMidiValid(bool value);
    void setMapsValid(bool value);

    void setDuration(int value);

    void setMidiTempo(int value);
    void setMidiTimeSig(int value);
    void setMidiScale(int value);
    void setMidiTonality(int value);
    void setMidiTitle(const QString &value);

private:
    bool midiValid;
    bool mapsValid;

    int duration;

    int tempo;
    int timeSig;
    QString scale;
    int tonality;
    QString title;

    int midiTempo;
    int midiTimeSig;
    int midiScale;
    int midiTonality;
    QString midiTitle;

    DuMidiFilePtr selectedFile;
    DuMidiKeyMapper *mapper;

    QStringList trackNames;

    QList<QPair<int, int> > selectedIndexes;

    QList<DuMidiTrackPtr> selectedTracks;
    QList<DuInstrumentPtr> selectedInstruments;

    QStringList midiScaleBoxModel;

    DuTimeSignatureModel timeSigBoxModel;
    DuScaleModel scaleBoxModel;
    DuTonalityModel tonalityBoxModel;
};

#endif // MIDICONVERSIONHELPER_H
