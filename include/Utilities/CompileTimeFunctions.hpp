#pragma once

#include <tuple>
#include <type_traits>

template<int N, int D, bool stop = (D*D > N)>
struct IsPrimeUtil;

template<int N, int D>
struct IsPrimeUtil<N, D, true> 
{
    static constexpr bool value = true;
};

template<int N, int D>
struct IsPrimeUtil<N, D, false> 
{
    static constexpr bool divisible = (N % D == 0);
    static constexpr bool value = divisible ? false : IsPrimeUtil< N, D+2 >::value;
};

template<int N>
struct IsPrime 
{
    static constexpr bool value = (N < 2) ? false : (N % 2 == 0) ? false : IsPrimeUtil<N,3>::value;
};

template<> 
struct IsPrime<2> 
{ 
    static constexpr bool value = true; 
};
template<> 
struct IsPrime<3> 
{ 
    static constexpr bool value = true; 
};


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
static constexpr std::size_t Length_v = Length<Seq>::value;

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

//---------- Concat ----------

template<typename... Vecs>
struct Concat;

template<template <int...> class Vec1, int... Args1>
struct Concat<Vec1<Args1...>>
{
    using type = Vec1<Args1...>;
};

template<template <int...> class Vec1, int... Args1, template<int...> class Vec2, int... Args2>
struct Concat<Vec1<Args1...>, Vec2<Args2...>>
{
    using type = Vec1<Args1..., Args2...>;
};

template<typename Vec1, typename Vec2, typename... Rest>
struct Concat< Vec1, Vec2, Rest... > 
{
  using merged12 = typename Concat<Vec1, Vec2>::type;
  using type     = typename Concat< merged12, Rest... >::type;
};

//---------- Partition ----------

template <typename Seq, int Pivot>
struct Partition;

template <template<int...> class Seq, int Pivot>
struct Partition<Seq<>, Pivot>
{
    using less = Seq<>;
    using ge = Seq<>;
};

template <template<int...> class Seq, int Head, int... Tail, int Pivot>
struct Partition<Seq<Head, Tail...>, Pivot>
{
    static constexpr bool comp = (Head < Pivot);
    using part = Partition<Seq<Tail...>, Pivot>;

    using less = std::conditional_t<comp, typename Concat<Seq<Head>, typename part::less>::type, typename part::less>;
    using ge = std::conditional_t<comp, typename part::ge, typename Concat<Seq<Head>, typename part::ge>::type>; 

};

//---------- QuickSort ----------

template<typename Seq>
struct QuickSort;

template <template<int...> class Seq>
struct QuickSort<Seq<>>
{
    using type = Seq<>;
};

template <template<int...> class Seq, int Head>
struct QuickSort<Seq<Head>>
{
    using type = Seq<Head>;
};

template <template<int...> class Seq, int Pivot, int... Tail>
struct QuickSort<Seq<Pivot, Tail...>>
{
    using parts = Partition<Seq<Tail...>, Pivot>;
    using sorted_less = typename QuickSort< typename parts::less >::type;
    using sorted_ge = typename QuickSort< typename parts::ge>::type;
    using type = typename Concat<sorted_less, Seq<Pivot>, sorted_ge>::type;
};

//---------- Unique ----------

template <typename Seq>
struct Unique;

template <template<int...> class Seq>
struct Unique<Seq<>>
{
    using type = Seq<>;
};

template <template<int...> class Seq, int Head>
struct Unique< Seq<Head>> {
    using type = Seq<Head>;
};
template <template<int...> class Seq, int Head, int... Tail>
struct Unique<Seq<Head, Head, Tail...>>
{
    using type = typename Unique<Seq<Head, Tail...>>::type;
};

template <template<int...> class Seq, int Head1, int Head2, int... Tail>
struct Unique<Seq<Head1, Head2, Tail...>>
{
    using TailUnique = typename Unique< Seq<Head2, Tail...> >::type;
    using type = Prepend_t< Head1, TailUnique >;
};

//---------- Set ----------
template <int... Elem>
struct Set
{
    using SortedSet = typename InsertionSort<Set<Elem...>>::type;
    using type = typename Unique<SortedSet>::type;
};

template <typename Seq>
struct SetFrom;

template <template<int...> class Seq, int... Args>
struct SetFrom<Seq<Args...>>
{
    using type = typename Set<Args...>::type;
};

//---------- Get ----------

template<int N, typename Seq>
struct GetValue;

template<template<int...> class Seq, int Head, int... Tail>
struct GetValue<0, Seq<Head, Tail...>>
{
    static constexpr int value = Head;
};

template<int N, template<int...> class Seq, int Head, int... Tail>
struct GetValue<N, Seq<Head, Tail...>>
{
    static_assert(N > 0, "Error: N should be a positive integer");
    static_assert(N <= sizeof...(Tail),
                  "Get<N,Seq>: index out of range");
    static constexpr int value = GetValue<N - 1, Seq<Tail...>>::value;
};

template<int N, typename Seq>
struct GetIndex;

template<int N, template<int...> class Seq>
struct GetIndex<N, Seq<>>
{
    static constexpr int value = -1;
};

template<int N, template<int...> class Seq, int... Tail>
struct GetIndex<N, Seq<N, Tail...>>
{
    static constexpr int value = 0;
};

template<int N, template<int...> class Seq, int Head, int... Tail>
struct GetIndex<N, Seq<Head, Tail...>>
{
    static constexpr int sub = GetIndex<N, Seq<Tail...>>::value;
    static constexpr int value = (sub < 0 ? -1 : (sub + 1));
};

//---------- BisectLeft ----------

template<int L, int R, int N, template<int...> class Seq, int... T>
static constexpr int bisectLeftLR(Seq<T...> haystack)
{
    if constexpr (L >= R)
    {
        return L;
    }
    else
    {
        constexpr int Mid = (L + R) / 2;
        if constexpr (GetValue<Mid, decltype(haystack)>::value < N)
        {
            return bisectLeftLR<Mid+1, R, N>(haystack);
        }
        else
        {
            return bisectLeftLR<L, Mid, N>(haystack);
        }
    }
};

template<int N, typename Seq>
struct BisectLeft;

template<int N, template<int...> class Seq, int... T>
struct BisectLeft<N, Seq<T...>>
{
    static constexpr int value = bisectLeftLR<0, sizeof...(T), N>(Seq<T...>{});
};

//---------- Run Length Encoding ----------

template<int...>
struct RLE;

template<>
struct RLE<>
{
    using type = CompileTimeVector<>;
};

template<int H, int... T>
struct RLE<H, T...>
{
    template<typename Acc, int Current, int CurrentCount, int... List>
    struct Helper;

    template<typename Acc, int Current, int CurrentCount>
    struct Helper<Acc, Current, CurrentCount>
    {
        using type = Concat<Acc, CompileTimeVector<CurrentCount, Current>>::type;
    };

    template<typename Acc, int Current, int CurrentCount, int Head, int... Tail>
    struct Helper<Acc, Current, CurrentCount, Head, Tail...>
    {
        using type = std::conditional_t<Current == Head,
                        Helper<Acc, Current, CurrentCount + 1, Tail...>,
                        Helper<typename Concat<Acc, CompileTimeVector<CurrentCount, Current>>::type, Head, 1, Tail...>
                     >::type;
    };

    using type = Helper<CompileTimeVector<>, H, 0, H, T...>::type;
};



