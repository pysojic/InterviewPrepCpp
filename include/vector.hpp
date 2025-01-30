#pragma once

template<typename T>
class vector
{

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