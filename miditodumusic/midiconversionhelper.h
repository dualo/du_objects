#ifndef MIDICONVERSIONHELPER_H
#define MIDICONVERSIONHELPER_H

#include "../midifile/dumidifile.h"
#include "../dumusicfile/instrument/duinstrument.h"

#include "dumidikeymapper.h"

#include <QPair>


class MidiConversionHelper
{
public:
    explicit MidiConversionHelper();
    ~MidiConversionHelper();

    bool isValid() const;

    int getDuration() const;

    QList<QString> mapList() const;
    void chooseMap();

    int getTempo() const;
    void setTempo(int value);

    int getTimeSig() const;
    void setTimeSig(int value);

    int getScale() const;
    void setScale(int value);

    int getTonality() const;
    void setTonality(int value);

    QString getTitle() const;
    void setTitle(const QString &value);

    QList<DuMidiTrackPtr> getTracks();
    int getMidiChannel(int index) const;

    QString getTrackName(int index) const;
    const QStringList getTrackNames() const;

    void addSelection(int trackNum, int loopNum);
    void removeSelectionAt(int index);

    QPair<int, int> getIndexes(int index) const;
    int findIndexes(int trackIndex, int loopIndex) const;

    const DuMidiTrackPtr getMidiTrack(int index) const;
    void setSelectedTrack(int index, const DuMidiTrackPtr &midiTrack);

    const DuInstrumentPtr getInstrument(int index) const;
    void setSelectedInstr(int index, const DuInstrumentPtr &instrument);

    QPair<bool, int> getPercuSetting(int index) const;

    void setPercuMapping(int index, const QPair<bool, int> &mapping);

    bool isPercu(int index) const;

    int fetchKeyboard(int key, int index) const;
    int fetchPercuKey(int gmKey, int index) const;

private:
    bool importMidiFile();
    bool populateMapper();
    bool filterMetaEvents();

private:
    bool valid;
    int duration;

    DuMidiKeyMapperPtr mapper;

    int tempo;
    int timeSig;
    int scale;
    int tonality;
    QString title;

    DuMidiFilePtr selectedFile;

    QList<QString> trackNames;

    QList<QPair<int, int>> selectedIndexes;

    QList<DuMidiTrackPtr> selectedTracks;
    QList<DuInstrumentPtr> selectedInstruments;

    QList<QPair<bool, int>> percuMappings;

};

#endif // MIDICONVERSIONHELPER_H
