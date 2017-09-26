#include "DuEnum.h"

DU_OBJECT_IMPL(DuEnumNoTemplate)

DuEnumNoTemplate::DuEnumNoTemplate(int defaultValue, int byteSize, int maxValue, int minValue) :
    DuNumeric(defaultValue, byteSize, maxValue, minValue)
{
}

DuEnumNoTemplate::DuEnumNoTemplate(int value, int byteSize, int maxValue, int minValue, int defaultValue) :
    DuNumeric(value, byteSize, maxValue, minValue, defaultValue)
{
}
