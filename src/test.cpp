#include <iostream>
#include <vector>

#include "array.hpp"
#include "vector.hpp"

struct testClass
{
    testClass() = default;
    testClass(int a1, int a2) : a{a1}, b{a2} {}
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
    ~testClass() { std::cout << "destroy" << std::endl; }

    int a;
    int b;
};

int main()
{
    impl::Vector<testClass> vec;

    testClass test;
    vec.push_back(test);
    vec.push_back(testClass(1,2));
    vec.push_back(testClass(1,1));
    
}