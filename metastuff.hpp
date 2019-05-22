#ifndef METASTUFF_H
#define METASTUFF_H

#include <tuple>

// C++ 11

template <int ...Numbers>
struct NumberList;

template <int H, int... Numbers>
struct NumberList<H, Numbers...>
{
    static constexpr int Head = H;
    using Tail = NumberList<Numbers...>;
};

template <>
struct NumberList<> {};

template <typename NumberList>
struct ListLength
{
    static constexpr int value =
            1 + ListLength<typename NumberList::Tail>::value;
};

template <>
struct ListLength<NumberList<>>
{
    static constexpr int value = 0;
};

template <int H, typename Numbers>
struct PushFront;

template <int H, int... Numbers>
struct PushFront<H, NumberList<Numbers...>>
{
    using type = NumberList<H, Numbers...>;
};

template <typename L1, typename L2>
struct ConcatList;

template <int... Numbers1, int... Numbers2>
struct ConcatList<NumberList<Numbers1...>, NumberList<Numbers2...>>
{
    using type = NumberList<Numbers1..., Numbers2...>;
};

// Starts from 0
template <int N>
struct Sequence
{
    using type = typename ConcatList<typename Sequence<N-1>::type, NumberList<N-1>>::type;
};

template <>
struct Sequence<0>
{
    using type = NumberList<>;
};

template <typename F, typename Tup,  int ... ind>
auto tupleApply_(F f, Tup tup, NumberList<ind...>)
    -> decltype (f(std::get<ind>(tup)...))
{
    return f(std::get<ind>(tup)...);
}


template <typename F, typename... Args>
auto tupleApply(F f, std::tuple<Args...> t)
    -> decltype (tupleApply_(f, t, typename Sequence<sizeof... (Args)>::type{}))
{
    using seq = typename Sequence<sizeof... (Args)>::type;
    return tupleApply_(f, t, seq{});
}

template <typename L1, typename L2, template<int, int> class T>
struct Transform;

template <int... Numbers1, int... Numbers2, template<int, int> class T>
struct Transform<NumberList<Numbers1...>, NumberList<Numbers2...>,  T>
{
    using zCall = typename Transform<
        typename NumberList<Numbers1...>::Tail,
        typename NumberList<Numbers2...>::Tail, T>::type;

    using type = typename PushFront<T<NumberList<Numbers1...>::Head,
                                    NumberList<Numbers2...>::Head>::value,
                                    zCall>::type;
};

template <template<int, int> class T>
struct Transform<NumberList<>, NumberList<>,  T>
{
    using type = NumberList<>;
};

#endif // METASTUFF_H
