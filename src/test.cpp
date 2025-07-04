#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <bit>
#include <thread>
#include <chrono>

#include "Containers/Array.hpp"
#include "Containers/Vector.hpp"
#include "SmartPointers/SharedPtr.hpp"
#include "Containers/List.hpp"
#include "Containers/ForwardList.hpp"
#include "Concurrency/SpinLock.hpp"
#include "Containers/HashMap.hpp"
#include "Containers/OpenAddressingHashMap.hpp"
#include "Utilities/Any.hpp"
#include "Utilities/CompileTimeFunctions.hpp"

int main()
{
    static_assert(std::is_same_v<CompileTimeVector<1, 2>, CompileTimeVector<1,2>>);
    print(CompileTimeVector<1,2,3,4,5>{});
    static_assert(std::is_same_v<Prepend_t<1, CompileTimeVector<2,3>>, CompileTimeVector<1,2,3>>);
    static_assert(std::is_same_v< Append<4, CompileTimeVector<1,2,3>>::type , CompileTimeVector<1,2,3,4> >);
    static_assert(std::is_same_v< PopBack<CompileTimeVector<1,2,3,4>>::type , CompileTimeVector<1,2,3> >);
    static_assert(std::is_same_v<RemoveFirst<1, CompileTimeVector<1,1,2>>::type, CompileTimeVector<1,2>>);
    static_assert(std::is_same_v<RemoveFirst<2, CompileTimeVector<1,1,2>>::type, CompileTimeVector<1,1>>);
    static_assert(std::is_same_v<RemoveAll<9, CompileTimeVector<1,9,2,9,3,9>>::type, CompileTimeVector<1,2,3>>);
    static_assert(Length_t<CompileTimeVector<>> == 0);
    static_assert(Length_t<CompileTimeVector<1,2,3>> == 3);
    static_assert(Min<CompileTimeVector<3,1,2>>::value == 1);
    static_assert(Min<CompileTimeVector<1,2,3>>::value == 1);
    static_assert(Min<CompileTimeVector<3,2,1>>::value == 1);
    static_assert(std::is_same_v<Insert<5, CompileTimeVector<4,6>>::type, CompileTimeVector<4,5,6>>);
    static_assert(std::is_same_v<InsertionSort<CompileTimeVector<2,4,3,1>>::type, CompileTimeVector<1,2,3,4>>);
    static_assert(std::is_same_v<AppendVector<CompileTimeVector<1,2>, CompileTimeVector<3,4>>::type, CompileTimeVector<1,2,3,4>>);
    static_assert(std::is_same_v<Unique<CompileTimeVector<1,1,2,2,2,3,4>>::type, CompileTimeVector<1,2,3,4>>);

    std::cout << Dimension<int>::value << std::endl;
}