#pragma once

#include <utility>
#include <memory>

template<typename T, typename Allocator = std::allocator<T>>
class Vector
{
public:
    using alloc = std::allocator_traits<Allocator>;

    Vector();
    Vector(size_t size) ;
    Vector(size_t size, const T& value);
    Vector(std::initializer_list<T> init);
    Vector(const Vector<T, Allocator>& other);
    Vector& operator= (const Vector<T, Allocator>& other);
    Vector(Vector<T, Allocator>&& other) noexcept;
    Vector<T, Allocator>& operator=(Vector<T, Allocator>&& other) noexcept;
    ~Vector() noexcept;    

    void clear() noexcept;
    void push_back(const T& obj);
    void push_back(T&& obj);
    void pop_back() noexcept;
    template <typename... Args>
    void emplace_back(Args&&... args);
    void shrink_to_fit();
    void resize(size_t newSize);
    void reserve(size_t newCapacity);

    T& front() noexcept { return m_Arr[0]; }
    const T& front() const noexcept{ return m_Arr[0]; }
    T& back() noexcept { return m_Arr[m_Size - 1]; }
    const T& back() const noexcept{ return m_Arr[m_Size - 1]; }
    const T& operator[](size_t index) const noexcept { return m_Arr[index]; }
    T& operator[](size_t index) noexcept { return m_Arr[index]; }
    size_t capacity() const noexcept { return m_Capacity; }
    size_t size() const noexcept { return m_Size; }
    void print() const;

private:
    void reallocate(size_t newCapacity);
    
private:
    Allocator m_Allocator;
    size_t m_Capacity;
    size_t m_Size;
    T* m_Arr;
};

//------------Implementation--------------

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector() 
    : m_Capacity{1}, m_Size{0}, m_Arr{alloc::allocate(m_Allocator, m_Capacity)}
{
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(size_t size) 
    : m_Capacity{size}, m_Size{0}, m_Arr{alloc::allocate(m_Allocator, m_Capacity)}
{
    try 
    {
        for (size_t i{}; i < size; ++i) 
        {
            alloc::construct(m_Allocator, &m_Arr[m_Size++]); // default constructing
        }
    } 
    catch (...) 
    {
        // If construction fails, clean up constructed elements and free memory.
        clear();
        alloc::deallocate(m_Allocator, m_Arr, m_Capacity);
        throw;
    }
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(size_t size, const T& value) 
    : m_Capacity{size}, m_Size{0}, m_Arr{alloc::allocate(m_Allocator, m_Capacity)}
{
    try 
    {
        for (size_t i{}; i < size; ++i) 
        {
            alloc::construct(m_Allocator, &m_Arr[m_Size++], value);
        }
    } 
    catch (...) 
    {
        clear();
        alloc::deallocate(m_Allocator, m_Arr, m_Capacity);
        throw;
    }
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(std::initializer_list<T> init)
{
    size_t size = init.size();
    m_Capacity = size;
    m_Arr = alloc::allocate(m_Allocator, m_Capacity);
    m_Size = 0;

    for (auto& elem : init)
    {
        alloc::construct(m_Allocator, &m_Arr[m_Size++], elem);
    }
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(const Vector<T, Allocator>& other) 
    : m_Capacity{other.m_Capacity}, m_Size{0}, m_Arr{alloc::allocate(m_Allocator, m_Capacity)}
{
    try 
    {
        for (size_t i{}; i < size; ++i) 
        {
            alloc::construct(m_Allocator, &m_Arr[m_Size++], other.m_Arr[i]);
        }
    } 
    catch (...) 
    {
        clear();
        alloc::deallocate(m_Allocator, m_Arr, m_Capacity);
        throw;
    }
}

template <typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator= (const Vector<T, Allocator>& other)
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
        alloc::deallocate(m_Allocator, m_Arr, m_Capacity);

        m_Capacity = other.m_Capacity;
        // m_Size = 0; // Not really needed since clear() was called
        m_Arr = alloc::allocate(m_Allocator, m_Capacity);
        
        for (size_t i{}; i < size; ++i) 
        {
            alloc::construct(m_Allocator, &m_Arr[m_Size++], other.m_Arr[i]);
        }
    }

    return *this;
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(Vector<T, Allocator>&& other) noexcept
    : m_Arr{std::exchange(other.m_Arr, nullptr)}, m_Capacity{std::exchange(other.m_Capacity, 0)}, 
    m_Size{std::exchange(other.m_Size, 0)}
{}

template <typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(Vector<T, Allocator>&& other) noexcept
{
    if (this != &other)
    {
        // Same problem as for the copy assignment operator, see above
        // this->~Vector();
        clear();
        alloc::deallocate(m_Allocator, m_Arr, m_Capacity);

        // Steal resources
        m_Arr = std::exchange(other.m_Arr, nullptr);
        m_Capacity = std::exchange(other.m_Capacity, 0);
        m_Size = std::exchange(other.m_Size, 0);
    }
    return *this;
}

template <typename T, typename Allocator>
Vector<T, Allocator>::~Vector() noexcept
{
    clear();
    alloc::deallocate(m_Allocator, m_Arr, m_Capacity);
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::clear() noexcept
{
    for (size_t i = 0; i < m_Size; ++i)
    {
        // m_Arr[i].~T(); Equivalent 
        alloc::destroy(m_Allocator, &m_Arr[i]);
    }
    m_Size = 0;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::push_back(const T& obj)
{
    if (m_Size >= m_Capacity)
        reallocate(m_Capacity * 2);
    
    // When you allocate raw memory with ::operator new, no objects are constructed in that memory. 
    // Directly assigning to m_Arr[m_Size] is essentially writing to uninitialized memory, 
    // which is undefined behavior—even if it appears to work in some cases.
    // m_Arr[m_Size++] = obj;
    std::construct_at(&m_Arr[m_Size++], obj);
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::push_back(T&& obj)
{
    if (m_Size >= m_Capacity)
        reallocate(m_Capacity * 2);
    
    // When you allocate raw memory with ::operator new, no objects are constructed in that memory. 
    // Directly assigning to m_Arr[m_Size] is essentially writing to uninitialized memory, 
    // which is undefined behavior—even if it appears to work in some cases.
    // m_Arr[m_Size++] = std::move(obj);
    std::construct_at(&m_Arr[m_Size++], std::move(obj));
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::pop_back() noexcept
{
    if (m_Size > 0)
    {
        m_Arr[m_Size - 1].~T();
        --m_Size;
    }
}

template<typename T, typename Allocator>
template<typename... Args>
void Vector<T, Allocator>::emplace_back(Args&&... args)
{
    if (m_Size >= m_Capacity)
        reallocate(m_Capacity * 2);

    std::construct_at(&m_Arr[m_Size++], std::forward<Args>(args)...);
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::shrink_to_fit()
{
    reallocate(m_Size);
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::resize(size_t newSize)
{
    if (newSize > m_Size)
    {
        if (newSize < m_Capacity)
        {
            for (size_t i = m_Size; i < newSize; ++i)
            {
                std::construct_at(&m_Arr[i]);
            }
        }   
        else
        {
            reallocate(newSize + (newSize >> 1));
            for (size_t i = m_Size; i < newSize; ++i)
            {
                std::construct_at(&m_Arr[i]);
            }
        }
    }
    
    if (newSize < m_Size)
    {
        for (size_t i = m_Size - 1; i > newSize - 1; --i)
        {
            //m_Arr[i].~T();
            std::destroy_at(&m_Arr[i]);
        }
    }

    m_Size = newSize;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::reserve(size_t newCapacity)
{
    if (newCapacity > m_Capacity)
        reallocate(newCapacity);
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::print() const 
{
    for (size_t i = 0; i < m_Size; ++i )
    {
        std::cout << m_Arr[i] << '|';
    }

    std::cout << std::endl;
}


template <typename T, typename Allocator>
void Vector<T, Allocator>::reallocate(size_t newCapacity)
{                 
    T* ptr = static_cast<T*>(::operator new(newCapacity * sizeof(T)));

    for (size_t i = 0; i < m_Size; ++i)
    {
        // ptr[i] = std::move(m_Arr[i]); // Does not work, ptr[i] is unassigned memory, cannot move assign on unitialized memory
        std::construct_at(&ptr[i], std::move(m_Arr[i]));
        // m_Arr[i].~T(); Equivalent to line below
        std::destroy_at(&m_Arr[i]);
    }

    ::operator delete(m_Arr, m_Capacity * sizeof(T));
    m_Capacity = newCapacity;
    m_Arr = ptr;
}