#include <iostream>
#include "array.hpp"

int main()
{
    Array<int, 8> a;
    Array<int, 8> a2(6);
    a2[0] = 2;

    std::cout << "back: " << a2.back() << std::endl; 
    std::cout << "front: " << a2.front() << std::endl;
    std::cout << "[]: " << a2[1] << std::endl;
    std::cout << a2.size() << std::endl;
}