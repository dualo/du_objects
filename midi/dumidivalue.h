#ifndef DUMIDIVALUE_H
#define DUMIDIVALUE_H

#include "dumidiobject.h"
#include <QVariant>


DU_OBJECT(DuMidiValue)

class DuMidiValue : public DuMidiObject
{
public:
    explicit DuMidiValue(int maxSize = -1);
    virtual ~DuMidiValue();

    virtual QDebug debugPrint(QDebug dbg) const;

    virtual int size() const;

    const QVariant &getValue() const;
    void setValue(const QVariant &value);

    int getMaxSize() const;
    void setMaxSize(int value);

protected:
    QVariant &getValue();

private:
    QVariant value;

    /**
     * @brief if -1, there is no max size (default)
     */
    int maxSize;
};

#endif // DUMIDIVALUE_H
