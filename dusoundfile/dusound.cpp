#include "dusound.h"

#include <QDebug>

DU_OBJECT_IMPL(DuSound)

DuSound::DuSound() :
    DuBinaryData()
{

}

DuObjectPtr DuSound::clone() const
{
    return DuSoundPtr(new DuSound(*this));
}

DuSoundPtr DuSound::fromBinary(const QByteArray &data)
{
    DuSoundPtr sound(new DuSound);
    sound->setData(data);

    return sound;
}

int DuSound::databaseId() const
{
    return m_databaseId;
}

void DuSound::setDatabaseId(int databaseId)
{
    m_databaseId = databaseId;
}

QStringList DuSound::lists() const
{
    return m_lists;
}

void DuSound::setLists(const QStringList &lists)
{
    m_lists = lists;
}
