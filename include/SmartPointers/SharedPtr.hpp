#pragma once

#include <iostream>
#include <utility>
#include <atomic>
#include <memory>
#include <cstddef>

namespace pysojic
{
    // Base class for the control block
    struct ControlBlockBase
    {
        virtual void destroy_object() noexcept = 0;   // when strong -> 0
        virtual void destroy_self() noexcept = 0;     // when strong==0 && weak==0
        virtual ~ControlBlockBase() noexcept = default;
        
        std::atomic_size_t m_RefCount{1};
        std::atomic_size_t m_WeakRefCount{0};
    };

    // Derived class for cases where the control block and data were initialized separately
    template <typename T>
    struct ControlBlockSeparate : public ControlBlockBase
    {
        explicit ControlBlockSeparate(T* data)
            : m_Data{data}
        {}
        virtual void destroy_object() noexcept override // when strong -> 0
        {
            delete m_Data;
            m_Data = nullptr;
        } 
        virtual void destroy_self() noexcept override // when strong==0 && weak==0
        {
            delete this;
        }

        T* m_Data;
    };

    // Derived class for cases where the control block and data were initialized together (via make_shared)
    template <typename T>
    struct ControlBlockInplace : public ControlBlockBase
    {
        template <typename... Args>
        explicit ControlBlockInplace(Args&&... args)
        {
            std::construct_at(ptr(), std::forward<Args>(args)...);
        }
        T* ptr() noexcept
        {
            /* std::launder makes this a "valid pointer to the T object currently living in this raw storage"
               after placement-new/construct_at, so the compiler doesn't treat it as an old/byte-pointer under
               object-lifetime rules (important when objects are created/destroyed in-place in the same buffer). */
            return std::launder(reinterpret_cast<T*>(buf));
        }
        virtual void destroy_object() noexcept override // when strong -> 0
        {
            std::destroy_at(ptr()); // destroys the object
        } 
        virtual void destroy_self() noexcept override     // when strong==0 && weak==0
        {
            this->~ControlBlockInplace();
            ::operator delete(this); // release the memory
        }
        
        // Constructing a T at a misaligned address is undefined behavior in the language, hence alignas is mandatory
        // here, otherwise it is UB
        alignas(T) std::byte buf[sizeof(T)];
    };

    template <typename T>
    class SharedPtr
    {

    public:
        SharedPtr() = default;
        explicit SharedPtr(T* ptr);
        SharedPtr(const SharedPtr& other);
        SharedPtr& operator =(const SharedPtr& other);
        SharedPtr(SharedPtr&& other) noexcept;
        SharedPtr& operator= (SharedPtr&& other) noexcept;
        ~SharedPtr() noexcept;

        T* get() const noexcept { return m_Data; }
        size_t use_count() const noexcept { return m_ControlBlock ? m_ControlBlock->m_RefCount.load() : 0; }
        explicit operator bool() const { return m_Data != nullptr; }
        // const T& operator []() const; To do next
        const T& operator *() const { return *m_Data; }
        T* operator ->() const { return m_Data; }

        void reset(T* other_ptr);
        void swap(SharedPtr& other);
        void release();

        template<typename K, typename... Args>
        friend SharedPtr<K> make_shared(Args&&... args);

    private:
        ControlBlockBase* m_ControlBlock{nullptr};
        T* m_Data{nullptr};
    };

    //------------Implementation--------------

    template <typename T>
    SharedPtr<T>::SharedPtr(T* ptr) 
        : m_ControlBlock{ ptr ? new ControlBlockSeparate<T>(ptr) : nullptr }, m_Data{ptr}
    {
        if (ptr)
            m_ControlBlock = new ControlBlockSeparate<T>{ptr};
    }

    template <typename T>
    SharedPtr<T>::SharedPtr(const SharedPtr<T>& other) 
        : m_ControlBlock{ other.m_ControlBlock } , m_Data{other.m_Data}
    {
        // Checks that other already owns memory
        if (m_ControlBlock) 
        {
            ++m_ControlBlock->m_RefCount;
        }
    }

    template <typename T>
    SharedPtr<T>& SharedPtr<T>::operator =(const SharedPtr<T>& other)
    {
        if (this != &other)
        {
            release();
            m_ControlBlock = other.m_ControlBlock;
            m_Data = other.m_Data;
            if (m_ControlBlock)
            {
                ++m_ControlBlock->m_RefCount;
            }
        }

        return *this;
    }

    template <typename T>
    SharedPtr<T>::SharedPtr(SharedPtr&& other) noexcept
        : m_ControlBlock{std::exchange(other.m_ControlBlock, nullptr)}, m_Data{std::exchange(other.m_Data, nullptr)}
    {}

    template <typename T>
    SharedPtr<T>& SharedPtr<T>::operator= (SharedPtr&& other) noexcept
    {
        if (this != &other)
        {
            release();
            m_ControlBlock = std::exchange(other.m_ControlBlock, nullptr);
            m_Data = std::exchange(other.m_Data, nullptr);
        }

        return *this;
    }

    template <typename T>
    SharedPtr<T>::~SharedPtr() noexcept
    {
        release();
    }

    template <typename T>
    void SharedPtr<T>::reset(T* other_ptr) 
    { 
        release();
        if (!other_ptr) 
        {
            m_ControlBlock = nullptr;
            m_Data = nullptr;
            return;
        }
        m_ControlBlock = new ControlBlockSeparate<T>(other_ptr); 
        m_Data = other_ptr;
    }

    template <typename T>
    void SharedPtr<T>::swap(SharedPtr<T>& other)
    {
        std::swap(m_ControlBlock, other.m_ControlBlock);
        std::swap(m_Data, other.m_Data);
    }

    template <typename T>
    void SharedPtr<T>::release()
    {
        if (!m_ControlBlock) 
            return;

        if (m_ControlBlock->m_RefCount.fetch_sub(1) == 1) 
        {
            m_ControlBlock->destroy_object();
            if (m_ControlBlock->m_WeakRefCount.load() == 0) 
            {
                m_ControlBlock->destroy_self();
            }
        }

        m_ControlBlock = nullptr;
        m_Data = nullptr;
    }

    template<typename T, typename... Args>
    SharedPtr<T> make_shared(Args&&... args)
    {
        using CB = ControlBlockInplace<T>;

        CB* mem = static_cast<CB*>(::operator new(sizeof(CB)));
        try 
        {
            CB* cb = std::construct_at(mem, std::forward<Args>(args)...);

            SharedPtr<T> sp;
            sp.m_ControlBlock = cb;
            sp.m_Data = cb->ptr();
            return sp;
        } 
        catch (...) 
        {
            ::operator delete(mem);
            throw;
        }
    }

}