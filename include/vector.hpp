#pragma once

namespace impl
{
    template<typename T>
    class Vector
    {
    public:
        Vector() : data_{nullptr}, capacity_{}, size_{}
        {
            reallocate(2);
        }

        void push_back(const T& value)
        {
            if (size_ >= capacity_)
                reallocate(capacity_ + capacity_/2);

            data_[size_] = std::move(value);
            size_++;
        }

        std::size_t size() const { return size_; }

    private:
        void reallocate(std::size_t newCapacity)
        {
            T* new_data = (T*)::operator new(newCapacity * sizeof(T));

            if (newCapacity < size_)
            {
                size_ = newCapacity;
            }

            for (std::size_t i = 0; i < size_; ++i)
            {
                new_data[i] = std::move(data_[i]);
            }

            for (std::size_t i = 0; i < size_; ++i)
            {
                data_[i].~T();
            }

            ::operator delete(data_, capacity_ * sizeof(T));
            data_ = new_data;
            capacity_ = newCapacity;
        }

    private:
        T* data_;
        std::size_t capacity_;
        std::size_t size_;
    };
}