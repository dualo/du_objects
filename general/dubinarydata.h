#ifndef DUBINARYDATA_H
#define DUBINARYDATA_H

#include "duvalue.h"
#include <QDataStream>


DU_OBJECT(DuBinaryData)

class DuBinaryData : public DuValue
{
public:
    explicit DuBinaryData(int maxSize = -1);
    virtual ~DuBinaryData();

    virtual DuObjectPtr clone() const;

    QByteArray toDuMusicBinary() const;
    QByteArray toMidiBinary() const;
    QJsonValue toJson() const;

    virtual int size() const;

    const QByteArray getData() const;
    bool setData(const QByteArray &value);
    void setData(QDataStream &stream);

    void resize(int size);
    void append(char c);

protected:
    QByteArray data();
};

#endif // DUBINARYDATA_H
