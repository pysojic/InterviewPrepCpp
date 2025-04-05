#pragma once

#include <tuple>

// Function that returns the size of a tuple at compile time

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

// Function that appends the types of two tuple at compile time

template <typename First, typename Second>
struct Append;

template <typename... Ts, typename... Us>
struct Append<std::tuple<Ts...>, std::tuple<Us...>>
{
    using type = std::tuple<Ts..., Us...>;
};

// Function that calculates the dimension of an array at compile-time

template <typename T>
struct Dimension
{
    static constexpr size_t value = 0;
};

template <typename T>
struct Dimension<T[]>
{
    static constexpr size_t value = 1 + Dimension<T>::value;
};

template <typename T, size_t N>
struct Dimension<T[N]>
{
    static constexpr size_t value = 1 + Dimension<T>::value;
};