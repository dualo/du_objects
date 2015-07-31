#include "dutimesignaturemodel.h"


DuTimeSignature::DuTimeSignature(int value, const QString &name) :
    m_value(value),
    m_name(name)
{
}

int DuTimeSignature::value() const
{
    return m_value;
}

QString DuTimeSignature::name() const
{
    return m_name;
}



DuTimeSignatureModel::DuTimeSignatureModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

void DuTimeSignatureModel::addTimeSignature(const DuTimeSignature &timeSig)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_timeSigs << timeSig;
    endInsertRows();
}

int DuTimeSignatureModel::findValue(const QString &name) const
{
    QListIterator<DuTimeSignature> it(m_timeSigs);

    while (it.hasNext())
    {
        const DuTimeSignature &tmpTimeSig = it.next();
        if (tmpTimeSig.name() == name)
            return tmpTimeSig.value();
    }

    return -1;
}

int DuTimeSignatureModel::indexFromValue(int value) const
{
    int count = m_timeSigs.count();

    for (int i = 0; i < count; i++)
    {
        const DuTimeSignature &tmpTimeSig = m_timeSigs[i];
        if (tmpTimeSig.value() == value)
            return i;
    }

    return -1;
}

int DuTimeSignatureModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_timeSigs.count();
}

QVariant DuTimeSignatureModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_timeSigs.count())
        return QVariant();

    const DuTimeSignature &timeSig = m_timeSigs[index.row()];
    if (role == ValueRole)
        return timeSig.value();
    else if (role == NameRole)
        return timeSig.name();
    return QVariant();
}

QHash<int, QByteArray> DuTimeSignatureModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ValueRole] = "value";
    roles[NameRole] = "name";
    return roles;
}
