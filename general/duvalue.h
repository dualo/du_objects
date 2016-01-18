#ifndef DUVALUE_H
#define DUVALUE_H

#include "duobject.h"
#include <QVariant>


DU_OBJECT(DuValue)

class DuValue : public DuObject
{
public:
    explicit DuValue(int maxSize = -1);
    explicit DuValue(const QVariant &value, int maxSize = -1);
    virtual ~DuValue();

    virtual int size() const;

    const QVariant &getValue() const;
    bool setValue(const QVariant &value);
    virtual QVariant checkValue(const QVariant &value, bool &success);

    int getMaxSize() const;
    void setMaxSize(int value);

    QDebug debugPrint(QDebug dbg) const;

protected:
    QVariant &getValue();

private:
    QVariant value;

    /**
     * @brief if -1, there is no max size (default)
     */
    int maxSize;
};

#endif // DUVALUE_H
