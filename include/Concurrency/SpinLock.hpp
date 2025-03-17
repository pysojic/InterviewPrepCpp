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

/*
From Anthony William's Concurrency in Action book:

The lock() operation is a loop on flag.test_and_set() using std::memory_
order_acquire ordering, and the unlock() is a call to flag.clear() with std::memory
_order_release ordering. When the first thread calls lock(), the flag is initially clear,
so the first call to test_and_set() will set the flag and return false, indicating that
this thread now has the lock, and terminating the loop. The thread is then free to
modify any data protected by the mutex. Any other thread that calls lock() at this
time will find the flag already set and will be blocked in the test_and_set() loop.

When the thread with the lock has finished modifying the protected data, it calls
unlock(), which calls flag.clear() with std::memory_order_release semantics.
This then synchronizes (see section 5.3.1) with a subsequent call to flag.test
_and_set() from an invocation of lock() on another thread, because this call has
std::memory_order_acquire semantics. Because the modification of the protected
data is necessarily sequenced before the unlock() call, this modification happens
before the unlock() and thus happens before the subsequent lock() call from the
second thread (because of the synchronizes with relationship between the unlock()
and the lock()) and happens before any accesses to that data from this second thread
once it has acquired the lock.
*/