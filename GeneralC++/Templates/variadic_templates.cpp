#include <iostream>

template <typename T>
void variadic(T arg)
{
    std::cout << arg << std::endl;
}

// You have to add T, Args in order to differenciate between the upper function
template <typename T, typename... ARGS> // <int, int, int>
void variadic(T value, ARGS... args) // 5, 6, 7
{
    variadic(value);
    variadic(args...); // variadic(6,7) -> variadic(6, variadic(7)) will expand args
}

#ifdef NOT_DEF
template <typename T>
T square(T t) { return t * t; }

// Our base case just returns the value.
template <typename T>
double power_sum(T t) { return t; }

// Our new recursive case.
template <typename T, typename... Rest>
double power_sum(T t, Rest... rest)
{
    return t + power_sum(square(rest)...); // square(rest)... will be expanded as square(4), square(5), square(6)
}
#endif

int main(void)
{

    variadic(5, 6, 7);

    return 0;
}