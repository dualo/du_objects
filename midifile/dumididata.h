#ifndef DUMIDIDATA_H
#define DUMIDIDATA_H

#include "dumidivalue.h"
#include <QDataStream>


DU_OBJECT(DuMidiData)

class DuMidiData : public DuMidiValue
{
public:
    explicit DuMidiData(int maxSize = -1);
    virtual ~DuMidiData();

    virtual DuObjectPtr clone() const;

    virtual const QByteArray toMidiBinary() const;

    virtual int size() const;

    const QByteArray getData() const;
    bool setData(const QByteArray &value);
    void setData(QDataStream &stream);

    void resize(int size);
    void append(char c);

protected:
    QByteArray data();
};

#endif // DUMIDIDATA_H
