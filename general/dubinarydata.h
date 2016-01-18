#ifndef DUBINARYDATA_H
#define DUBINARYDATA_H

#include "duvalue.h"
#include <QDataStream>


DU_OBJECT(DuBinaryData);

class DuBinaryData : public DuValue
{
public:
    explicit DuBinaryData(int maxSize = -1);
    virtual ~DuBinaryData();

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

    // DuObject interface
public:
    virtual DuObjectPtr clone() const override;

    virtual QByteArray toDuMusicBinary() const override;
    virtual QByteArray toMidiBinary() const override;
    virtual QJsonValue toJson() const override;

    virtual QDebug debugPrint(QDebug dbg) const override;

    virtual int size() const override;

    // DuValue interface
public:
    virtual QVariant checkValue(const QVariant &value, bool &success) override;
};

#endif // DUBINARYDATA_H
