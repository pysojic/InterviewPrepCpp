#include <iostream>
#include <print>

#include "Containers/Vector.hpp"
#include "SmartPointers/SharedPtr.hpp"

int main()
{
    pysojic::SharedPtr<int> sp;
    pysojic::SharedPtr<int> sp2 = pysojic::make_shared<int>(2);
    std::cout << sizeof(sp) << '\n';
    std::cout << sizeof(*(sp2.m_ControlBlock));
}