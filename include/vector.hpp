#pragma once

namespace impl
{
    template<typename T>
    class Vector
    {
    public:
        vector() : data_{nullptr}, capacity_{}, size_{}
        {}

        void push_back(const T& value)
        {
            if (size >= capacity)
                reallocate(capacity + capacity/2);

            data[size] = std::move(value);
            size++;
        }

        std::size_t size() const { return size_; }

    private:
        void reallocate(std:size_t newCapacity)
        {
            T* new_data = (T*)::operator new(new_capacity * sizeof(T));

            if (newCapacity < size_)
            {
                size_ = newCapacity;
            }

            for (std::size_t i = 0; i < size_; ++i)
            {
                new_data[i] = std::move(data[i]);
            }

            delete[] data_;
            data_ = new_data;
            capacity_ = newCapacity;
        }

    private:
        T* data_;
        std::size_t capacity_;
        std:size_t size_;
    };
}