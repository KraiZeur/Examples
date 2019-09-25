#include "Vector.h"

Vector::Vector()
{
}

void Vector::push_back(const T &value)
{
    _value=value;
}

T& Vector::front()
{
    return _value;
}
