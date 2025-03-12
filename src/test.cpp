#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <bit>

#include "Containers/Array.hpp"
#include "Containers/Vector.hpp"
#include "SmartPointers/SharedPtr.hpp"
#include "Containers/List.hpp"
#include "Containers/ForwardList.hpp"

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

int main()
{
    pysojic::List<int> l1{1,2,3,4,5,6,8};
    l1.print();
    l1.reverse();
    l1.print();
    l1.reverse();
    auto l2{l1};
    l1.merge(l2);
    l1.print();
    l2.print();
}