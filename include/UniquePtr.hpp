// UniquePtr
#include <utility>
#include <string>
#include <iostream>

template <typename T> 
class UniquePtr
{
public:
    UniquePtr() : m_Data{ nullptr } {}
    UniquePtr(T* ptr) : m_Data{ ptr } {}
    ~UniquePtr() 
    {
        delete m_Data; // free the pointer
    }

    UniquePtr(UniquePtr&& from) : m_Data{std::exchange(from.m_Data, nullptr)}
    {
    }

    // Move assignment
    UniquePtr& operator = (UniquePtr&& from)
     {
        if (this != &from) 
        {
            m_Data = std::exchange(from.m_Data, nullptr);
        }

        return *this; // remember to dereference
    }
    
    // Delete the copy constructor and copy assignment
    // Unique pointers take ownership of heap alloc object
    // Construct, delete object. Cannot have 2 copies of unique ptr
    UniquePtr(const UniquePtr& other) = delete;
    UniquePtr & operator = (const UniquePtr & other) = delete;

    // Operations
    T* release() 
    {
        // releases control over pointer
        return std::exchange(m_Data, nullptr);
    }; 
    
    void reset(T* replacement){
        // delete one currently used by shared ptr, takes new one
        if (replacement != m_Data)
        {
            delete m_Data; // delete the object managed
            m_Data = replacement;
        }
    }; 

    void swap(UniquePtr& other) 
    {
        std::swap(m_Data, other.m_Data);
    };

    T* get() const 
    {
        // const T* would make it a const object
        return m_Data;
    }; 

    T& operator* () 
    {
        // return a T reference
        return *(m_Data);
    }; 

    // Marking this as explicit prevents implicit conversions in general 
    // but still allows usage in conditional expressions where a bool is required by context
    explicit operator bool()
    {
        return get() != nullptr;
    }

private:
    T * m_Data;
};

// Make Unique
template <typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args) 
{
    // ... is the parameter pack syntax
    // in std::forward, the ... after means to unpack the params
    return UniquePtr<T>(new T{std::forward<Args>(args)...});
};
