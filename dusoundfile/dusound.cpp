#include "dusound.h"

#include <QDebug>

DU_OBJECT_IMPL(DuSound)

DuSound::DuSound() :
    DuBinaryData(),
    m_databaseId(-1)
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

    QScopedPointer<s_instr_m3> soundM3Infos(new s_instr_m3);

    std::memcpy((char*)soundM3Infos.data(), &data.data()[0x20], INSTR_INFO_INSTR_SIZE);

    sound->setName(QByteArray((char *)soundM3Infos->instr_name, NAME_CARACT));

    return sound;
}

DuSoundPtr DuSound::fromBinary(QIODevice *input)
{
    QByteArray array = input->readAll();

    if (array.isEmpty())
    {
        qCritical() << "DuSound::fromBinary():\n"
                    << "failed to generate DuSound\n"
                    << "selected file could not be read";

        return DuSoundPtr();
    }

    return DuSound::fromBinary(array);
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

QString DuSound::name() const
{
    return m_name;
}

void DuSound::setName(const QString &name)
{
    m_name = name;
}
