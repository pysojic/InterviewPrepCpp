#pragma once

namespace impl
{
    template<typename T>
    class Vector
    {
    public:
        Vector() : m_Arr{nullptr}, m_Capacity{}, m_Size{}
        {
            re_alloc(2);
        }
        Vector(size_t size) : m_Capacity{size}, m_Size{size}
        {
            re_alloc(m_Capacity);
        }
        ~Vector() noexcept
        {
            clear();
            ::operator delete(m_Arr, m_Capacity * sizeof(T));
        }
        
        void clear()
        {
            for (size_t i = 0; i < m_Size; ++i)
            {
                m_Arr[i].~T();
            }
            m_Size = 0;
        }
        void push_back(const T& obj)
        {
            if (m_Size >= m_Capacity)
                re_alloc(m_Capacity + (m_Capacity >> 1));
            
            m_Arr[m_Size++] = obj;
        }
        void push_back(T&& obj)
        {
            if (m_Size >= m_Capacity)
                re_alloc(m_Capacity + (m_Capacity >> 1));
            
            m_Arr[m_Size++] = std::move(obj);
        }
        void pop_back()
        {
            if (m_Size > 0)
            {
                m_Arr[m_Size - 1].~T();
                --m_Size;
            }
        }
        template <typename... Args>
        void emplace_back(Args&&... args)
        {
            if (m_Size >= m_Capacity)
                re_alloc(m_Capacity + (m_Capacity >> 1));

            new(&m_Arr[m_Size++]) T(std::forward<Args>(args)...);
        }

        size_t capacity() const noexcept { return m_Capacity; }
        size_t size() const noexcept { return m_Size; }
        void print() const {
            if (m_Capacity == 0)
                return;

            for (size_t i = 0; i < m_Size; ++i )
            {
                std::cout << m_Arr[i] << '|';
            }

            std::cout << std::endl;
        }

    private:
        void re_alloc(size_t newCapacity)
        {                 
            T* ptr = static_cast<T*>(::operator new(newCapacity * sizeof(T)));

            for (size_t i = 0; i < m_Size; ++i)
            {
                // ptr[i] = std::move(m_Arr[i]); // Does not work, ptr[i] is unassigned memory, cannot move assign on unitialized memory
                new(&ptr[i]) T(std::move(m_Arr[i]));
                m_Arr[i].~T();
            }

            ::operator delete(m_Arr, m_Capacity * sizeof(T));
            m_Capacity = newCapacity;
            m_Arr = ptr;
        }

        
        private:
        T* m_Arr;
        size_t m_Capacity;
        size_t m_Size;
    };
}