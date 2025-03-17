#include <atomic>
#include <iostream>

class SpinLock
{
public:
    SpinLock();

    void lock() noexcept;
    bool try_lock() noexcept;
    void unlock() noexcept;

private:
    std::atomic_flag m_Flag;
};

SpinLock::SpinLock()
    : m_Flag{ATOMIC_FLAG_INIT}
{}

void SpinLock::lock() noexcept
{   
    while(m_Flag.test_and_set(std::memory_order_acquire))
        std::cout << "Already locked, keep spinning" << std::endl;
    std::cout << "Lock acquired!" << std::endl;
}

bool SpinLock::try_lock() noexcept
{
    return !m_Flag.test_and_set(std::memory_order_acquire);
}

void SpinLock::unlock() noexcept
{
    m_Flag.clear(std::memory_order_release);
}