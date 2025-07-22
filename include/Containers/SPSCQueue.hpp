#pragma once

#include <atomic>
#include <memory>
#include <array>

// Fixed Lock-free Single Producer, Single Consumer Queue
// Heavily inspired by rigtorp impl here: https://github.com/rigtorp/SPSCQueue

template<typename T, std::size_t Capacity>
class SPSCQueue {
public:
    // compile‐time proof that Capacity is a power of two ≥ 2
    static_assert(Capacity >= 2 && (Capacity & (Capacity - 1)) == 0,
                  "Capacity must be ≥2 and a power of two");

    SPSCQueue() = default;
    // no copies or moves
    SPSCQueue(const SPSCQueue&) = delete;
    SPSCQueue& operator=(const SPSCQueue&) = delete;
    SPSCQueue(SPSCQueue&&) = delete;
    SPSCQueue& operator=(SPSCQueue&&) = delete;

    bool push(const T& value) noexcept 
    {
        auto w = m_WriteIdx.load(std::memory_order_relaxed);
        auto r = m_ReadIdx .load(std::memory_order_acquire);

        // full when write - read == Capacity
        if (w - r == Capacity) 
            return false;

        m_Data[w & mask()] = value;
        m_WriteIdx.store(w + 1, std::memory_order_release);
        return true;
    }

    bool pop(T& out) noexcept 
    {
        auto r = m_ReadIdx .load(std::memory_order_relaxed);
        auto w = m_WriteIdx.load(std::memory_order_acquire);

        // empty when write == read
        if (r == w)
            return false;

        out = m_Data[r & mask()];
        m_ReadIdx.store(r + 1, std::memory_order_release);
        return true;
    }

    /// Peek at the next element (precondition: !empty())
    T& front() noexcept 
    {
        auto r = m_ReadIdx.load(std::memory_order_acquire);
        assert(r != m_WriteIdx.load(std::memory_order_acquire)
            && "front() on empty queue");
        return m_Data[r & mask()];
    }

    std::size_t size() const noexcept 
    {
        auto w = m_WriteIdx.load(std::memory_order_acquire);
        auto r = m_ReadIdx .load(std::memory_order_acquire);
        return w - r;
    }

    constexpr std::size_t capacity() const noexcept 
    {
        return Capacity;
    }

    bool empty() const noexcept 
    {
        return m_ReadIdx.load(std::memory_order_acquire)
             == m_WriteIdx.load(std::memory_order_acquire);
    }

    bool full() const noexcept 
    {
        return (m_WriteIdx.load(std::memory_order_acquire)
              - m_ReadIdx.load(std::memory_order_acquire))
             == Capacity;
    }

private:
    std::size_t mask() const noexcept 
    {
        return Capacity - 1;
    }

    std::array<T, Capacity> m_Data;
    alignas(std::hardware_constructive_interference_size) std::atomic<std::size_t> m_ReadIdx  {0};
    alignas(std::hardware_constructive_interference_size) std::atomic<std::size_t> m_WriteIdx {0};
};