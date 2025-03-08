#pragma once

#include <iostream>
#include <utility>
#include <atomic>

template <typename T>
class SharedPtr
{
    template <typename K>
    class ControlBlock
    {
        friend class SharedPtr;
        ControlBlock();
        ControlBlock(K* data, size_t refCount, size_t weakRefCount);
        ~ControlBlock() noexcept;

        T* m_Data;
        std::atomic_size_t m_RefCount;
        std::atomic_size_t m_WeakRefCount;
    };

public:
    SharedPtr() ;
    SharedPtr(T* ptr) ;
    SharedPtr(const SharedPtr& other);
    SharedPtr& operator =(const SharedPtr& other);
    SharedPtr(SharedPtr&& other) noexcept;
    SharedPtr& operator= (SharedPtr&& other) noexcept;
    ~SharedPtr() noexcept;

    T* get() const noexcept { return m_ControlBlock->m_Data; }
    size_t use_count() const noexcept { return m_ControlBlock ? m_ControlBlock->m_RefCount : 0; }
    operator bool() const { return m_ControlBlock && m_ControlBlock->m_Data != nullptr; }
    // const T& operator []() const; To do next
    const T& operator *() const { return *m_ControlBlock->m_Data; }
    T* operator ->() const { return m_ControlBlock->m_Data; }

    void reset(T* other_ptr);
    void swap(SharedPtr& other);
    void release();

private:
    ControlBlock<T>* m_ControlBlock;
};


template <typename T>
template <typename K>
SharedPtr<T>::ControlBlock<K>::ControlBlock() 
    : m_Data{nullptr}, m_RefCount{}, m_WeakRefCount{}
{}

template <typename T>
template <typename K>
SharedPtr<T>::ControlBlock<K>::ControlBlock(K* data, size_t refCount, size_t weakRefCount) 
    : m_Data{data}, m_RefCount{refCount}, m_WeakRefCount{weakRefCount}
{}

template <typename T>
template <typename K>
SharedPtr<T>::ControlBlock<K>::~ControlBlock() noexcept { delete m_Data; }

template <typename T>
SharedPtr<T>::SharedPtr() 
    : m_ControlBlock{ nullptr }
{}

template <typename T>
SharedPtr<T>::SharedPtr(T* ptr) 
    : m_ControlBlock( new ControlBlock<T>(ptr, 1, 0) )
{}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& other) 
    : m_ControlBlock{ other.m_ControlBlock } 
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
        if (m_ControlBlock)
        {
            ++m_ControlBlock->m_RefCount;
        }
    }

    return *this;
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr&& other) noexcept
    : m_ControlBlock{std::exchange(other.m_ControlBlock, nullptr)}
{}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator= (SharedPtr&& other) noexcept
{
    if (this != &other)
    {
        m_ControlBlock = std::exchange(other.m_ControlBlock, nullptr);
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
    m_ControlBlock = new ControlBlock<T>(other_ptr, 1, 0);
}

template <typename T>
void SharedPtr<T>::swap(SharedPtr<T>& other)
{
    std::swap(m_ControlBlock, other.m_ControlBlock);
}

template <typename T>
void SharedPtr<T>::release()
{
    if (m_ControlBlock)
    {
        if (--m_ControlBlock->m_RefCount == 0)
        {
            // Could potentially keep the control block alive if WeakRefCount above one
            delete m_ControlBlock;
        }
        m_ControlBlock = nullptr;
    }
}