#ifndef DUSCALEMODEL_H
#define DUSCALEMODEL_H

#include <QObject>
#include <QString>
#include <QAbstractListModel>
#include <QModelIndex>
#include <QVariant>


class DuScale
{
public:
    DuScale(const QString &id, const QString &name);

    QString id() const;
    QString name() const;

private:
    QString m_id;
    QString m_name;
};


class DuScaleModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum ScaleRoles {
        IdRole = Qt::UserRole + 1,
        NameRole
    };

    explicit DuScaleModel(QObject *parent = 0);

    void addScale(const DuScale &scale);

    QString findId (const QString &name) const;
    QString findName(const QString &id) const;

    int indexFromId(const QString &id) const;
    int indexFromName(const QString &name) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<DuScale> m_scales;
};

#endif // DUSCALEMODEL_H
