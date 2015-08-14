#include "dusoundheader.h"

#include "../general/dunumeric.h"

DuSoundHeader::DuSoundHeader() :
    DuContainer()
{
    addChild(KeySize,            new DuNumeric(0));
    addChild(KeyMappingAddress,  new DuNumeric(0));
    addChild(KeyMetadataAddress, new DuNumeric(0));
}

DuObjectPtr DuSoundHeader::clone() const
{
    return DuSoundHeaderPtr(new DuSoundHeader(*this));
}

int DuSoundHeader::size() const
{
    return INSTR_HEADER_SIZE;
}

DuSoundHeaderPtr DuSoundHeader::fromBinary(const s_instr_header &data)
{
    DuSoundHeaderPtr header(new DuSoundHeader);

    bool verif = true;

    verif = header->setSize(data.full_size)              ? verif : false;
    verif = header->setMappingAddress(data.mapping_addr) ? verif : false;
    verif = header->setMetadataAddress(data.meta)        ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "An attribute was not properly set";
    }

    return header;
}

QByteArray DuSoundHeader::toDuMusicBinary() const
{
    int tmpInt;

    // HEADER
    s_instr_header soundHeader;
    std::memcpy((char*)&soundHeader, QByteArray(size(), 0), size());

    soundHeader.KW_INST = 0x54534E49;
    soundHeader.KW_MAPP = 0x5050414D;
    soundHeader.KW_META = 0x4154454D;

    tmpInt = getSize();
    if (tmpInt == -1)
        return QByteArray();
    soundHeader.full_size = tmpInt;

    tmpInt = getMappingAddress();
    if (tmpInt == -1)
        return QByteArray();
    soundHeader.mapping_addr = tmpInt;

    tmpInt = getMetadataAddress();
    if (tmpInt == -1)
        return QByteArray();
    soundHeader.meta = tmpInt;

    return QByteArray((char*)&soundHeader, size());
}

DU_KEY_ACCESSORS_IMPL(DuSoundHeader, Size,            Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSoundHeader, MappingAddress,  Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSoundHeader, MetadataAddress, Numeric, int, -1)
