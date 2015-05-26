#ifndef DUMIDIDATA_H
#define DUMIDIDATA_H

#include "../duvalue.h"
#include <QDataStream>


DU_OBJECT(DuMidiData)

class DuMidiData : public DuValue
{
public:
    explicit DuMidiData(int maxSize = -1);
    virtual ~DuMidiData();

    QByteArray toDuMusicFile() const;
    QJsonValue toJson() const;

    virtual int size() const;

    QByteArray data();
    const QByteArray getData() const;
    bool setData(const QByteArray &value);
    void setData(QDataStream &stream);

    void resize(int size);
    QByteArray &append(char c);
};

#endif // DUMIDIDATA_H
