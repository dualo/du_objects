#ifndef DUTONALITYMODEL_H
#define DUTONALITYMODEL_H

#include <QString>
#include <QAbstractListModel>


class DuTonality
{
public:
    DuTonality(int value, int midiRef, const QString &name);

    int value() const;
    int midiRef() const;
    QString name() const;

private:
    int m_value;
    int m_midiRef;
    QString m_name;
};


class DuTonalityModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum TonalityRoles {
        ValueRole = Qt::UserRole + 1,
        NameRole
    };

    explicit DuTonalityModel(QObject *parent = 0);

    void addTonality(const DuTonality &tonality);

    int findValue (const QString &name) const;
    int findValue (int midiRef) const;
    QString findName(int midiRef) const;

    int indexFromValue(int value) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<DuTonality> m_tonalities;
};

#endif // DUTONALITYMODEL_H
