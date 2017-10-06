#ifndef DUENUM_H
#define DUENUM_H

#include "dunumeric.h"

#include <QMetaEnum>

DU_OBJECT(DuEnumNoTemplate);

class DuEnumNoTemplate : public DuNumeric
{
public:
    explicit DuEnumNoTemplate(int defaultValue, int byteSize, int maxValue, int minValue);
    explicit DuEnumNoTemplate(int value, int byteSize, int maxValue, int minValue, int defaultValue);
    virtual ~DuEnumNoTemplate() = default;

    virtual QMap<int, QString> getAvailableValues() const = 0;
    virtual QString getEnumString() const = 0;
};

DU_OBJECT_TEMPLATE(DuEnum);

template <class T>
class DuEnum : public DuEnumNoTemplate
{
    static_assert(std::is_enum<T>::value, "DuEnum requires an enumeration type");

public:
    explicit DuEnum(T defaultValue, int byteSize, T maxValue, T minValue);
    explicit DuEnum(T value, int byteSize, T maxValue, T minValue, T defaultValue);
    virtual ~DuEnum() = default;

    T getEnum() const;
    bool setEnum(T value);

    // DuEnumNoTemplate interface
public:
    virtual QMap<int, QString> getAvailableValues() const Q_DECL_OVERRIDE;
    virtual QString getEnumString() const Q_DECL_OVERRIDE;

    // DuObject interface
public:
    virtual DuObjectPtr clone() const Q_DECL_OVERRIDE;
    virtual QDebug debugPrint(QDebug dbg) const Q_DECL_OVERRIDE;
};

DU_OBJECT_TEMPLATE_IMPL(DuEnum)

template<class T>
DuEnum<T>::DuEnum(T defaultValue, int byteSize, T maxValue, T minValue) :
    DuEnumNoTemplate(defaultValue, byteSize, maxValue, minValue)
{
}

template<class T>
DuEnum<T>::DuEnum(T value, int byteSize, T maxValue, T minValue, T defaultValue) :
    DuEnumNoTemplate(value, byteSize, maxValue, minValue, defaultValue)
{
}

template<class T>
T DuEnum<T>::getEnum() const
{
    return static_cast<T>(getNumeric());
}

template<class T>
bool DuEnum<T>::setEnum(T value)
{
    return setNumeric(value);
}

template<class T>
QMap<int, QString> DuEnum<T>::getAvailableValues() const
{
    const QMetaEnum& meta = QMetaEnum::fromType<T>();

    QMap<int, QString> values;
    for (int i = 0; i < meta.keyCount(); ++i)
    {
        values.insert(meta.value(i), QString::fromUtf8(meta.key(i)));
    }

    return values;
}

template<class T>
QString DuEnum<T>::getEnumString() const
{
    return QString::fromUtf8(QMetaEnum::fromType<T>().valueToKey(getNumeric()));
}

template<class T>
DuObjectPtr DuEnum<T>::clone() const
{
    return DuEnumPtr<T>(new DuEnum<T>(*this));
}

template<class T>
QDebug DuEnum<T>::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuEnum<" << QMetaEnum::fromType<T>().name() << ">(" << getEnum() << ")";

    return dbg.space();
}

#endif // DUENUM_H
