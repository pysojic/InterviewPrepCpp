#pragma once

template <int... Elems>
struct Vect {};

template<int D, int N>
struct isPrimeUtil2;

template<int D, int N>
requires(D*D > N)
struct isPrimeUtil2<D, N>
{
    static constexpr bool value = true;
};

template<int D, int N>
requires(D*D <= N)
struct isPrimeUtil2<D,N>
{
    static constexpr bool divisible = N % D == 0;
    static constexpr bool value = divisible ? false : isPrimeUtil2<D + 2, N>::value;
};

template<int N>
struct isPrime2
{
    static constexpr bool value = (N % 2 == 0) ? false : isPrimeUtil2<3, N>::value;
};

template<>
struct isPrime2<2>
{
    static constexpr bool value = true;
};

template<>
struct isPrime2<3>
{
    static constexpr bool value = true;
};

template<int N>
struct Fibonacci
{
    static constexpr int value = Fibonacci<N-1>::value + Fibonacci<N-2>::value;
};

template<>
struct Fibonacci<0>
{
    static constexpr int value = 0;
};

template<>
struct Fibonacci<1>
{
    static constexpr int value = 1;
};


template <int T>
constexpr void print(Vect<T> a)
{
    std::cout << T;
}

template <int T, int... Ts>
constexpr void print(Vect<T, Ts...> a)
{
    std::cout << T << ", ";
    print(Vect<Ts...>{});
}

template <int Value, typename Vec>
struct Prepend2;

template <int Value, template<int...> class Seq>
struct Prepend2<Value, Seq<>>
{
    using type = Seq<Value>;
};

template <int Value, template<int...> class Seq, int... Rest>
struct Prepend2<Value, Seq<Rest...>>
{
    using type = Seq<Value, Rest...>;
};

template<int Value, typename Seq>
using Prepend2_t = typename Prepend2<Value, Seq>::type;

template<int Value, typename Seq>
struct Append2;

template<int Value, template<int...> class Seq>
struct Append2<Value, Seq<>>
{
    using type = Seq<Value>;
};

template<int Value, template<int...> class Seq, int Head, int... Args>
struct Append2<Value, Seq<Head, Args...>>
{
    using type = Prepend2_t<Head, typename Append2<Value, Seq<Args...>>::type>;
};

template<typename Seq>
struct PopBack2;

template<template<int...> class Seq, int Head>
struct PopBack2<Seq<Head>>
{
    using type = Seq<>;
};

template<template<int...> class Seq, int Head, int... Rest>
struct PopBack2<Seq<Head, Rest...>>
{
    using type = Prepend2_t<Head, typename PopBack2<Seq<Rest...>>::type>;
};

template<int Value, typename Seq>
struct RemoveAll2;

template<int Value, template<int...> class Seq>
struct RemoveAll2<Value, Seq<>>
{
    using type = Seq<>;
};

template<int Value, template<int...> class Seq, int... Rest>
struct RemoveAll2<Value, Seq<Value, Rest...>>
{
    using type = typename RemoveAll2<Value, Seq<Rest...>>::type;
};

template<int Value, template<int...> class Seq, int... Rest, int Head>
struct RemoveAll2<Value, Seq<Head, Rest...>>
{
    using type = Prepend2_t<Head, typename RemoveAll2<Value, Seq<Rest...>>::type>;
};

template<typename Seq>
struct Length2
{
    static constexpr int value = 0;
};

template<template<int...> class Seq, int Head>
struct Length2<Seq<Head>>
{
    static constexpr int value = 1;
};

template<template<int...> class Seq, int Head, int... Rest>
struct Length2<Seq<Head, Rest...>>
{
    static constexpr int value = 1 + Length2<Seq<Rest...>>::value;
};

template<typename Seq>
static constexpr int Length2_v = Length2<Seq>::value;

template<typename Seq>
struct Min2;

template<template<int...> class Seq, int Head>
struct Min2<Seq<Head>>
{
    static constexpr int value = Head;
};

template<template<int...> class Seq, int Head, int... Rest>
struct Min2<Seq<Head,Rest...>>
{
    static constexpr int TailMin = Min2<Seq<Rest...>>::value;
    static constexpr int value = Head <= TailMin ? Head : TailMin;
};

template <int Value, typename Seq>
struct Insert2;

template<int Value, template<int...> class Seq>
struct Insert2<Value, Seq<>>
{
    using type = Seq<Value>;
};

template<int Value, template<int...> class Seq, int Head, int... Rest>
struct Insert2<Value, Seq<Head, Rest...>>
{
    static constexpr bool comp = Value <= Head;
    using type = std::conditional_t<comp,
        Seq<Value, Head, Rest...>, 
        Prepend2_t<Head, typename Insert2<Value, Seq<Rest...>>::type>>;
};

template<typename Seq>
struct InsertionSort2;

template<template<int...> class Seq, int Head>
struct InsertionSort2<Seq<Head>>
{
    using type = Seq<Head>;
};

template<template<int...> class Seq, int Head, int... Args>
struct InsertionSort2<Seq<Head,Args...>>
{
    using type = typename Insert2<Head, typename InsertionSort2<Seq<Args...>>::type>::type;
};

template<typename... Seqs>
struct Concat2;

template<template<int...> class Seq1, int... Args1>
struct Concat2<Seq1<Args1...>>
{
    using type = Seq1<Args1...>;
};

template<template<int...> class Seq1, int... Args1, template<int...> class Seq2, int... Args2>
struct Concat2<Seq1<Args1...>, Seq2<Args2...>>
{
    using type = Seq1<Args1..., Args2...>;
};

template<typename Seq1, typename Seq2, typename... Rest>
struct Concat2<Seq1, Seq2, Rest...>
{
    using type = typename Concat2<typename Concat2<Seq1, Seq2>::type, Rest...>::type;
};

template <int Pivot, typename Seq>
struct Partition2;

template<int Pivot, template<int...> class Seq>
struct Partition2<Pivot, Seq<>>
{
    using lower = Seq<>;
    using ge = Seq<>;
};

template<int Pivot, template<int...> class Seq, int Head, int... Rest>
struct Partition2<Pivot, Seq<Head, Rest...>>
{
    static constexpr bool comp = Head < Pivot;
    using parts = Partition2<Pivot, Seq<Rest...>>;
    using lower = std::conditional_t<comp, Prepend2_t<Head, typename parts::lower>, typename parts::lower>;
    using ge = std::conditional_t<comp, typename parts::ge, Prepend2_t<Head, typename parts::ge>>;
};

template<typename Seq>
struct QuickSort2;

template<template<int...> class Seq>
struct QuickSort2<Seq<>>
{
    using type = Seq<>;
};

template<template<int...> class Seq, int Head>
struct QuickSort2<Seq<Head>>
{
    using type = Seq<Head>;
};

template<template<int...> class Seq, int Head, int... Rest>
struct QuickSort2<Seq<Head, Rest...>>
{
    using lower = typename QuickSort2<typename Partition2<Head, Seq<Rest...>>::lower>::type;
    using ge = typename QuickSort2<typename Partition2<Head, Seq<Rest...>>::ge>::type;
    using type = typename Concat2<lower, Seq<Head>, ge>::type;
};

template <typename Seq>
struct Unique2;

template <template<int...> class Seq>
struct Unique2<Seq<>>
{
    using type = Seq<>;
};

template <template<int...> class Seq, int Head>
struct Unique2<Seq<Head>>
{
    using type = Seq<Head>;
};

template <template<int...> class Seq, int Head, int... Rest>
struct Unique2<Seq<Head, Head, Rest...>>
{
    using type = typename Unique2<Seq<Head, Rest...>>::type;
};

template <template<int...> class Seq, int Head1, int Head2, int... Rest>
struct Unique2<Seq<Head1, Head2, Rest...>>
{
    using type = Prepend2_t<Head1, typename Unique2<Seq<Head2, Rest...>>::type>;
};