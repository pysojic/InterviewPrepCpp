#pragma once

#include <tuple>
#include <type_traits>

// Function that returns the size of a tuple at compile time

template <typename TList>
struct LengthTuple;

template<>
struct LengthTuple<std::tuple<>>
{
    static constexpr size_t value = 0;
};

template <typename T, typename... Us>
struct LengthTuple<std::tuple<T, Us...> >
{
    static constexpr size_t value = 1 + LengthTuple<std::tuple<Us...>>::value;
};

// Function that appends the types of two tuple at compile time

template <typename First, typename Second>
struct AppendTuple;

template <typename... Ts, typename... Us>
struct AppendTuple<std::tuple<Ts...>, std::tuple<Us...>>
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

template <int... Args>
struct CompileTimeVector {};

//---------- Print ----------

template<int Head>
constexpr void print(CompileTimeVector<Head>)
{
    std::cout << Head << std::endl;
}

template<int Head, int... Args>
constexpr void print(CompileTimeVector<Head, Args...> args)
{
    std::cout << Head << ' ';
    print(CompileTimeVector<Args...>{});
}

//---------- Prepend ----------

template<int Pre, typename Seq>
struct Prepend;

template <int Pre, template<int...> class Seq, int... Args>
struct Prepend<Pre, Seq<Args...>>
{
    using type = Seq<Pre, Args...>;
};

template<int Pre, typename... Args>
using Prepend_t = typename Prepend<Pre, Args...>::type;

//---------- Append ----------

template<int Post, typename Seq>
struct Append;

template <int Post, template<int...> class Seq, int... Args>
struct Append<Post, Seq<Args...>>
{
    using type = Seq<Args..., Post>;
};

//---------- PopBack ----------

template<typename Seq>
struct PopBack;

template<template<int...> class Seq, int T>
struct PopBack<Seq<T>>
{
    using type = Seq<>;
};

template<template<int...> class Seq, int First, int... Rest>
struct PopBack<Seq<First, Rest...>>
{
    using TailPopped = typename PopBack<Seq<Rest...>>::type;

    using type = typename Prepend<First, TailPopped>::type;
};

//---------- RemoveFirst ----------

template <int First, typename Seq>
struct RemoveFirst;

template <template<int...> class Seq, int First, int... Tail>
struct RemoveFirst<First, Seq<First, Tail...>>
{
    using type = Seq<Tail...>;
};

template <template<int...> class Seq, int H, int First, int... Tail>
struct RemoveFirst<First, Seq<H, Tail...>>
{
    using Rest = typename RemoveFirst<First, Seq<Tail...>>::type;
    using type = Prepend_t< H, Rest >;
};

//---------- RemoveAll ----------

template <int T, typename Seq>
struct RemoveAll;

template<int T, template<int...> class Seq>
struct RemoveAll<T, Seq<>>
{
    using type = Seq<>;
};

template <int T, template<int...> class Seq, int... Tail>
struct RemoveAll<T, Seq<T, Tail...>>
{
    using type = typename RemoveAll<T, Seq<Tail...>>::type;
};

template <int T, template<int...> class Seq, int Head, int... Tail>
struct RemoveAll<T, Seq<Head, Tail...>>
{
    using Rest = typename RemoveAll<T, Seq<Tail...>>::type;
    using type = typename Prepend<Head, Rest>::type;
};

//---------- Length ----------

template<typename Seq>
struct Length;

template<template<int...> class Seq>
struct Length<Seq<>>
{
    constexpr static size_t value = 0;
};

template<template<int...> class Seq, int Head, int... Tail>
struct Length<Seq<Head, Tail...>>
{
    constexpr static size_t value = 1 + Length<Seq<Tail...>>::value;
};

template<typename Seq>
static constexpr std::size_t Length_t = Length<Seq>::value;

//---------- Min ----------

template <typename Seq>
struct Min;

template <template<int...> class Seq, int N>
struct Min<Seq<N>> {
    static constexpr int value = N;
};


template <template<int...> class Seq, int First, int Second, int... Rest>
struct Min<Seq<First, Second, Rest...>> 
{
    // Compute the minimum of the tail (Second, Rest...)
    static constexpr int tail_min = Min<Seq<Second, Rest...>>::value;
    // The overall minimum is the lesser of First and tail_min
    static constexpr int value = First < tail_min ? First : tail_min;
};

//---------- Insert ----------

template<int Value, typename Seq>
struct Insert;

template<int Value, template<int...> class Seq>
struct Insert<Value, Seq<>>
{
    using type = Seq<Value>;
};

template<int Value, template<int...> class Seq, int Head, int... Tail>
struct Insert<Value, Seq<Head, Tail...>>
{
    static constexpr bool comp = (Value <= Head);
    using TailInserted = typename Insert<Value, Seq<Tail...>>::type;

    using type = std::conditional_t<comp,
                Seq<Value, Head, Tail...>,
                typename Prepend<Head, TailInserted>::type>;
};

//---------- InsertionSort ----------

template <typename Seq>
struct InsertionSort;

template <template<int...> class Seq>
struct InsertionSort<Seq<>>
{
    using type = Seq<>;
};

template <template<int...> class Seq, int Head>
struct InsertionSort<Seq<Head>>
{
    using type = Seq<Head>;
};

template <template<int...> class Seq, int Head1, int Head2, int... Tail>
struct InsertionSort<Seq<Head1, Head2, Tail...>>
{
    using TailSorted = typename InsertionSort<Seq<Head2, Tail...>>::type;

    using type = typename Insert<Head1, TailSorted>::type;
};