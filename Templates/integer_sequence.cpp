#include <iostream>
#include <tuple>

using std::tuple;

static void func(double d, float f, int i)
{
    std::cout << d << ", "
              << f << ", "
              << i << std::endl;
}

// The following code passes all parameters by
// value, for the sake of simplicity

template <typename F, typename TUP,
          int... INDICES>
static void tuple_call_(F f, TUP tup,
                        sequence<INDICES...>)
{
    f(std::get<INDICES>(tup)...);
}

template <typename F, typename... Ts>
static void tuple_call(F f, tuple<Ts...> tup)
{
    tuple_call_(f, tup,
                sequence_t<sizeof...(Ts)>{});
}

int main()
{
    func(1.0, 2.0, 3);

    tuple<double, float, int> tup{1.0, 2.0, 3};
    tuple_call(func, tup); // same effect
}