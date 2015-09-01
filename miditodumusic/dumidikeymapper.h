#ifndef DUMIDIKEYMAPPER_H
#define DUMIDIKEYMAPPER_H

#include <QObject>
#include <QJsonObject>
#include <QStringList>
#include <QPair>


//TODO: update following list each time the scales in defaultmaps.json change

#define SCALE_NONE                  "None"
#define SCALE_BARTOK                "Bartok"
#define SCALE_DORIAN                "Dorian"
#define SCALE_DOUBLE_HARMONIC       "Double harmonic"
#define SCALE_HARMONIC_MINOR        "Harmonic minor"
#define SCALE_HUNGARIAN_MINOR       "Hungarian minor"
#define SCALE_LYDIAN                "Lydian"
#define SCALE_MAJOR                 "Major"
#define SCALE_MELODIC_MINOR         "Melodic minor"
#define SCALE_MINOR                 "Minor"
#define SCALE_MIXOLYDIAN            "Mixolydian"
#define SCALE_PHRYGIAN              "Phrygian"
#define SCALE_PHRYGIAN_DOMINANT     "Phrygian dominant"
#define SCALE_UKRAINIAN_DORIAN      "Ukrainian dorian"


class DuMidiKeyMapper : public QObject
{
    Q_OBJECT

public:
    explicit DuMidiKeyMapper(QObject *parent = 0);
    ~DuMidiKeyMapper();

    QJsonValue toJson() const;

    QDebug debugPrint(QDebug dbg) const;

    int size() const;

    void importMaps(QJsonObject value);

    QStringList mapList() const;
    void chooseMap(QString scale, quint8 tonality);

    QPair<QString, QString> scaleIds(const QString &scale) const;
    int dutouchScale(const QString &scale) const;

    int keyboardFromMidi(quint8 key);

private:
    QJsonObject m_maps;
    QString m_scale;
    quint8 m_tonality;
};

#endif // DUMIDIKEYMAPPER_H
