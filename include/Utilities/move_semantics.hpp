#pragma once

#include <type_traits>

namespace pysojic
{
    template <typename T>
    constexpr std::remove_reference_t<T>&& move(T&& obj) noexcept
    {
        return static_cast<std::remove_reference_t<T>&&>(obj);
    }

    template <typename T>
    constexpr T&& forward(std::remove_reference_t<T>& obj) noexcept
    {
        // Here, obj is an lvalue ref, which with ref collapsing becomes & && -> &
        return static_cast<T&&>(obj);
    }

    template <typename T>
    constexpr T&& forward(std::remove_reference_t<T>&& obj) noexcept
    {
        // Prevents the following kind of bad behaviour: forward<int&>(123) -> Cannot bind an rvalue ref to an lvalue ref
        static_assert(!std::is_lvalue_reference_v<T>, "Bad forward: cannot forward an rvalue as an lvalue");
        // Here, obj is an rvalue ref, which with ref collapsing becomes && && -> &&
        return static_cast<T&&>(obj);
    }
}