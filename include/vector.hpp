#pragma once

namespace impl
{
    template<typename T>
    class Vector
    {
    public:
        Vector() 
            : m_Capacity{1}, m_Size{0}
        {
            m_Arr = static_cast<T*>(::operator new(m_Capacity * sizeof(T)));
        }

        Vector(size_t size) 
            : m_Capacity{size}, m_Size{0}
        {
            m_Arr = static_cast<T*>(::operator new(m_Capacity * sizeof(T)));
            try 
            {
                for (size_t i = 0; i < size; ++i) 
                {
                    std::construct_at(&m_Arr[m_Size++]); // default constructing
                }
            } 
            catch (...) 
            {
                // If construction fails, clean up constructed elements and free memory.
                clear();
                ::operator delete(m_Arr, m_Capacity * sizeof(T));
                throw;
            }
        }

        Vector(size_t size, const T& value) 
            : m_Capacity{size}, m_Size{0}
        {
            m_Arr = static_cast<T*>(::operator new(m_Capacity * sizeof(T)));
            try 
            {
                for (size_t i = 0; i < size; ++i) 
                {
                    std::construct_at(&m_Arr[m_Size++], value);
                }
            } 
            catch (...) 
            {
                clear();
                ::operator delete(m_Arr, m_Capacity * sizeof(T));
                throw;
            }
        }

        Vector(const Vector<T>& other) 
            : m_Capacity{other.m_Capacity}, m_Size{0}
        {
            m_Arr = static_cast<T*>(::operator new(m_Capacity * sizeof(T)));
            try 
            {
                for (size_t i = 0; i < other.m_Size; ++i) 
                {
                    std::construct_at(&m_Arr[m_Size++], other.m_Arr[i]);
                }
            } 
            catch (...) 
            {
                clear();
                ::operator delete(m_Arr, m_Capacity * sizeof(T));
                throw;
            }
        }
        Vector& operator= (const Vector<T>& other)
        {
            if (this != &other)
            {
                // Explicitly calling the destructor on this (i.e. this->~Vector()) destroys your object entirely. 
                // When you do that, you're not simply "clearing" the object's resources; you're destroying the entire object, 
                // including its vtable (if polymorphic) and its internal state. 
                // This leaves your object in an indeterminate state and can lead to undefined behavior when you try to use it afterward.
                // Remember that an object which has been moved from should be left in a valid state
                // this->~Vector();
                clear();
                ::operator delete(m_Arr, m_Capacity * sizeof(T));

                m_Capacity = other.m_Capacity;
                m_Size = 0;
                m_Arr = static_cast<T*>(::operator new(m_Capacity * sizeof(T)));
                
                for (size_t i = 0; i < other.m_Size; ++i)
                {
                    std::construct_at(&m_Arr[m_Size++], other.m_Arr[i]);
                }
            }

            return *this;
        }
        Vector(Vector<T>&& other) noexcept
            : m_Arr{other.m_Arr}, m_Capacity{other.m_Capacity}, m_Size{other.m_Size}
        {
            other.m_Arr = nullptr;
            other.m_Capacity = 0;
            other.m_Size = 0;
        }
        Vector<T>& operator=(Vector<T>&& other) noexcept
        {
            if (this != &other)
            {
                // Same problem as for the copy assignment operator, see above
                // this->~Vector();
                clear();
                ::operator delete(m_Arr, m_Capacity * sizeof(T));

                // Steal resources
                m_Arr = other.m_Arr;
                m_Capacity = other.m_Capacity;
                m_Size = other.m_Size;

                // Leave other in a safe state
                other.m_Arr = nullptr;
                other.m_Capacity = 0;
                other.m_Size = 0;
            }
            return *this;
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
            
            // When you allocate raw memory with ::operator new, no objects are constructed in that memory. 
            // Directly assigning to m_Arr[m_Size] is essentially writing to uninitialized memory, 
            // which is undefined behavior—even if it appears to work in some cases.
            // m_Arr[m_Size++] = obj;
            std::construct_at(&m_Arr[m_Size++], obj);
        }
        void push_back(T&& obj)
        {
            if (m_Size >= m_Capacity)
                re_alloc(m_Capacity + (m_Capacity >> 1));
            
            // When you allocate raw memory with ::operator new, no objects are constructed in that memory. 
            // Directly assigning to m_Arr[m_Size] is essentially writing to uninitialized memory, 
            // which is undefined behavior—even if it appears to work in some cases.
            // m_Arr[m_Size++] = std::move(obj);
            std::construct_at(&m_Arr[m_Size++], std::move(obj));
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

            std::construct_at(&m_Arr[m_Size++], std::forward<Args>(args)...);
        }

        size_t capacity() const noexcept { return m_Capacity; }
        size_t size() const noexcept { return m_Size; }
        void print() const 
        {
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
                std::construct_at(&ptr[i], std::move(m_Arr[i]));
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