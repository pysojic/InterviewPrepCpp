#include <atomic>

class Mutex
{
public:
    Mutex();

    void lock() noexcept;
    bool try_lock() noexcept;
    void unlock() noexcept;

private:
    std::atomic_flag m_Flag;
};

Mutex::Mutex() noexcept
    : m_Flag{ATOMIC_FLAG_INIT}
{}

void Mutex::lock() noexcept
{
    while(m_Flag.test_and_set(std::memory_order_acquire))
        m_Flag.wait(true, std::memory_order_relaxed);
}

bool Mutex::try_lock() noexcept
{
    return !m_Flag.test_and_set(std::memory_order_acquire);
}

void Mutex::unlock()
{
    m_Flag.clear(std::memory_order_release);
    m_Flag.notify_one();
}