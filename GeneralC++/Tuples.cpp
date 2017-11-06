#include <tuple>
#include <array>
#include <utility>
#include <iostream>

// http://www.pdimov.com/cpp2/simple_cxx11_metaprogramming.html

template <typename T>
class MyTemplate {
private:
    T _t;
public:
    MyTemplate()
    {

    }
    MyTemplate(T t) : _t(t)
    {

    }
};

template <typename T>
void display(T data)
{
    std::cout << "ok" << std::endl;
    // [](const auto& data)
    // {
    //     //std::cout << "* " << member.getName() << '\n';
    // }
}

template <typename F, typename... Args>
void for_each_arg(F&& f, Args&&... args)
{
    display(std::forward<Args>(args)...);
}

template <typename Tuple, size_t... I>
void loop_for_each(Tuple&& t, std::index_sequence<I...>)
{
    for_each_arg(std::get<I>(std::forward<Tuple>(t))...);
}

int main(void) {
    std::tuple<MyTemplate<int>, MyTemplate<char>> myTuple;

    auto myAutoTuple = std::make_tuple(MyTemplate<int>(1), MyTemplate<char>('c'));

    auto res = tuple_cat(myAutoTuple, myTuple);

    auto tpl = std::get<0>(myTuple);

    int x = 50;
    decltype(x) y = 50;

    // constexpr size_t tupleSize = std::tuple_size<std::decay_t<decltype(myAutoTuple)>>::value;

    constexpr size_t tupleSize = std::tuple_size< decltype(myAutoTuple) >::value;
    auto sequence = std::make_index_sequence<tupleSize>(); // (since C++14)

    loop_for_each(myAutoTuple, sequence);

    std::cout << tupleSize << std::endl;
}
