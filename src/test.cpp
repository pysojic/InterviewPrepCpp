#include <iostream>
#include <vector>
#include <string>

#include "Array.hpp"
#include "Vector.hpp"

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
    // Vector<testClass> vec;
    // std::cout << vec.capacity() << std::endl;

    Vector<std::string> vecEmpty;
    vecEmpty.push_back("I");
    vecEmpty.push_back("need");
    vecEmpty.push_back("this");
    vecEmpty.push_back("to");
    vecEmpty.push_back("work");

    vecEmpty.print();
    std::string str{"Test"};

    Vector<std::string> vec(5, "Hello");
    vec.print();

    Vector<std::string> vec2(vec);
    vec2.print();
    vec2.push_back("Test");

    Vector<std::string> vec3(std::move(vec2));
    std::cout << "vec2 after move" << std::endl; vec2.print();
    std::cout << "vec3 after move3" << std::endl;
    vec3.print();

    vec2 = std::move(vec3);
    std::cout << "vec2 after move2" << std::endl; vec2.print();
    std::cout << "vec2 after move3" << std::endl; vec3.print();

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

    std::cout << "Capacity: " << vec.capacity() << " Size: " << vec.size() << std::endl;
    vec.shrink_to_fit();
    std::cout << "Capacity: " << vec.capacity() << " Size: " << vec.size() << std::endl;
    
    vec.resize(4);
    vec.print();
    std::cout << "Capacity: " << vec.capacity() << " Size: " << vec.size() << std::endl;
    vec.resize(10);
    vec.print();
    std::cout << "Capacity: " << vec.capacity() << " Size: " << vec.size() << std::endl;
    vec.resize(15);
    vec.print();
    std::cout << "Capacity: " << vec.capacity() << " Size: " << vec.size() << std::endl;

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