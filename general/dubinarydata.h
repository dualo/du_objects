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

    int size() const;

    const QByteArray getBinaryData() const;
    bool setBinaryData(const QByteArray &value);

    const QByteArray getData() const;
    bool setData(const QByteArray &value);

    /**
     * @brief Should only be used after a call to resize() to ensure the correct
     *          number of bytes will be read from the stream.
     */
    void setData(QDataStream &stream);

    void resize(int size);
    void append(char c);

protected:
    QByteArray data();
};

#endif // DUBINARYDATA_H
