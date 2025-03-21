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

    private:
        void* m_Data;
        const std::type_info& (*m_GetType)();
        void* (*m_Clone)(void* otherData);
        void (*m_Destroy)(void* data);
    };

    //------------ Implementation ------------
    template <typename T>
    Any::Any(T&& data)
        : m_Data{ new T{std::forward<T>(data)} }, m_GetType{[]() -> const std::type_info& { return typeid(T); } },
        m_Clone{ [](void* otherData) -> void* { return new T(*static_cast<T*>(otherData)); } },
        m_Destroy{ [](void* data) { delete static_cast<T*>(data); } }
    {}

    Any::Any(const Any& other)
        : m_Data{ other.m_Clone(other.m_Data) }, m_GetType( other.m_GetType ), m_Clone{ other.m_Clone },
        m_Destroy{ other.m_Destroy }
    {}

    Any::~Any()
    {
        m_Destroy(m_Data);
    }

    template<typename T>
    T& any_cast(Any& any)
    {
        if (typeid(T) == any.m_GetType())
        {
            return *static_cast<T*>(any.m_Data);
        }
        else
        {
            throw std::bad_any_cast{};
        }
    }
}