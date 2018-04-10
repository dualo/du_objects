#ifndef DUTYPEDVALUE_H
#define DUTYPEDVALUE_H

#include "duvalue.h"

DU_OBJECT_TEMPLATE(DuTypedValue);

template<class T>
class DuTypedValue : public DuValue
{
public:
    explicit DuTypedValue(const T &value = {});
    virtual ~DuTypedValue() = default;

    virtual DuObjectPtr clone() const Q_DECL_OVERRIDE;

    T getTypedValue() const;
    bool setTypedValue(const T &value);

    QDebug debugPrint(QDebug dbg) const Q_DECL_OVERRIDE;

protected:
    T &getTypedValue();
};

DU_OBJECT_TEMPLATE_IMPL(DuTypedValue)

template<class T>
DuTypedValue<T>::DuTypedValue(const T &value)
{
    setTypedValue(value);
}

template<class T>
DuObjectPtr DuTypedValue<T>::clone() const
{
    return DuTypedValuePtr<T>(new DuTypedValue<T>(*this));
}

template<class T>
T DuTypedValue<T>::getTypedValue() const
{
    return getValue().template value<T>();
}

template<class T>
bool DuTypedValue<T>::setTypedValue(const T &value)
{
    return setValue(QVariant::fromValue(value));
}

template<class T>
QDebug DuTypedValue<T>::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuTypedValue(";
    dbg.nospace() << getTypedValue();
    dbg.nospace() << ")";

    return dbg.space();
}

template<class T>
T &DuTypedValue<T>::getTypedValue()
{
    return getValue().template value<T>();
}

#endif // DUTYPEDVALUE_H
