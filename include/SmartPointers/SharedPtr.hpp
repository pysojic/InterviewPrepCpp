#include <iostream>
#include <utility>
#include <atomic>

template <typename T>
struct ControlBlock
{
    ControlBlock() : m_Data{nullptr}, m_RefCount{}, m_WeakRefCount{}
    {}
    ControlBlock(T* data, size_t refCount, size_t weakRefCount) : m_Data{data}, m_RefCount{refCount}, m_WeakRefCount{weakRefCount}
    {}
    ~ControlBlock() noexcept { delete m_Data; }

    T* m_Data;
    std::atomic_size_t m_RefCount;
    std::atomic_size_t m_WeakRefCount;
};

template <typename T>
class SharedPtr
{
public:
    SharedPtr() 
        : m_ControlBlock{ nullptr }
    {}

    SharedPtr(T* ptr) 
        : m_ControlBlock( new ControlBlock(ptr, 1, 0) )
    {}

    SharedPtr(const SharedPtr& other) 
        : m_ControlBlock{ other.m_ControlBlock } 
    {
        // Checks that other already owns memory
        if (m_ControlBlock) 
        {
            ++m_ControlBlock->m_RefCount;
        }
    }

    SharedPtr& operator =(const SharedPtr& other)
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

    SharedPtr(SharedPtr&& other) noexcept
        : m_ControlBlock{std::exchange(other.m_ControlBlock, nullptr)}
    {}

    SharedPtr& operator= (SharedPtr&& other) noexcept
    {
        if (this != &other)
        {
            m_ControlBlock = std::exchange(other.m_ControlBlock, nullptr);
        }

        return *this;
    }
    ~SharedPtr() noexcept
    {
        release();
    }

    T* get() const noexcept { return m_ControlBlock->m_Data; }
    size_t use_count() const noexcept { return m_ControlBlock ? m_ControlBlock->m_RefCount : 0; }
    operator bool() const { return m_ControlBlock && m_ControlBlock->m_Data != nullptr; }
    // const T& operator []() const; To do next
    const T& operator *() const { return *m_ControlBlock->m_Data; }
    T* operator ->() const { return m_ControlBlock->m_Data; }

    void reset(T* other_ptr) 
    { 
        release();
        m_ControlBlock = new ControlBlock(other_ptr, 1, 0);
    }
    void swap(SharedPtr& other)
    {
        std::swap(m_ControlBlock, other.m_ControlBlock);
    }
    void release()
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

private:
    ControlBlock<T>* m_ControlBlock;
};
