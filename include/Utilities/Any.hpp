#pragma once

#include <any>

namespace pysojic
{
    class Any
    {
    public:
        template <typename T>
        explicit Any(T&&);
        Any(const Any& other);
        ~Any();

        template <typename T>
        Any& operator=(T&& other);
        void swap(Any& other) noexcept;
        void reset() noexcept;
        bool has_value() noexcept;
        const std::type_info& type() const noexcept;

        template <typename T>
        friend T& any_cast(Any& any);

    private:
        // Generic pointer to hold the generic data
        void* m_Data;
        // Points to a lambda that, when invoked, returns the type information (via typeid) for the stored object
        // This allows runtime type checking.
        const std::type_info& (*m_GetType)(); 
        // Points to a lambda that can create a deep copy of the stored object
        // This is used by the copy constructor to clone the value from another Any object
        void* (*m_Clone)(void* otherData);
        // Points to a lambda that knows how to delete the stored object (using delete on the proper type)
        void (*m_Destroy)(void* data);
    };

    //------------ Implementation ------------
    template <typename T>
    Any::Any(T&& data)
    // Need to use std::decay_t to allow passing array types (such as string literals), which cannot normally be allocated using new
    // std::decay_t converts T into a plain value type by stripping references, cv-qualifiers, and converting arrays to pointers
        : m_Data{ new std::decay_t<T>(std::forward<T>(data)) }, m_GetType{ []() -> const std::type_info& { return typeid(std::decay_t<T>); } },
        m_Clone{ [](void* otherData) -> void* { return new std::decay_t<T>(*static_cast<std::decay_t<T>*>(otherData)); } },
        m_Destroy{ [](void* data) { delete static_cast<std::decay_t<T>*>(data); } }
    {}

    Any::Any(const Any& other)
        : m_Data{ other.m_Clone(other.m_Data) }, m_GetType( other.m_GetType ), m_Clone{ other.m_Clone },
        m_Destroy{ other.m_Destroy }
    {}

    Any::~Any()
    {
        m_Destroy(m_Data);
    }

    template <typename T>
    Any& Any::operator=(T&& data)
    {
        reset();  // Clean up any existing stored value
        m_Data = new std::decay_t<T>{ std::forward<T>(data) };
        m_GetType = []() -> const std::type_info& { return typeid(std::decay_t<T>); };
        m_Clone = [](void* otherData) -> void* { return new std::decay_t<T>(*static_cast<std::decay_t<T>*>(otherData)); };
        m_Destroy = [](void* data) { delete static_cast<std::decay_t<T>*>(data); };
        return *this;
    }

    void Any::swap(Any& other) noexcept
    {
        std::swap(m_Data, other.m_Data);
        std::swap(m_Clone, other.m_Clone);
        std::swap(m_GetType, other.m_GetType);
        std::swap(m_Destroy, other.m_Destroy);
    }

    void Any::reset() noexcept
    {
        if (m_Data) 
        {
            m_Destroy(m_Data);
            m_Data = nullptr;
            m_GetType = []() -> const std::type_info& { return typeid(void); };
            m_Clone = nullptr;
            m_Destroy = nullptr;
        }
    }

    bool Any::has_value() noexcept
    {
        return m_Data != nullptr;
    }

    const std::type_info& Any::type() const noexcept
    {
        return m_GetType();
    }

    template<typename T>
    T& any_cast(Any& any)
    {
        if (typeid(std::decay_t<T>) == any.m_GetType())
        {
            return *static_cast<std::decay_t<T>*>(any.m_Data);
        }
        else
        {
            throw std::bad_any_cast{};
        }
    }
}