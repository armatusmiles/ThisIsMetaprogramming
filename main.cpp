#include "metastuff.hpp"

#include <iostream>
#include <cassert>

// C++ 11

void listTest()
{
    using NL = NumberList<1,2,3>;
    static_assert(NL::Head == 1, "");
    static_assert(NL::Tail::Head == 2, "");
    static_assert(NL::Tail::Tail::Head == 3, "");
}

void lengthTest()
{
    using NL = NumberList<1,2,3,4,5>;
    static_assert(ListLength<NL>::value == 5, "");
    using NL0 = NumberList<>;
    static_assert(ListLength<NL0>::value == 0, "");
}

void pushFronTest()
{
    using NL = NumberList<1,2,3>;
    using NL2 = PushFront<0, NL>::type;
    static_assert(ListLength<NL2>::value == 4, "");
    static_assert(NL2::Head == 0, "");
}

void concatListTest()
{
    using NL1 = NumberList<1,2>;
    using NL2 = NumberList<3,4>;
    using NL3 = ConcatList<NL1, NL2>::type;

    static_assert(ListLength<NL3>::value == 4, "");
    static_assert(NL3::Head == 1, "");
    static_assert(NL3::Tail::Head == 2, "");
    static_assert(NL3::Tail::Tail::Head == 3, "");
    static_assert(NL3::Tail::Tail::Tail::Head == 4, "");
}

void seqTest()
{
    using Seq = Sequence<2>::type;
    static_assert(ListLength<Seq>::value == 2, "");
    static_assert(Seq::Head == 0, "");
    static_assert(Seq::Tail::Head == 1, "");
}

void testFunctorAndTuple()
{
    auto func = [](int x, double y, double z, int w) { return x + y + z + w; };
    auto tup = std::make_tuple(5, 5.0, 1.5, 1);
    auto res = tupleApply(func, tup);
    assert(res == 12.5);

    auto func2 = [](std::string x, std::string y, std::string z) { return x + y + z; };
    auto tup2 = std::make_tuple("This ", "is ", "MetaProgramming");
    auto res2 = tupleApply(func2, tup2);
    assert(res2 == "This is MetaProgramming");
}


// meta function
template<int a, int b>
struct Minus
{
    static int const value = a - b;
};

void testTransform()
{
    using L4 = NumberList<4,3>;
    using L5 = NumberList<2,2>;

    using LL = Transform<L4, L5, Minus>::type;
    static_assert(LL::Head == 2, "");
    static_assert(LL::Tail::Head == 1, "");
}


int main()
{
    testFunctorAndTuple();
    using NL = NumberList<4,3>;
    printList<NL>(std::cout);
}
