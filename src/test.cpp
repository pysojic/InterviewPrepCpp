#include <iostream>
#include <print>

#include "Containers/Vector.hpp"
#include "SmartPointers/SharedPtr.hpp"
#include "SmartPointers/UniquePtr.hpp"

int main()
{
    pysojic::SharedPtr<int> sp;
    pysojic::SharedPtr<int> sp2 = pysojic::make_shared<int>(2);
    pysojic::UniquePtr<int> up;
    std::cout << sizeof(up);
}