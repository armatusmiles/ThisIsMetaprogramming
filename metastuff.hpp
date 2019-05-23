/**
 * Copyright (c) 2019 Anatolii Kurotych
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef METASTUFF_H
#define METASTUFF_H

#include <tuple>
#include <ostream>

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

template <typename NumberList>
void printList(std::ostream& os)
{
    os << NumberList::Head << ' ';
    printList<typename NumberList::Tail>(os);
}

template <>
void printList<NumberList<>>(std::ostream& os){ os << '\n'; } // for exit from recursive

#endif // METASTUFF_H
