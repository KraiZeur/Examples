#include <iostream>
#include "Vector.h"

/**
 * Vector.h (Prototype)
 *   includes vector.inl (Templates code + inlines)
 */

int main(void)
{
    Vector<int> vec;
    vec.push_back(5);

    std::cout << vec.front() << std::endl;

    return 0;
}