#ifndef DUTIMESIGNATUREMODEL_H
#define DUTIMESIGNATUREMODEL_H

#include <QString>
#include <QAbstractListModel>


class DuTimeSignature
{
public:
    DuTimeSignature(int value, const QString &name);

    int value() const;
    QString name() const;

private:
    int m_value;
    QString m_name;
};


class DuTimeSignatureModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum TimeSigRoles {
        ValueRole = Qt::UserRole + 1,
        NameRole
    };

    explicit DuTimeSignatureModel(QObject *parent = 0);

    void addTimeSignature(const DuTimeSignature &timeSig);

    int findValue(const QString &name) const;

    int indexFromValue(int value) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<DuTimeSignature> m_timeSigs;
};

#endif // DUTIMESIGNATUREMODEL_H
