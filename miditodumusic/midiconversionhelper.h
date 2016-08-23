#ifndef MIDICONVERSIONHELPER_H
#define MIDICONVERSIONHELPER_H

#include "../general/duobject.h"

#include "dutimesignaturemodel.h"
#include "duscalemodel.h"
#include "dutimesignaturemodel.h"
#include "dutonalitymodel.h"
#include "dumidikeymapper.h"

Q_DECLARE_LOGGING_CATEGORY(LOG_CAT_MIDI)

DU_OBJECT(DuMidiFile);
DU_OBJECT(DuMidiTrack);
DU_OBJECT(DuSound);

struct MidiTrackData
{
    QString name;
    int programChange;
};

class MidiConversionHelper
{
    Q_DECLARE_TR_FUNCTIONS(MidiConversionHelper)
public:
    explicit MidiConversionHelper();
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

    QVector<DuMidiTrackPtr> getTracks();
    int getMidiChannel(int index) const;

    QList<MidiTrackData> midiTracks() const;

    QStringList mapList() const;
    void chooseMap();

    QPair<int, int> getIndexes(int index) const;

    DuMidiTrackPtr getMidiTrack(int index) const;
    DuSoundPtr getSound(int index) const;

    int getKeyboardFromMidi(quint8 key) const;
    static int percuToMidi(quint8 duKey, quint8 keyboardIndex, quint8 mapIndex);

    DuTimeSignatureModel *getTimeSigBoxModel();
    DuScaleModel *getScaleBoxModel();
    DuTonalityModel *getTonalityBoxModel();

    int getDutouchScale() const;

    QStringList midiScales() const;

    void setTempo(int value);
    void setTimeSig(int value);
    void setScale(const QString &value);
    void setTonality(int value);

    void setTitle(const QString &value);

    int findTimeSig(const QString &key);
    QString findScale(const QString &key) const;
    int findTonality(const QString &key);

    void addSelection(int trackNum, int loopNum, int midiTrackIndex, const DuSoundConstPtr& sound);
    void removeSelectionAt(int index);

    int findIndexes(int trackIndex, int loopIndex) const;

    bool importMidiFile(const DuMidiFilePtr &midiFile);
    bool populateMapper(const QJsonObject &jsonMaps);

    bool init(const QByteArray &midiRawData);

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
    DuMidiKeyMapper mapper;

    QList<MidiTrackData> m_midiTracks;

    QList<QPair<int, int> > selectedIndexes;
    QList<DuMidiTrackPtr> selectedTracks;
    QList<DuSoundPtr> selectedSounds;

    QStringList midiScaleBoxModel;

    DuTimeSignatureModel timeSigBoxModel;
    DuScaleModel scaleBoxModel;
    DuTonalityModel tonalityBoxModel;
};

#endif // MIDICONVERSIONHELPER_H
