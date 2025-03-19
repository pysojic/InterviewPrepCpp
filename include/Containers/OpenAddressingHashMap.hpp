#pragma once

#include <tuple>
#include <vector>
#include <stdexcept>

namespace pysojic
{
    template <typename Key, typename Value, typename HashFunction = std::hash<Key>>
    class OpenAddressingHashMap
    {
        class Entry
        {
            friend class OpenAddressingHashMap;
        public:
            enum class State
            {
                EMPTY,
                OCCUPIED,
                DELETED
            };

            Entry()
                : key_{}, value_{}, state_{State::EMPTY}
            {}

            Entry(const Key& key, const Value& val)
                : key_{key}, value_{val}, state_{State::OCCUPIED}
            {}
            
        private:
            Key key_;
            Value value_;
            State state_ = State::EMPTY;
        };

    public:
        OpenAddressingHashMap();

        void insert(const Key& key, const Value& val);
        void remove(const Key& key);
        Value& operator[](const Key& key);

        size_t size() const noexcept { return m_NumElems; }
        size_t capacity() const noexcept { return m_Arr.capacity(); }
        size_t load_factor() const noexcept { return static_cast<double>(m_NumElems) / m_Arr.size(); }

    private:
        size_t  hash_function(const Key& key);

    private:
        std::vector<Entry> m_Arr;
        size_t m_NumElems;
        double m_MaxLoadFactor;
    };

    //------------Implementation--------------

    template <typename Key, typename Value, typename HashFunction>
    size_t OpenAddressingHashMap<Key, Value, HashFunction>::hash_function(const Key& key)
    {
        return HashFunction{}(key) % m_Arr.size();
    }

    template <typename Key, typename Value, typename HashFunction>
    OpenAddressingHashMap<Key, Value, HashFunction>::OpenAddressingHashMap()
        : m_Arr(3), m_NumElems{}, m_MaxLoadFactor{0.8}
    {
    }

    template <typename Key, typename Value, typename HashFunction>
    void OpenAddressingHashMap<Key, Value, HashFunction>::insert(const Key& key, const Value& val)
    {
        if (load_factor() >= m_MaxLoadFactor)
            m_Arr.resize(m_Arr.size() * 2);

        
        
        size_t index = hash_function(key);
        
        while (m_Arr[index].state_ == Entry::State::OCCUPIED)
        {
            index = (index + 1) % m_Arr.size();
        }

        m_Arr[index] = Entry(key, val);
        ++m_NumElems;
    }

    template <typename Key, typename Value, typename HashFunction>
    Value& OpenAddressingHashMap<Key, Value, HashFunction>::operator[](const Key& key)
    {
        size_t index = hash_function(key);

        if (m_Arr[index].state_ != Entry::State::OCCUPIED)
        {
            m_Arr[index] = Entry(key, Value{});
            return m_Arr[index].value_;
        }

        while(m_Arr[index].key_ != key)
        {
            index = (index + 1) % m_Arr.size();
        }

        return m_Arr[index].value_;
    }

    template <typename Key, typename Value, typename HashFunction>
    void OpenAddressingHashMap<Key, Value, HashFunction>::remove(const Key& key)
    {
        size_t index = hash_function(key);

        if (m_Arr[index].state_ != Entry::State::OCCUPIED)
        {
            throw std::out_of_range("Key not found");
        }

        while(m_Arr[index++].key_ != key);

        m_Arr[index].state_ = Entry::State::DELETED;
        --m_NumElems;
    }
}