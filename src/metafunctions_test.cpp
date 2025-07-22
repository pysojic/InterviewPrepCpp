#include <iostream>
#include "Utilities/tests.hpp"

#include "Utilities/CompileTimeFunctions.hpp"

int main()
{
    static_assert(std::is_same_v<CompileTimeVector<1, 2>, CompileTimeVector<1,2>>);
    print(CompileTimeVector<1,2,3,4,5>{});
    static_assert(Dimension<int>::value == 0);
    static_assert(Dimension<int[]>::value == 1);
    static_assert(Dimension<int[2]>::value == 1);    
    static_assert(Dimension<int[3][4]>::value == 2);
    static_assert(Dimension<int[5][6][7]>::value == 3);
    static_assert(std::is_same_v<Prepend_t<1, CompileTimeVector<2,3>>, CompileTimeVector<1,2,3>>);
    static_assert(std::is_same_v< Append<4, CompileTimeVector<1,2,3>>::type , CompileTimeVector<1,2,3,4> >);
    static_assert(std::is_same_v< PopBack<CompileTimeVector<1,2,3,4>>::type , CompileTimeVector<1,2,3> >);
    static_assert(std::is_same_v<RemoveFirst<1, CompileTimeVector<1,1,2>>::type, CompileTimeVector<1,2>>);
    static_assert(std::is_same_v<RemoveFirst<2, CompileTimeVector<1,1,2>>::type, CompileTimeVector<1,1>>);
    static_assert(std::is_same_v<RemoveAll<9, CompileTimeVector<1,9,2,9,3,9>>::type, CompileTimeVector<1,2,3>>);
    static_assert(Length_v<CompileTimeVector<>> == 0);
    static_assert(Length_v<CompileTimeVector<1,2,3>> == 3);
    static_assert(Min<CompileTimeVector<3,1,2>>::value == 1);
    static_assert(Min<CompileTimeVector<1,2,3>>::value == 1);
    static_assert(Min<CompileTimeVector<3,2,1>>::value == 1);
    static_assert(std::is_same_v<Insert<5, CompileTimeVector<4,6>>::type, CompileTimeVector<4,5,6>>);
    static_assert(std::is_same_v<InsertionSort2<CompileTimeVector<2,4,3,1>>::type, CompileTimeVector<1,2,3,4>>);
    static_assert(std::is_same_v<InsertionSort2<CompileTimeVector<2,4,3,1,7,-8>>::type, CompileTimeVector<-8,1,2,3,4,7>>);
    static_assert(std::is_same_v<QuickSort2<CompileTimeVector<2,4,3,1,7,-8>>::type, CompileTimeVector<-8,1,2,3,4,7>>);
    static_assert(std::is_same_v<Concat<CompileTimeVector<1,2>, CompileTimeVector<3,4>, CompileTimeVector<5,6>>::type, CompileTimeVector<1,2,3,4,5,6>>);
    static_assert(std::is_same_v<Unique<CompileTimeVector<1,1,2,2,2,3,4>>::type, CompileTimeVector<1,2,3,4>>);
    static_assert(std::is_same_v<Set<2,1,3,1,2,3>::type, Set<1,2,3>::type>);
    static_assert(std::is_same_v<SetFrom<CompileTimeVector<2,1,3,1,2,3>>::type, Set<1,2,3>::type>);

    static_assert(BisectLeft<3, CompileTimeVector<0,1,2,3,4>>::value == 3);
    static_assert(BisectLeft<3, CompileTimeVector<0,1,2,4,5>>::value == 3);
    static_assert(BisectLeft<9, CompileTimeVector<0,1,2,4,5>>::value == 5);
    static_assert(BisectLeft<-1, CompileTimeVector<0,1,2,4,5>>::value == 0);
    static_assert(BisectLeft<2, CompileTimeVector<0,2,2,2,2,2>>::value == 1);
    static_assert(std::is_same_v<RLE<2,2,2,2>::type, CompileTimeVector<4,2>>);
    static_assert(std::is_same_v<RLE<2,2,2,2,3>::type, CompileTimeVector<4,2,1,3>>);
    static_assert(std::is_same_v<InsertAt<3, 0, CompileTimeVector<4,5,6>>::type, CompileTimeVector<3,4,5,6>>);
    static_assert(std::is_same_v<InsertAt<3, 1, CompileTimeVector<4,5,6>>::type, CompileTimeVector<4,3,5,6>>);
    static_assert(std::is_same_v<InsertAt<3, 2, CompileTimeVector<4,5,6>>::type, CompileTimeVector<4,5,3,6>>);
    static_assert(std::is_same_v<InsertAt<3, 3, CompileTimeVector<4,5,6>>::type, CompileTimeVector<4,5,6,3>>);
    static_assert(std::is_same_v<Zip<Multiply, CompileTimeVector<1,2,3>, CompileTimeVector<4,5,6>, CompileTimeVector<1,2,3>>::type, CompileTimeVector<4,20,54>>);
    static_assert(std::is_same_v<Zip<Plus, CompileTimeVector<1,2,3>, CompileTimeVector<4,5,6>, CompileTimeVector<1,2,3>>::type, CompileTimeVector<6,9,12>>);
    static_assert(std::is_same_v<Zip<Minus, CompileTimeVector<1,2,3>, CompileTimeVector<4,5,6>, CompileTimeVector<1,2,3>>::type, CompileTimeVector<-2,-1,-0>>); // This is equivalent to A - (B - C)

    std::cout << 1 << ": " << IsPrime<1>::value << '\n';
    std::cout << 2 << ": " << IsPrime<2>::value << '\n';
    std::cout << 3 << ": " << IsPrime<3>::value << '\n';
    std::cout << 4 << ": " << IsPrime<4>::value << '\n';
    std::cout << 5 << ": " << IsPrime<5>::value << '\n';
    std::cout << 6 << ": " << IsPrime<6>::value << '\n';
    std::cout << 7 << ": " << IsPrime<7>::value << '\n';

    std::cout << "Fibonacci" << 1 << ": " << Fibonacci<1>::value << '\n';
    std::cout << "Fibonacci" << 2 << ": " << Fibonacci<2>::value << '\n';
    std::cout << "Fibonacci" << 3 << ": " << Fibonacci<3>::value << '\n';
    std::cout << "Fibonacci" << 4 << ": " << Fibonacci<4>::value << '\n';
    std::cout << "Fibonacci" << 5 << ": " << Fibonacci<5>::value << '\n';
    std::cout << "Fibonacci" << 6 << ": " << Fibonacci<6>::value << '\n';
    std::cout << "Fibonacci" << 40 << ": " << Fibonacci<40>::value << '\n';

    print(Vect<1,2,3,4>{});
}