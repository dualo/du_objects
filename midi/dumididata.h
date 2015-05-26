#ifndef DUMIDIDATA_H
#define DUMIDIDATA_H


#include "../duvalue.h"
#include <QDataStream>


class DuMidiData : public DuValue
{
public:
    explicit DuMidiData(int maxSize = -1);
    virtual ~DuMidiData();

    QByteArray toDuMusicFile() const;
    QJsonValue toJson() const;

    virtual int size() const;

    const QByteArray getConstData() const;
    bool setData(const QByteArray &value);
    void setData(QDataStream &stream);

    QByteArray &append(char c);

protected:
    QByteArray getData();
};

#endif // DUMIDIDATA_H
