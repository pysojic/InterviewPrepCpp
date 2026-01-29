#include <atomic>
#include <iostream>

/*
NOTE ABOUT PERFORMANCE UNDER CONTENTION
--------------------------------------
This spinlock uses atomic_flag::test_and_set() in a tight loop (a “TAS” lock: test-and-set).
Under contention, this performs poorly because each failed iteration is a read–modify–write (RMW)
on the same cache line. On most CPUs, an RMW requires exclusive ownership of that cache line,
so every spinning thread repeatedly forces the line into an exclusive state. With multiple
contenders this creates heavy cache-coherence traffic (“cache line ping-pong”), burns CPU,
increases latency, and can reduce overall throughput. It is also not fair: a thread may starve.

How to improve:
1) Use a TTAS lock (test–test–and–set):
    - Do ONE RMW attempt to acquire.
    - If it fails, spin using only relaxed loads until the lock looks free, then retry the RMW.
    This keeps the cache line mostly shared while waiting, dramatically reducing coherence storms.

 2) Add backoff / pause:
    - Insert a processor “pause” instruction (e.g., _mm_pause on x86) inside the spin loop to
      reduce power and improve hyper-threading behavior.
    - Consider exponential backoff or occasional std::this_thread::yield() for longer waits.

 3) Consider a different primitive when appropriate:
    - For anything but very short critical sections (and especially if blocking/I/O can occur),
      prefer std::mutex or platform futex-based locks to avoid wasting CPU.
    - For higher fairness under contention, consider ticket/MCS locks.

 See a nice post from Erik Rigtorp about this here: https://rigtorp.se/spinlock/
*/

/*
NOTE 2 ABOUT SCHEDULING / YIELDING UNDER CONTENTION (WHY PURE SPINNING CAN GET WORSE)
-----------------------------------------------------------------------------------
The TTAS-style optimization described in NOTE 1 reduces *cache-coherence* pain (it avoids doing
an RMW on every failed iteration), but it does NOT solve a different problem: *CPU scheduling*.

If a thread fails to acquire the lock and then spins aggressively, it consumes its entire time
slice doing “useful work” from the scheduler’s point of view (it is runnable and burning CPU).
On an oversubscribed system (more runnable threads than cores), this can lead to a bad outcome:
the thread that is spinning gets plenty of CPU time, while the thread that currently *holds* the
lock (and therefore is the only one that can make progress by releasing it) may get delayed by
the scheduler. In the worst case, the spinner effectively prevents timely execution of the lock
holder, increasing lock hold time and overall latency.

A common mitigation is to add a “polite” waiting strategy: try to acquire the lock a small number
of times (often ~8–16 iterations is a reasonable starting point), using a CPU pause instruction
inside the spin (e.g., _mm_pause on x86 to reduce pipeline pressure and improve SMT behavior),
and if the lock is still not available, briefly yield or sleep so other threads can run and the
current owner can reach unlock(). This is why you will often see implementations that do:
  - short spin with pause/backoff
  - then std::this_thread::yield(), nanosleep(), or a futex-based park when waiting longer
*/

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