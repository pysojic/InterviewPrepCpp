#pragma once

#include <tuple>
#include <type_traits>
#include <iostream>

//
// Compile-time primality test for an integer N.
// Usage: IsPrime<17>::value is true, IsPrime<18>::value is false.
//

template<int N>
struct IsPrime 
{
    // Helper that tests divisibility of T by odd divisors starting at D.
    template<int T, int D>
    struct IsPrimeUtil;

    // Base case: if D*D > T, no divisors were found → T is prime.
    template<int T, int D>
    requires(D*D > T)
    struct IsPrimeUtil<T, D> 
    {
        static constexpr bool value = true;
    };

    // Recursive case: check divisibility by D, then continue with D+2.
    template<int T, int D>
    requires(D*D <= T)
    struct IsPrimeUtil<T, D> 
    {
        static constexpr bool divisible = (T % D == 0);
        static constexpr bool value = divisible ? false : IsPrimeUtil< T, D+2 >::value;
    };

    // Entry point: handle small values and even numbers, then start at divisor 3.
    static constexpr bool value = (N < 2) ? false : (N % 2 == 0) ? false : IsPrimeUtil<N,3>::value;
};

// Explicit specializations for small primes.
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

//
// LengthTuple<TList>::value
// Compile-time length of a std::tuple<...>.
//

template <typename TList>
struct LengthTuple;

// Empty tuple has length 0.
template<>
struct LengthTuple<std::tuple<>>
{
    static constexpr size_t value = 0;
};

// Recursively strip the head type and count.
template <typename T, typename... Us>
struct LengthTuple<std::tuple<T, Us...> >
{
    static constexpr size_t value = 1 + LengthTuple<std::tuple<Us...>>::value;
};

//
// AppendTuple<First, Second>::type
// Concatenate the type lists of two std::tuple<...> types.
//

template <typename First, typename Second>
struct AppendTuple;

template <typename... Ts, typename... Us>
struct AppendTuple<std::tuple<Ts...>, std::tuple<Us...>>
{
    using type = std::tuple<Ts..., Us...>;
};

//
// Dimension<T>::value
// Compute the number of array dimensions of T.
//   Dimension<int>::value        == 0
//   Dimension<int[3]>::value     == 1
//   Dimension<int[3][4]>::value  == 2
//

template <typename T>
struct Dimension
{
    static constexpr size_t value = 0;
};

// One dimension for an unsized array type T[].
template <typename T>
struct Dimension<T[]>
{
    static constexpr size_t value = 1;
};

// Sized array adds one dimension plus dimensions of the element type.
template <typename T, size_t N>
struct Dimension<T[N]>
{
    static constexpr size_t value = 1 + Dimension<T>::value;
};

//
// CompileTimeVector<int...>
// Simple type wrapper for a sequence of integral values,
// used by all sequence-manipulation metafunctions below.
//

template <int... Args>
struct CompileTimeVector {};

//---------- Print ----------
//
// Compile-time “print” helpers for CompileTimeVector.
// These are constexpr functions but perform runtime I/O.
//

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
//
// Prepend<Pre, Seq<...>>::type
// Insert Pre at the beginning of an integer sequence.
//

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
//
// Append<Post, Seq<...>>::type
// Append Post at the end of an integer sequence.
//

template<int Post, typename Seq>
struct Append;

template <int Post, template<int...> class Seq, int... Args>
struct Append<Post, Seq<Args...>>
{
    using type = Seq<Args..., Post>;
};

//---------- PopBack ----------
//
// PopBack<Seq<...>>::type
// Remove the last element from an integer sequence.
//

template<typename Seq>
struct PopBack;

// Single-element sequence → becomes empty.
template<template<int...> class Seq, int T>
struct PopBack<Seq<T>>
{
    using type = Seq<>;
};

// General case: recursively pop from the tail and rebuild the head.
template<template<int...> class Seq, int First, int... Rest>
struct PopBack<Seq<First, Rest...>>
{
    using TailPopped = typename PopBack<Seq<Rest...>>::type;

    using type = typename Prepend<First, TailPopped>::type;
};

//---------- RemoveFirst ----------
//
// RemoveFirst<Value, Seq<...>>::type
// Remove the first occurrence of Value from the sequence.
//

template <int First, typename Seq>
struct RemoveFirst;

// If the first element matches, drop it.
template <template<int...> class Seq, int First, int... Tail>
struct RemoveFirst<First, Seq<First, Tail...>>
{
    using type = Seq<Tail...>;
};

// Otherwise, keep the head and recurse on the tail.
template <template<int...> class Seq, int H, int First, int... Tail>
struct RemoveFirst<First, Seq<H, Tail...>>
{
    using Rest = typename RemoveFirst<First, Seq<Tail...>>::type;
    using type = Prepend_t< H, Rest >;
};

//---------- RemoveAll ----------
//
// RemoveAll<Value, Seq<...>>::type
// Remove all occurrences of Value from the sequence.
//

template <int T, typename Seq>
struct RemoveAll;

// Empty sequence: nothing to remove.
template<int T, template<int...> class Seq>
struct RemoveAll<T, Seq<>>
{
    using type = Seq<>;
};

// Head matches → skip it and recurse on tail.
template <int T, template<int...> class Seq, int... Tail>
struct RemoveAll<T, Seq<T, Tail...>>
{
    using type = typename RemoveAll<T, Seq<Tail...>>::type;
};

// Head does not match → keep head and recurse on tail.
template <int T, template<int...> class Seq, int Head, int... Tail>
struct RemoveAll<T, Seq<Head, Tail...>>
{
    using Rest = typename RemoveAll<T, Seq<Tail...>>::type;
    using type = typename Prepend<Head, Rest>::type;
};

//---------- Length ----------
//
// Length<Seq<...>>::value
// Compile-time length of an integer sequence.
//

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
//
// Min<Seq<...>>::value
// Compute the minimum element of a non-empty sequence.
//

template <typename Seq>
struct Min;

// Single-element sequence: that element is the minimum.
template <template<int...> class Seq, int N>
struct Min<Seq<N>> 
{
    static constexpr int value = N;
};

// General case: compare head with min of tail.
template <template<int...> class Seq, int First, int Second, int... Rest>
struct Min<Seq<First, Second, Rest...>> 
{
    static constexpr int tail_min = Min<Seq<Second, Rest...>>::value;
    static constexpr int value = First < tail_min ? First : tail_min;
};

//---------- Insert ----------
//
// Insert<Value, Seq<...>>::type
// Insert Value into a sequence that is assumed to be sorted,
// preserving sorted order (used by insertion sort). 
//

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
//
// InsertionSort<Seq<...>>::type
// Classic insertion sort performed at compile time on an integer sequence.
//

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
//
// Concat<Seq1, Seq2, ...>::type
// Concatenate one or more integer sequences into a single sequence.
//

template<typename... Vecs>
struct Concat;

// Single sequence → unchanged.
template<template <int...> class Vec1, int... Args1>
struct Concat<Vec1<Args1...>>
{
    using type = Vec1<Args1...>;
};

// Concatenate two sequences.
template<template <int...> class Vec1, int... Args1, template<int...> class Vec2, int... Args2>
struct Concat<Vec1<Args1...>, Vec2<Args2...>>
{
    using type = Vec1<Args1..., Args2...>;
};

// Variadic case: fold concatenation over all sequences.
template<typename Vec1, typename Vec2, typename... Rest>
struct Concat< Vec1, Vec2, Rest... > 
{
  using merged12 = typename Concat<Vec1, Vec2>::type;
  using type     = typename Concat< merged12, Rest... >::type;
};

//---------- Partition ----------
//
// Partition<Seq<...>, Pivot>
// Split a sequence into two sequences:
//   less: elements < Pivot
//   ge:   elements >= Pivot
// Used by QuickSort.
//

template <typename Seq, int Pivot>
struct Partition;

// Empty sequence → both parts empty.
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
//
// QuickSort<Seq<...>>::type
// Compile-time quicksort over an integer sequence.
//

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

// General case: choose first element as pivot, partition, then sort parts.
template <template<int...> class Seq, int Pivot, int... Tail>
struct QuickSort<Seq<Pivot, Tail...>>
{
    using parts = Partition<Seq<Tail...>, Pivot>;
    using sorted_less = typename QuickSort< typename parts::less >::type;
    using sorted_ge = typename QuickSort< typename parts::ge>::type;
    using type = typename Concat<sorted_less, Seq<Pivot>, sorted_ge>::type;
};

//---------- Unique ----------
//
// Unique<Seq<...>>::type
// Remove consecutive duplicates from a sorted sequence.
//

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

// If the first two elements are the same, skip one.
template <template<int...> class Seq, int Head, int... Tail>
struct Unique<Seq<Head, Head, Tail...>>
{
    using type = typename Unique<Seq<Head, Tail...>>::type;
};

// Otherwise, keep head and recurse.
template <template<int...> class Seq, int Head1, int Head2, int... Tail>
struct Unique<Seq<Head1, Head2, Tail...>>
{
    using TailUnique = typename Unique< Seq<Head2, Tail...> >::type;
    using type = Prepend_t< Head1, TailUnique >;
};

//---------- Set ----------
//
// Set<Elem...>::type
// Build a sorted, deduplicated set of integers from Elem...
// by insertion-sorting and then applying Unique.
//

template <int... Elem>
struct Set
{
    using SortedSet = typename InsertionSort<Set<Elem...>>::type;
    using type = typename Unique<SortedSet>::type;
};

// SetFrom<Seq<...>>::type
// Convenience wrapper to build a Set from an existing sequence.
//

template <typename Seq>
struct SetFrom;

template <template<int...> class Seq, int... Args>
struct SetFrom<Seq<Args...>>
{
    using type = typename Set<Args...>::type;
};

//---------- Get ----------
//
// GetValue<N, Seq<...>>::value
// Retrieve the N-th element (0-based) from an integer sequence.
//

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

//
// GetIndex<Value, Seq<...>>::value
// Get the index (0-based) of the first occurrence of Value in Seq,
// or -1 if Value does not appear.
//

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
//
// bisectLeftLR<L, R, N>(Seq<...>)
// Internal helper: perform a compile-time binary search for the
// leftmost insertion position of N in a sorted sequence.
//
// BisectLeft<N, Seq<...>>::value
// Public interface that returns that insertion index.
//

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
//
// RLE<Values...>::type
// Run-length encode a sequence of integers into a CompileTimeVector
// of (count, value) pairs, e.g.
//   RLE<1,1,1,2,2,3>::type  =>  <3,1,2,2,1,3>.
//

template<int...>
struct RLE;

template<>
struct RLE<>
{
    using type = CompileTimeVector<>;
};

template<int Head, int... Tail>
struct RLE<Head, Tail...>
{
    // Helper accumulates runs as (count, value) pairs.
    template<typename AccumulatedRuns, int CurrentValue, int CurrentCount, int... List>
    struct Helper;

    // End of input: push the final run into the accumulator.
    template<typename AccumulatedRuns, int CurrentValue, int CurrentCount>
    struct Helper<AccumulatedRuns, CurrentValue, CurrentCount>
    {
        using type = Concat<AccumulatedRuns, CompileTimeVector<CurrentCount, CurrentValue>>::type;
    };

    // General case: extend current run or start a new one.
    template<typename AccumulatedRuns, int CurrentValue, int CurrentCount, int H, int... T>
    struct Helper<AccumulatedRuns, CurrentValue, CurrentCount, H, T...>
    {
        using type = std::conditional_t<CurrentValue == H,
                        Helper<AccumulatedRuns, CurrentValue, CurrentCount + 1, T...>,
                        Helper<typename Concat<AccumulatedRuns, CompileTimeVector<CurrentCount, CurrentValue>>::type, H, 1, T...>
                     >::type;
    };

    using type = Helper<CompileTimeVector<>, Head, 0, Head, Tail...>::type;
};

//---------- InsertAt ----------
//
// InsertAt<Value, Index, Seq<...>>::type
// Insert Value at position Index (0-based) in the sequence.
//

template<int Value, int Index, typename Seq>
struct InsertAt;

template<int Value, template<int...> class Seq, int... Tail>
struct InsertAt<Value, 0, Seq<Tail...>>
{
    using type = Prepend_t<Value, Seq<Tail...>>;
};

template<int Value, int Index, template<int...> class Seq, int Head, int... Tail>
requires(Index > 0)
struct InsertAt<Value, Index, Seq<Head, Tail...>>
{
    using type = Prepend_t<Head, typename InsertAt<Value, Index - 1, Seq<Tail...>>::type>;
};

//---------- Zip ----------
//
// Arithmetic functors used with Zip / Zip2.
//

template<int A, int B>
struct Plus 
{
    static constexpr int value = A + B;
};

template<int A, int B>
struct Minus 
{
    static constexpr int value = A - B;
};

template<int A, int B>
struct Multiply 
{
    static constexpr int value = A * B;
};

//
// Zip2<Op, Seq1<...>, Seq2<...>>::type
// Apply a binary metafunction Op element-wise to two sequences of
// equal length.
//
// Zip<Op, Seq1, Seq2, ...>::type
// Generalization to zipping more than two sequences.
//

template <template<int,int> class Op, typename Seq1, typename Seq2>
struct Zip2;

template<template<int,int> class Op, template<int...> class Seq1, int... Args1, template<int...> class Seq2, int... Args2>
struct Zip2<Op, Seq1<Args1...>, Seq2<Args2...>>
{
    using type = Seq1<(Op<Args1,Args2>::value)...>;
};

template<template<int,int> class Op, typename... Seqs>
struct Zip;

// Single sequence → unchanged.
template<template<int,int> class Op, typename Seq>
struct Zip<Op, Seq>
{
    using type = Seq;
};

// Variadic case: zip Seq1 with the zip of the remaining sequences.
template<template<int,int> class Op, typename Seq1, typename...Seqs>
struct Zip<Op, Seq1, Seqs...>
{
    using type = typename Zip2<Op, Seq1, typename Zip<Op, Seqs...>::type>::type;
};
