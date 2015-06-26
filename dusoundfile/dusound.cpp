#include "dusound.h"

#include <cstring>

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
    QScopedPointer<s_instr_m3> soundM3Infos(new s_instr_m3);

    std::memcpy((char*)soundM3Infos.data(), &data.data()[0x20], INSTR_INFO_INSTR_SIZE);

    DuSoundPtr sound(new DuSound);

    if (soundM3Infos->instr_midi_pc != 0xFF)
    {
        sound->setData(data);
    }

    return sound;
}

DuSoundPtr DuSound::fromBinary(QIODevice *input)
{
    QByteArray array = input->readAll();

    if (array.isEmpty())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuSound::fromBinary():\n"
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
    QScopedPointer<s_instr_m3> soundM3Infos(new s_instr_m3);

    std::memcpy((char*)soundM3Infos.data(), &getData().data()[0x20], INSTR_INFO_INSTR_SIZE);

    return QByteArray((char *)soundM3Infos->instr_name, NAME_CARACT);
}

void DuSound::setName(const QString &name)
{
    QByteArray newNameData(NAME_CARACT, 0);
    newNameData.replace(0, name.size(), name.toUtf8());

    QByteArray data = getData();
    std::memcpy(((s_instr_m3*)&data.data()[0x20])->instr_name, newNameData, NAME_CARACT);

    setData(data);
}
