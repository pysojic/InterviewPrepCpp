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

struct testClass
{
    testClass() { std::cout << "default" << std::endl; };
    testClass(int a1, int a2) : a{a1}, b{a2} { std::cout << "construct with params" << std::endl; }
    testClass(const testClass& other) : a{other.a}, b{other.b} { std::cout << "copy" << std::endl;}
    testClass(testClass&& other) : a{other.a}, b{other.b} { std::cout << "move" << std::endl;}
    testClass& operator=(const testClass& other)
    {
        if (this != &other)
        {
            a = other.a;
            b = other.b;
        }

        std::cout << "copy ass" << std::endl;

        return *this;
    }
    testClass& operator=(testClass&& other)
    {
        if (this != &other)
        {
            a = std::move(other.a);
            b = std::move(other.b);
        }

         std::cout << "move ass" << std::endl;

        return *this;
    }
    ~testClass() { std::cout << "destroy test (" << a << ',' << b << ')' << std::endl; }

    friend std::ostream& operator << (std::ostream& os, const testClass& test)
    {
        os << '[' << test.a << ',' << test.b << ']';
        return os;
    }

    void print() const { std::cout << '[' << a << ',' << b << ']' << std::endl; }

    int a;
    int b;
};

SpinLock sp;

void test()
{
    sp.lock();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    sp.unlock();
}

int main()
{
    pysojic::Any test(2);

    std::cout << test << std::endl;

}