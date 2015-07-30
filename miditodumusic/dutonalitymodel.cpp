#include "dutonalitymodel.h"


DuTonality::DuTonality(int value, const QString &name) :
    m_value(value),
    m_name(name)
{
}

int DuTonality::value() const
{
    return m_value;
}

QString DuTonality::name() const
{
    return m_name;
}



DuTonalityModel::DuTonalityModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

void DuTonalityModel::addTonality(const DuTonality &tonality)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_tonalities << tonality;
    endInsertRows();
}

int DuTonalityModel::findValue(const QString &name) const
{
    QListIterator<DuTonality> it(m_tonalities);

    while (it.hasNext())
    {
        const DuTonality &tmpTonality = it.next();
        if (tmpTonality.name() == name)
            return tmpTonality.value();
    }

    return -1;
}

int DuTonalityModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_tonalities.count();
}

QVariant DuTonalityModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_tonalities.count())
        return QVariant();

    const DuTonality &tonality = m_tonalities[index.row()];
    if (role == ValueRole)
        return tonality.value();
    else if (role == NameRole)
        return tonality.name();
    return QVariant();
}

QHash<int, QByteArray> DuTonalityModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ValueRole] = "value";
    roles[NameRole] = "name";
    return roles;
}
