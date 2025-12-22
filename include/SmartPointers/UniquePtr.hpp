#pragma once

// UniquePtr
#include <utility>
#include <string>
#include <iostream>

template <typename T>
struct DefaultDeleter
{
    void operator()(T* ptr) const
    {
        delete ptr;
    }
};

namespace pysojic
{
    template <typename T, typename Deleter = DefaultDeleter<T>> 
    class UniquePtr
    {
    public:
        UniquePtr() : m_Data{ nullptr } {}
        explicit UniquePtr(T* ptr) : m_Data{ ptr } {}
        UniquePtr(T* ptr, const Deleter& deleter) : m_Data{ ptr }, m_Deleter{ deleter } {}
        ~UniquePtr() 
        {
            if (m_Data)
                m_Deleter(m_Data); // free the pointer
        }

        UniquePtr(UniquePtr&& from) : m_Data{std::exchange(from.m_Data, nullptr)}, m_Deleter{ std::move(from.m_Deleter) }
        {
        }

        // Move assignment
        UniquePtr& operator = (UniquePtr&& from)
        {
            if (this != &from) 
            {
                m_Deleter(m_Data);
                m_Data = std::exchange(from.m_Data, nullptr);
                m_Deleter = std::move(from.m_Deleter);
            }

            return *this; // remember to dereference
        }
        
        // Delete the copy constructor and copy assignment
        // Unique pointers take ownership of heap alloc object
        // Construct, delete object. Cannot have 2 copies of unique ptr
        UniquePtr(const UniquePtr& other) = delete;
        UniquePtr & operator = (const UniquePtr & other) = delete;

        // Operations
        T* release() noexcept
        {
            // releases control over pointer
            return std::exchange(m_Data, nullptr);
        }; 

        void reset(T* replacement) noexcept
        {
            // delete one currently used by unique ptr, takes new one
            if (replacement != m_Data)
            {
                m_Deleter(m_Data); // delete the object managed
                m_Data = replacement;
            }
        }; 

        void swap(UniquePtr& other) 
        {
            std::swap(m_Data, other.m_Data);
            std::swap(m_Deleter, other.m_Deleter);
        };

        T* get() const noexcept
        {
            // const T* would make it a const object
            return m_Data;
        }; 

        T& operator* () noexcept
        {
            // return a T reference
            return *m_Data;
        }; 

        T* operator->() noexcept 
        {
            return m_Data;
        };

        // Marking this as explicit prevents implicit conversions in general 
        // but still allows usage in conditional expressions where a bool is required by context
        // e.g. if(sharedPtr) will work as expected
        [[nodiscard]] explicit operator bool() const noexcept
        {
            return m_Data != nullptr;
        }

    private:
        T * m_Data;
        // use the [[no_unique_address]] attribute to save memory when deleter is stateless
       [[no_unique_address]] Deleter m_Deleter; 
    };

    // Make Unique
    template <typename T, typename... Args>
    UniquePtr<T> make_unique(Args&&... args) 
    {
        // ... is the parameter pack syntax
        // in std::forward, the ... after means to unpack the params
        return UniquePtr<T>(new T{std::forward<Args>(args)...});
    };
}
