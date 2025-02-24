#include <iostream>
#include <utility>

struct ControlBlock
{
    ControlBlock() : m_RefCount{}, m_WeakRefCount{}
    {}
    ControlBlock(size_t refCount, size_t weakRefCount) : m_RefCount{refCount}, m_WeakRefCount{weakRefCount}
    {}
    ~ControlBlock() noexcept {}

    // T* m_Ptr; Should the control block point to the shared object ?
    size_t m_RefCount;
    size_t m_WeakRefCount;
};

template <typename T>
class SharedPtr
{
public:
    SharedPtr() 
        : m_Ptr{ nullptr }, m_ControlBlock{ nullptr }
    {}

    SharedPtr(T* ptr) 
        : m_Ptr{ ptr }, m_ControlBlock( new ControlBlock(1, 0) )
    {}

    SharedPtr(const SharedPtr& other) 
        : m_Ptr{ other.m_Ptr }, m_ControlBlock{ other.m_ControlBlock } 
    {
        if (m_ControlBlock) 
        {
            ++m_ControlBlock->m_RefCount;
        }
    }

    SharedPtr& operator =(const SharedPtr& other)
    {
        if (this != &other)
        {
            if (m_ControlBlock->m_RefCount == 1)
            {
                delete m_Ptr;
                delete m_ControlBlock;
            }
            else
            {
                m_ControlBlock->m_RefCount--;
            }

            m_ControlBlock = other.m_ControlBlock;
            m_ControlBlock->m_RefCount++;
            m_Ptr = other.m_Ptr;
        }

        return *this;
    }

    SharedPtr(SharedPtr&& other) noexcept
        : m_Ptr{std::exchange(other.m_Ptr, nullptr)}, m_ControlBlock{std::exchange(other.m_ControlBlock, nullptr)}
    {}

    SharedPtr& operator= (SharedPtr&& other) noexcept
    {
        if (this != &other)
        {
            m_Ptr = std::exchange(other.m_Ptr, nullptr);
            m_ControlBlock = std::exchange(other.m_ControlBlock, nullptr);
        }

        return *this;
    }
    ~SharedPtr() noexcept
    {
        if (m_ControlBlock->m_RefCount == 1)
        {
            // Could potentially keep the control block alive if WeakRefCount above one
            delete m_Ptr;
            delete m_ControlBlock;
        }
        else
        {
            m_ControlBlock->m_RefCount--;
        }
    }

    T* get() const noexcept { return m_Ptr; }
    size_t use_count() const noexcept { return m_ControlBlock->m_RefCount; }
    operator bool() const { return m_Ptr != nullptr; }
    // const T& operator []() const; To do next
    const T& operator *() const { return *m_Ptr; }
    T* operator ->() const { return m_Ptr; }

    void reset(T* other_ptr) 
    { 
        if (m_ControlBlock)
        {
            if (m_ControlBlock->m_RefCount == 1)
            {
                // Could potentially keep the control block alive if WeakRefCount above one
                delete m_Ptr;
                delete m_ControlBlock;
            }
            else
            {
                m_ControlBlock->m_RefCount--;
            }
        }
        
        m_Ptr = other_ptr;
        m_ControlBlock = new ControlBlock(1, 0);
    }
    void swap(SharedPtr& other)
    {
        std::swap(m_Ptr, other.m_Ptr);
        std::swap(m_ControlBlock, other.m_ControlBlock);
    }

private:
    T* m_Ptr;
    ControlBlock* m_ControlBlock;
};
