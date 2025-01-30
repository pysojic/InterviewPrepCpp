// g++-14 -std=c++20 -I include -g -Wall -Wextra -Wpedantic src/*.cpp -o out

#include <iostream>

template <typename T, std::size_t Size>
class Array
{
public:
    constexpr Array() : m_arr{} {}
    constexpr Array(const T& value)
    { 
        for (std::size_t i = 0; i < Size; ++i)
        {
            m_arr[i] = value;
        }
    }
   constexpr  Array(const Array& other)
    {            
        for (std::size_t i = 0; i < Size; ++i)
        {
            m_arr[i] = other[i];
        }
    }
    constexpr Array& operator= (const Array& other)
    {
        if (this != &other)
        {
            for (std::size_t i = 0; i < Size; ++i)
            {
                m_arr[i] = other[i];
            }
        }

        return *this;
    }
    constexpr Array(Array&& other)
    {
        for (std::size_t i = 0; i < Size; ++i)
        {
            m_arr[i] = std::move(other[i]);
        }
    }
    constexpr Array& operator= (Array&& other)
    {
        if (this != &other)
        {
            for (std::size_t i = 0; i < Size; ++i)
            {
                m_arr[i] = std::move(other[i]);
            }
        }

        return *this;
    }
    ~Array() noexcept = default; // explain why destrcutors must be noexcept

    constexpr const T& back() const noexcept { return m_arr[Size - 1]; }
    constexpr const T& front() const noexcept { return m_arr[0]; }
    constexpr const T& operator[](std::size_t index) const noexcept { return m_arr[index]; }
    constexpr T* data() noexcept { return m_arr; }
    [[nodiscard]] constexpr bool empty() noexcept { return Size == 0;}
    constexpr T& operator[](std::size_t index) noexcept { return m_arr[index]; }
    constexpr std::size_t size() const noexcept { return Size; }

private:
    T m_arr[Size];
};

