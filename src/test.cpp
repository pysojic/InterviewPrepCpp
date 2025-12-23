#include <iostream>
#include <print>

#include "Containers/Vector.hpp"
#include "SmartPointers/SharedPtr.hpp"
#include "SmartPointers/UniquePtr.hpp"

int main()
{
    pysojic::SharedPtr<std::string> sp;
    pysojic::SharedPtr<std::string> sp2 = pysojic::make_shared<std::string>("Hello the world");
    std::cout << *sp2 << std::endl;
    sp = std::move(sp2);
    {
    pysojic::SharedPtr<std::string> sp3 = sp;
    std::cout << *sp << "refcount:" << sp.use_count() <<  std::endl;
    }
    std::cout << *sp << "refcount:" << sp.use_count() <<  std::endl;

}