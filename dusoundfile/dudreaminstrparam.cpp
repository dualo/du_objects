#include "dudreaminstrparam.h"

#include "../general/dunumeric.h"

DuDreamInstrParam::DuDreamInstrParam() :
    DuContainer()
{
    addChild(KeyMinVelocity,     new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0x7F, 0x00));
    addChild(KeyStartNote,       new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE, 0x7F, 0x00));
    addChild(KeyMaxVelocity,     new DuNumeric(0x7F, NUMERIC_DEFAULT_SIZE, 0x7F, 0x00));
    addChild(KeyEndNote,         new DuNumeric(0x7F, NUMERIC_DEFAULT_SIZE, 0x7F, 0x00));

    addChild(KeySampleParamAddr, new DuNumeric(0, 2, 0xFFFF, 0x0000));
}

DuObjectPtr DuDreamInstrParam::clone() const
{
    return DuDreamInstrParamPtr(new DuDreamInstrParam(*this));
}

int DuDreamInstrParam::size() const
{
    return INSTR_DREAM_IP_SIZE;
}

DuDreamInstrParamPtr DuDreamInstrParam::fromBinary(const dream_ip &data)
{
    DuDreamInstrParamPtr dreamIP(new DuDreamInstrParam);
    bool verif = true;

    verif = dreamIP->setMinVelocity(data.min_vel) ? verif : false;
    verif = dreamIP->setStartNote(data.start_note) ? verif : false;
    verif = dreamIP->setMaxVelocity(data.max_vel) ? verif : false;
    verif = dreamIP->setEndNote(data.end_note) ? verif : false;

    verif = dreamIP->setSampleParamAddr(data.dream_sp_addr) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "an attribute was not properly set";
    }

    return dreamIP;
}

QByteArray DuDreamInstrParam::toDuMusicBinary() const
{
    dream_ip dataStruct;

    int tmpNum = 0;

    tmpNum = getMinVelocity();
    if (tmpNum == -1)
        return QByteArray();
    dataStruct.min_vel = tmpNum;

    tmpNum = getStartNote();
    if (tmpNum == -1)
        return QByteArray();
    dataStruct.start_note = tmpNum;

    tmpNum = getMaxVelocity();
    if (tmpNum == -1)
        return QByteArray();
    dataStruct.max_vel = tmpNum;

    tmpNum = getEndNote();
    if (tmpNum == -1)
        return QByteArray();
    dataStruct.end_note = tmpNum;

    tmpNum = getSampleParamAddr();
    if (tmpNum == -1)
        return QByteArray();
    dataStruct.dream_sp_addr = tmpNum;

    return QByteArray((char*)&dataStruct, size());
}

DU_KEY_ACCESSORS_IMPL(DuDreamInstrParam, MinVelocity,     Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuDreamInstrParam, StartNote,       Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuDreamInstrParam, MaxVelocity,     Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuDreamInstrParam, EndNote,         Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuDreamInstrParam, SampleParamAddr, Numeric, int, -1)
