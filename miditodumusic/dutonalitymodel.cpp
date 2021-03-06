#include "dutonalitymodel.h"


DuTonality::DuTonality(int value, int midiRef, const QString &name) :
    m_value(value),
    m_midiRef(midiRef),
    m_name(name)
{
}

int DuTonality::value() const
{
    return m_value;
}

int DuTonality::midiRef() const
{
    return m_midiRef;
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

int DuTonalityModel::findValue(int midiRef) const
{
    QListIterator<DuTonality> it(m_tonalities);

    while (it.hasNext())
    {
        const DuTonality &tmpTonality = it.next();
        if (tmpTonality.midiRef() == midiRef)
            return tmpTonality.value();
    }

    return -1;
}

QString DuTonalityModel::findName(int midiRef) const
{
    QListIterator<DuTonality> it(m_tonalities);

    while (it.hasNext())
    {
        const DuTonality &tmpTonality = it.next();
        if (tmpTonality.midiRef() == midiRef)
            return tmpTonality.name();
    }

    return QString();
}

int DuTonalityModel::indexFromValue(int value) const
{
    int count = m_tonalities.count();

    for (int i = 0; i < count; i++)
    {
        const DuTonality &tmpTonality = m_tonalities[i];
        if (tmpTonality.value() == value)
            return i;
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
