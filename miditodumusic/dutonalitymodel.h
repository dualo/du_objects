#ifndef DUTONALITYMODEL_H
#define DUTONALITYMODEL_H

#include <QString>
#include <QAbstractListModel>


class DuTonality
{
public:
    DuTonality(int value, const QString &name);

    int value() const;
    QString name() const;

private:
    int m_value;
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

    DuTonalityModel(QObject *parent = 0);

    void addTonality(const DuTonality &tonality);

    int findValue (const QString &name) const;
    QString findName(int value) const;

    int indexFromValue(int value) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<DuTonality> m_tonalities;
};

#endif // DUTONALITYMODEL_H
