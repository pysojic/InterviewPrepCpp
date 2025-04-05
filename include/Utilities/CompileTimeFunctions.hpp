#pragma once

#include <tuple>

// Function that returns the size of a tuple at compile time

using CharTypes = std::tuple<char16_t, char32_t, u_int8_t>;

template <typename TList>
struct Length;

template<>
struct Length<std::tuple<>>
{
    static constexpr size_t value = 0;
};

template <typename T, typename... Us>
struct Length<std::tuple<T, Us...> >
{
    static constexpr size_t value = 1 + Length<std::tuple<Us...>>::value;
};