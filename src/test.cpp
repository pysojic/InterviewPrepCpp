#include <iostream>
#include <print>

#include "Containers/Vector.hpp"

int main()
{
    Vector<int> v{1,2,3,4,5};

    for (size_t i{}; i < v.size(); ++i)
    {
        std::cout << v[i] << ',';
    }

    Vector v2(v);

    for (size_t i{}; i < v2.size(); ++i)
    {
        std::cout << v[i] << ',';
    }

    return 0;
}