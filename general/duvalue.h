#ifndef DUVALUE_H
#define DUVALUE_H

#include "duobject.h"
#include <QVariant>


DU_OBJECT(DuValue);

class DuValue : public DuObject
{
public:
    explicit DuValue(int maxSize = -1);
    explicit DuValue(const QVariant &value, int maxSize = -1);
    virtual ~DuValue();

    virtual bool parseJson(const QJsonValue &jsonValue) override;

    virtual int size() const override;

    const QVariant &getValue() const;
    bool setValue(const QVariant &value);

    int getMaxSize() const;
    void setMaxSize(int value);

    QDebug debugPrint(QDebug dbg) const override;

protected:
    QVariant &getValue();

    virtual QVariant checkValue(const QVariant &value, bool &success);

private:
    QVariant m_value;

    /**
     * @brief if -1, there is no max size (default)
     */
    int m_maxSize;
};

#endif // DUVALUE_H
