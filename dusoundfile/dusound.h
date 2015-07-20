#ifndef DUSOUND_H
#define DUSOUND_H

#include "../general/dubinarydata.h"


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
