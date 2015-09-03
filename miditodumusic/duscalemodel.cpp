#include "duscalemodel.h"


DuScale::DuScale(const QString &id, const QString &name) :
    m_id(id),
    m_name(name)
{
}

QString DuScale::id() const
{
    return m_id;
}

QString DuScale::name() const
{
    return m_name;
}



DuScaleModel::DuScaleModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

void DuScaleModel::addScale(const DuScale &scale)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_scales << scale;
    endInsertRows();
}

QString DuScaleModel::findId(const QString &name) const
{
    QListIterator<DuScale> it(m_scales);

    while (it.hasNext())
    {
        const DuScale &tmpScale = it.next();
        if (tmpScale.name() == name)
            return tmpScale.id();
    }

    return QString();
}

QString DuScaleModel::findName(const QString &id) const
{
    QListIterator<DuScale> it(m_scales);

    while (it.hasNext())
    {
        const DuScale &tmpScale = it.next();
        if (tmpScale.id() == id)
            return tmpScale.name();
    }

    return QString();
}

int DuScaleModel::indexFromId(const QString &id) const
{
    int count = m_scales.count();

    for (int i = 0; i < count; i++)
    {
        const DuScale &tmpScale = m_scales[i];
        if (tmpScale.id() == id)
            return i;
    }

    return -1;
}

int DuScaleModel::indexFromName(const QString &name) const
{
    int count = m_scales.count();

    for (int i = 0; i < count; i++)
    {
        const DuScale &tmpScale = m_scales[i];
        if (tmpScale.name() == name)
            return i;
    }

    return -1;
}

int DuScaleModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_scales.count();
}

QVariant DuScaleModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_scales.count())
        return QVariant();

    const DuScale &scale = m_scales[index.row()];
    if (role == IdRole)
        return scale.id();
    else if (role == NameRole)
        return scale.name();
    return QVariant();
}

QHash<int, QByteArray> DuScaleModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    return roles;
}
