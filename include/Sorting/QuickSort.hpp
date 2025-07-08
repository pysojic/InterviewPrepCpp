#pragma once

#include <tuple>

template<size_t Pivot, typename Array>
std::tuple<Array, Array> partition(Pivot pivot, Array& array)
{
    if (array.empty())
        return Array{};
    auto comp = [] { return array[0] < pivot; };
    auto lower = comp ? partition()
}

template<typename Array>
Array quick_sort(Array& arr)
{

}