#ifndef DUSOUND_H
#define DUSOUND_H

#include "../general/dubinarydata.h"


//TODO: this will be in an external
#pragma pack(push, 4)

#define NAME_CARACT	12
#define INSTR_INFO_DUMMY 7

typedef struct
{
    uint32_t KW_MMRY;
    uint8_t instr_name[NAME_CARACT];
    uint8_t instr_midi_pc;
    uint8_t instr_midi_C0;
    uint8_t instr_key_map;
    uint8_t instr_octave;
    uint32_t instr_user_id;
    uint32_t instr_id;
    uint32_t sample_address;
    uint8_t instr_noteoff;
    uint8_t instr_cat[NAME_CARACT];
    uint8_t instr_relvolume;
    uint8_t format_id;
    uint8_t touche;
    uint8_t nb_layer;
    uint16_t ip_size;
    uint16_t sp_size;
    uint32_t sample_size;
    uint8_t instr_dummy[INSTR_INFO_DUMMY];
} s_instr_m3;

#define INSTR_INFO_INSTR_SIZE 64 //(NAME_CARACT*2 + 5*4 + 2*2 + 9*1 + INSTR_INFO_DUMMY)

#pragma pack(pop)


DU_OBJECT(DuSound)

class DuSound : public DuBinaryData //TODO: inherit from DuContainer
{
public:
    DuSound();

    virtual DuObjectPtr clone() const;

    static DuSoundPtr fromBinary(const QByteArray &data);
    static DuSoundPtr fromBinary(QIODevice *input);

    int databaseId() const;
    void setDatabaseId(int databaseId);

    QStringList lists() const;
    void setLists(const QStringList &lists);

    QString name() const;
    void setName(const QString &name);

private:
    int m_databaseId;
    QStringList m_lists;
};

Q_DECLARE_METATYPE(DuSoundPtr)
Q_DECLARE_METATYPE(DuSoundConstPtr)

#endif // DUSOUND_H
