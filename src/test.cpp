#include <iostream>
#include <vector>
#include <string>

#include "array.hpp"
#include "vector.hpp"

struct testClass
{
    testClass() { std::cout << "default" << std::endl; };
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
    ~testClass() { std::cout << "destroy test (" << a << ',' << b << ')' << std::endl; }

    friend std::ostream& operator << (std::ostream& os, const testClass& test)
    {
        os << test.a << ',' << test.b;
        return os;
    }

    void print() const { std::cout << a << ',' << b << std::endl; }

    int a;
    int b;
};

int main()
{
    // impl::Vector<testClass> vec;
    // std::cout << vec.capacity() << std::endl;

    impl::Vector<std::string> vec;
    std::string str("I will get this");

    vec.push_back("Hello");
    vec.push_back("The");
    vec.push_back("World");
    vec.push_back("!");
    vec.pop_back();
    vec.push_back("I");
    vec.push_back("am");
    vec.push_back("Pierre");
    vec.pop_back();
    vec.emplace_back(str.begin(), str.end());

    vec.print();

    vec.clear();

    // testClass test(7,9);
    // vec.print();
    // vec.push_back(test);
    // std::cout << vec.capacity() << std::endl;
    // vec.push_back(testClass(1,2));
    // vec.push_back(testClass(1,1));
    // vec.print();
    // vec.emplace_back(7,9);
    // std::cout << vec.capacity() << std::endl;
    // vec.print();
    // std::cout << vec.size() << std::endl;
    // vec.pop_back();
    // vec.print();
    // vec.clear();
    // vec.print();
    // vec.print();
    // vec.pop_back();
    // vec.emplace_back(3, 4);
    // vec.clear();
    
}