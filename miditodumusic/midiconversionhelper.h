#ifndef MIDICONVERSIONHELPER_H
#define MIDICONVERSIONHELPER_H

#include "../midifile/dumidifile.h"
#include "../dumusicfile/instrument/duinstrument.h"

#include "dutimesignaturemodel.h"
#include "dutonalitymodel.h"

#include "dumidikeymapper.h"

#include <QStringList>
#include <QPair>


class MidiConversionHelper : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool valid READ isValid NOTIFY validChanged)

    Q_PROPERTY(int duration READ getDuration NOTIFY durationChanged)

    Q_PROPERTY(int tempo READ getTempo NOTIFY tempoChanged)
    Q_PROPERTY(int timeSig READ getTimeSig NOTIFY timeSigChanged)
    Q_PROPERTY(int scale READ getScale NOTIFY scaleChanged)
    Q_PROPERTY(int tonality READ getTonality NOTIFY tonalityChanged)

    Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY titleChanged)

    Q_PROPERTY(QString midiTimeSig READ getMidiTimeSigStr NOTIFY timeSigChanged)
    Q_PROPERTY(QStringList midiScales READ midiScales)

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

    QList<DuMidiTrackPtr> getTracks();
    int getMidiChannel(int index) const;

    QString getTrackName(int index) const;
    const QStringList getTrackNames() const;

    QList<QString> mapList() const;
    void chooseMap();

    QPair<int, int> getIndexes(int index) const;

    const DuMidiTrackPtr getMidiTrack(int index) const;

    const DuInstrumentPtr getInstrument(int index) const;

    bool isPercu(int index) const;

    int fetchKeyboard(int key, int index) const;
    int fetchPercuKey(int gmKey, int index) const;

    static int percuKey(quint8 duKey, quint8 keyboard, quint8 mapIndex);

    DuTimeSignatureModel *getTimeSigBoxModel();

    DuTonalityModel *getTonalityBoxModel();

    QStringList midiScales() const;

public slots:
    void setTempo(int value);
    void setTimeSig(int value);
    void setScale(int value);
    void setTonality(int value);

    void setTitle(const QString &value);

    QString getMidiTimeSigStr() const;

    int findTimeSig(const QString &key);
    int findTonality(const QString &key);

    void addSelection(int trackNum, int loopNum);
    void removeSelectionAt(int index);

    int findIndexes(int trackIndex, int loopIndex) const;

    void setSelectedTrack(int index, const DuMidiTrackPtr &midiTrack);

    void setSelectedInstr(int index, const DuInstrumentPtr &instrument);

    QPair<bool, int> getPercuMapping(int index) const;

    void setPercuMapping(int index, const QPair<bool, int> &mapping);

    bool importMidiFile(const DuMidiFilePtr &midiFile);
    bool populateMapper(const QJsonObject &jsonMaps);

    void importMidiFromFile();
    void importMapsFromFile();

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

private:
    bool midiValid;
    bool mapsValid;

    int duration;

    int tempo;
    int timeSig;
    int scale;
    int tonality;
    QString title;

    DuMidiFilePtr selectedFile;
    DuMidiKeyMapperPtr mapper;

    QStringList trackNames;

    QList<QPair<int, int>> selectedIndexes;

    QList<DuMidiTrackPtr> selectedTracks;
    QList<DuInstrumentPtr> selectedInstruments;

    QList<QPair<bool, int>> percuMappings;

    QStringList midiScaleBoxModel;

    DuTimeSignatureModel timeSigBoxModel;
    QList<QPair<int, QString>> scaleBoxModel;
    DuTonalityModel tonalityBoxModel;
};

#endif // MIDICONVERSIONHELPER_H
