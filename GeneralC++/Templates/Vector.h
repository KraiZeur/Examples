#ifndef __VECTOR_H__
#define __VECTOR_H__

template<typename T>
class Vector
{
private:
    T _value;
public:
    Vector();
    void push_back(const T& value);
    T& front();
}

#include "Vector.inl"

#endif /* __VECTOR_H__ */